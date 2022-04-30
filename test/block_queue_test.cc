#include <thread>
#include <random>
#include <iostream>
#include <chrono>
#include <vector>
#include <ctime>

#include "block_queue.h"

BlockQueue<int> block_queue(10);
std::mutex out_mutex;

void Producer()
{
    std::thread::id id = std::this_thread::get_id();
    while (true) {
        int value = rand();
        block_queue.Push(value);
        {
            std::lock_guard<std::mutex> lock(out_mutex);
            std::cout << "Thread id : " << id << " produces value : " << value << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void Consumer()
{
    std::thread::id id = std::this_thread::get_id();
    while (true) {
        int value = block_queue.Take();
        {
            std::lock_guard<std::mutex> lock(out_mutex);
            std::cout << "Thread id : " << id << " consumes value : " << value << std::endl;  
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }    
}

int main()
{
    int producer_cnt = 2;
    int consumer_cnt = 5;
    std::vector<std::thread> producer_vec;
    std::vector<std::thread> consumer_vec;
    for (int i = 0; i < producer_cnt; ++i) {
        producer_vec.emplace_back(std::thread(Producer));
    }
    for (int i = 0; i < consumer_cnt; ++i) {
        consumer_vec.emplace_back(std::thread(Consumer));
    }
    for (auto &t : producer_vec) {
        t.join();
    }
    for (auto &t : consumer_vec) {
        t.join();
    }
    return 0;
}