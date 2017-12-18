/*****************************************************
 * file: pqueue.h
 * written: 18.12.2017
 * last modified:
 * synopsis: Interface for the thread safe queue class
 * author: Vladyslav Podilnyk
 * ***************************************************/

#ifndef PQUEUE_H
#define PQUEUE_H

#include <functional>
#include <mutex>
#include <queue>
#include <condition_variable>
#include "netpod_helpers.h"

namespace netpod {

static constexpr size_t max_task_number {100};

struct ConditionalLock
{
    std::mutex                        mutex;
    std::condition_variable           signal;    
};

class ConcurrentQueue
{
    public:
        ConcurrentQueue() = default;
        ConcurrentQueue(const ConcurrentQueue &) = delete;
        ConcurrentQueue(const ConcurrentQueue &&) = delete;
        ConcurrentQueue& operator= (const ConcurrentQueue &) = delete;
        ConcurrentQueue& operator= (const ConcurrentQueue &&) = delete;
        void push(const std::function<void()> & element);
        void pop();
        std::function<void()> front();
        std::function<void()> back();
        bool empty();
        ssize_t size();
        void wait();
        void waitForEmptyQueue();
        ~ConcurrentQueue() = default;

    private:
        std::mutex                        mutex;
        ConditionalLock                   condlock;
        std::queue<std::function<void()>> queue;
};

} // netpod

#endif // PQUEUE_H