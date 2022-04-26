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
        std::time_t now = std::time(0);
        char *time_str = std::ctime(&now);
        block_queue.Push(value);
        size_t size = block_queue.Size();
        std::lock_guard<std::mutex> lock(out_mutex);
        std::cout << "Time : " << time_str << " thread id : " << id << " produces value : " << value 
            << " and now block queue's size is : " << size << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void Consumer()
{
    std::thread::id id = std::this_thread::get_id();
    while (true) {
        std::time_t now = std::time(0);
        char *time_str = std::ctime(&now);
        int value = block_queue.Take();
        size_t size = block_queue.Size();
        std::lock_guard<std::mutex> lock(out_mutex);
        std::cout << "Time : " << time_str << " thread id : " << id << " consumes value : " << value 
            << " when block queue's size is : " << size << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }    
}

int main()
{
    int producer_cnt = 5;
    int consumer_cnt = 8;
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