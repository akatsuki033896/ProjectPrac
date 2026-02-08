#include <cassert>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <iostream>
#include <format>
#include <thread>

std::queue<int> q;
std::mutex mtx;
std::condition_variable cv;

void push_product(int i) {
    {
        std::unique_lock<std::mutex> lk(mtx);
        q.push(i);
    } // 锁析构了
    cv.notify_one(); // 通知消费者有新产品
}

int pop_product() {
    std::unique_lock<std::mutex> lk(mtx);
    cv.wait(lk, [] { return !q.empty(); }); // 等待产品
    auto product = q.front(); // 取出产品
    if (product != 0) {
        q.pop();
    }
    return product;
}

void producer() {
    for (int i = 1; i < 100; i++) {
        push_product(i);
    }
    push_product(0); // 发送标志位，表示结束
}

void consumer(int id) {
    while (int product = pop_product()) {
        std::cout << std::format("消费者 {} 得到了 {}\n", id, product);
    } // 不为0时
}

int main() {
    std::jthread producer1_thread(producer); // C++20 引入的线程类，自动 join
    std::jthread producer2_thread(producer);
    std::vector<std::jthread> consumers_threads;
    for (int i = 0; i < 32; i++) {
        std::jthread consumer_thread(consumer, i);
        consumers_threads.push_back(std::move(consumer_thread)); // 不移动的话就会变成拷贝，但线程不可拷贝，因为持有资源
    }
    // for (int i = 0; i < 32; i++) {
    //     consumers_threads[i].join(); // 等待所有消费者线程完成
    // }
    // producer1_thread.join(); // 等待生产者线程完成
    // producer2_thread.join();
    push_product(0); // 通知消费者结束
    return 0;
}
