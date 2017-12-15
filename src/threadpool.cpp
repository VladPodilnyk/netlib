/*****************************************************
 * file: threadpool.cpp
 * written: 28.12.2017
 * last modified:
 * synopsis: Implementation for the ThreadPool class
 * author: Vladyslav Podilnyk
 * ***************************************************/

#include "netlib/threadpool.h"

namespace netpod {

ThreadPool::ThreadPool(const int thread_num)
{
    num_of_theads = thread_num;
    controller = std::make_shared<AssignController> ();
    controller->counter.store(num_of_theads);

    // threads initialization
    int i = 0;
    while (i < num_of_theads) {
        auto thread_descriptor = std::make_shared<Context> ();
        thread_descriptor->is_working = false;
        thread_descriptor->controller_ptr = controller;
        thread_descriptor->func_pointer = nullptr;
        thread_descriptor->thread = std::thread(&ThreadPool::worker, this, thread_descriptor);
        threads.push_back(thread_descriptor);
        ++i;
    }

    // creating main thread
    is_running = true;
    main_thread = std::thread(&ThreadPool::spawn, this);
}


void ThreadPool::push(const std::function<void()> & task)
{
    workqueue.push(task);
}


void ThreadPool::spawn()
{
    std::vector<std::shared_ptr<Context>>::iterator cache = threads.begin();

    while (is_running) {
        // here worker loop will go sleep until new tasks arrive; all actions are hiden 
        workqueue.wait();
        if (!workqueue.back())
            return;

        bool spurious_wakeup = true;
        std::vector<std::shared_ptr<Context>>::iterator current_worker;

        while (spurious_wakeup) {

            if (cache == threads.end())
                cache = threads.begin();

            std::unique_lock<std::mutex> lck(controller->mutex);
            controller->condvar.wait(lck, [&]()->bool{ return controller->counter.load(); });

            for (current_worker = cache; current_worker != threads.end(); ++current_worker) {
                std::unique_lock<std::mutex> lck((*current_worker)->mutex, std::defer_lock);

                if (lck.try_lock() && !(*current_worker)->is_working) {
                    spurious_wakeup = false;
                    cache = current_worker + 1;
                    break;
                }
            }
        }

        controller->counter--;

        {
            std::lock_guard<std::mutex> lck((*current_worker)->mutex);
            (*current_worker)->func_pointer = workqueue.front();
            (*current_worker)->is_working = true;
        }

        workqueue.pop();
        (*current_worker)->signal.notify_one();

    }
}


void ThreadPool::worker(std::shared_ptr<Context> context)
{
    while (true) {
        {
            std::unique_lock<std::mutex> lck(context->mutex);
            context->signal.wait(lck, [&](){ return context->is_working; }); // wait until task is given

            if (!context->func_pointer) {
                context->controller_ptr->counter++;
                context->controller_ptr->condvar.notify_one();
                context->is_working = false;
                return;
            }

            context->func_pointer();
            context->is_working = false;

            context->controller_ptr->counter++;
            context->controller_ptr->condvar.notify_one();
        }
        context->signal.notify_one();  // notify scheduler that thread is ready to work
    }
}


ThreadPool::~ThreadPool()
{
    workqueue.waitForEmptyQueue();

    for (auto it = threads.begin(); it != threads.end(); ++it) {
        // give signal to all working threads complete tasks and exit
        {
            std::unique_lock<std::mutex> lck((*it)->mutex);
            (*it)->signal.wait(lck, [&]()->bool{ return !(*it)->is_working; });
            (*it)->func_pointer = nullptr;
            (*it)->is_working = true;
            (*it)->controller_ptr->counter--;
        }
        (*it)->signal.notify_one();
        (*it)->thread.join();
    }

    workqueue.push(nullptr);
    main_thread.join();
}

} // netpod
