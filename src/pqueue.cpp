/*****************************************************
 * file: pqueue.cpp
 * written: 28.12.2017
 * last modified:
 * synopsis: Implementation of thread safe queue
 * author: Vladyslav Podilnyk
 * ***************************************************/

#include "netlib/pqueue.h"

namespace netpod {

void ConcurrentQueue::push(const std::function<void()> & element)
{
    size_t size;
    std::function<void()> queue_member = element;
    {
        std::lock_guard<std::mutex> lock(mutex);
        queue.push(queue_member);
        size = queue.size();
    }

    if (size == 1)
        condlock.signal.notify_one();

    if (size > max_task_number)
        throw QueueOverflow();
}


void ConcurrentQueue::pop()
{
    ssize_t size;
    {
        std::lock_guard<std::mutex> lock(mutex);
        queue.pop();
        size = queue.size();
    }

    if (size == 0)
        condlock.signal.notify_one();    
}


std::function<void()> ConcurrentQueue::front()
{
    std::function<void()> task;
    {
        std::lock_guard<std::mutex> lock(mutex);
        task = queue.front();
    }

    return task;
}


std::function<void()> ConcurrentQueue::back()
{
    std::function<void()> task;
    {
        std::lock_guard<std::mutex> lock(mutex);
        task = queue.back();
    }

    return task;
}


bool ConcurrentQueue::empty()
{
    bool empty;
    {
        std::lock_guard<std::mutex> lock(mutex);
        empty = queue.empty();
    }

    return empty;
}


ssize_t ConcurrentQueue::size()
{
    ssize_t size;
    {
        std::lock_guard<std::mutex> lock(mutex);
        size = queue.size();
    }

    return size;
}


void ConcurrentQueue::wait()
{
    std::unique_lock<std::mutex> lock(condlock.mutex);
    condlock.signal.wait(lock, [&]()->bool{ return !empty(); });
}


void ConcurrentQueue::waitForEmptyQueue()
{
    std::unique_lock<std::mutex> lock(condlock.mutex);
    condlock.signal.wait(lock, [&]()->bool{ return empty(); });
}

} // netpod
