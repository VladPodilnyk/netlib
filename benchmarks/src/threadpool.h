/*****************************************************
 * file: threadpool.h
 * written: 15.01.2017
 * synopsis: API for the ThreadPool class 
 *           using task-based parallelism
 *           insted of thread-based.
 * author: Vladyslav Podilnyk
 * ***************************************************/

#ifndef __THREAD_POOL_H
#define __THREAD_POOL_H

#include <cstdlib>
#include <future>
#include <forward_list>
#include <functional>
#include <chrono>
#include <random>

namespace refThreadPool
{

static size_t num_of_tasks {22};
 
int fib(int n);

class ThreadPool
{
    public:
        ThreadPool();
        ThreadPool(const ThreadPool &) = delete;
        ThreadPool(const ThreadPool &&) = delete;
        ThreadPool& operator= (const ThreadPool &) = delete;
        ThreadPool& operator= (const ThreadPool &&) = delete;
        void run();
        virtual ~ThreadPool() = default;

    private:
        // data
        std::forward_list <std::function <void()>> tasks;
        std::forward_list <std::future <void>>    workers;




};

} // refThreadPool

#endif // __THREAD_POOL_H