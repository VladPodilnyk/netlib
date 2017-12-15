/****************************************************
 * file: threadpool.h
 * written: 15.12.2017
 * last modified:
 * synopsis: Interface for the thread pool class
 * author: Vladyslav Podilnyk
 * **************************************************/

#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <queue>
#include <vector>
#include <atomic>
#include "pqueue.h"


namespace netpod {

struct AssignController
{
    std::atomic<int>        counter;
    std::condition_variable condvar;
    std::mutex              mutex;
};


struct Context
{
    bool                              is_working;      // signal to the thread to do work
    std::mutex                        mutex;
    std::thread                       thread;
    std::function<void()>             func_pointer;    // particular task
    std::condition_variable           signal;
    std::shared_ptr<AssignController> controller_ptr;  // actually semaphore that controls assignment process
};


class ThreadPool
{
    public:
        ThreadPool() {};
        ThreadPool(const ThreadPool &) = delete;
        ThreadPool(const ThreadPool &&) = delete;
        ThreadPool& operator= (const ThreadPool &) = delete;
        ThreadPool& operator= (const ThreadPool &&) = delete;
        explicit ThreadPool(const int thread_num);
        void push(const std::function<void()> & task);
        virtual ~ThreadPool();

    private:
        // data
        std::shared_ptr<AssignController>     controller;
        std::vector<std::shared_ptr<Context>> threads;       // pool of threads
        ConcurrentQueue                       workqueue;     // queue of incoming tasks
        std::thread                           main_thread;   // main thread that controlls work of the pool
        int                                   num_of_theads;
        bool                                  is_running;

        // functions
        void spawn();
        void worker(std::shared_ptr<Context> context);
};


} // netpod

#endif // THREAD_POOL_H