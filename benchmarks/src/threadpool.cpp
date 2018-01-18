/*****************************************************
 * file: threadpool.cpp
 * written: 15.01.2017
 * synopsis: Implementation for the ThreadPool class 
 *           using task-based parallelism
 *           insted of thread-based.
 * author: Vladyslav Podilnyk
 * ***************************************************/

#include "threadpool.h"

namespace refThreadPool
{
 
int fib(int n)
{
    if (n < 3) return 1;
    else return fib(n-1) + fib(n-2);
}


ThreadPool::ThreadPool()
{
    for (size_t i = 0 ; i < num_of_tasks; ++i)
        tasks.push_front(std::bind([](int order)->void {
                                        int fibonacci_num = fib(order); 
                                    }, i));
}


void ThreadPool::run()
{
    while (!tasks.empty()) {
        workers.push_front(std::future <void>(std::async(std::launch::async, tasks.front())));
        tasks.pop_front();
    }

    while (!workers.empty()) {
        workers.front().wait();
        workers.pop_front();
    }
}

} // refThreadPool

