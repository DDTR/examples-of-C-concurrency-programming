#ifndef BLOCKQUEUE_H_
#define BLOCKQUEUE_H_

#include <queue>
#include <mutex>
#include <condition_variable>
#include <cassert>
template <typename T>
class BlockQueue
{
public:
    BlockQueue(size_t capacity) 
        : capacity_(capacity),
        queue_(),
        mutex_(),
        cond_()
    {}
    
    void Push(T &&value)
    {
        std::unique_lock<std::mutex> lock(mutex_);
        while (queue_.size() >= capacity_) {
            cond_.wait(lock);
        }
        assert(queue_.size() < capacity_);
        queue_.push(value);
        cond_.notify_one();
    }
    
    // TODO
    void Push(const T &value)
    {
        std::unique_lock<std::mutex> lock(mutex_);
        while (queue_.size() >= capacity_) {
            cond_.wait(lock);
        }
        assert(queue_.size() < capacity_);
        queue_.push(value);
        cond_.notify_one();
    }

    T Take()
    {
        std::unique_lock<std::mutex> lock(mutex_);
        while (queue_.size() == 0) {
            cond_.wait(lock);
        }
        assert(queue_.size() > 0);
        T value(std::move(queue_.front()));
        queue_.pop();
        cond_.notify_one();
        return value;
    }

    size_t Size()
    {
        std::unique_lock<std::mutex> lock(mutex_);
        return queue_.size();
    }

private:
    size_t capacity_;
    std::queue<T> queue_;
    std::mutex mutex_;  
    std::condition_variable cond_; 
};

#endif