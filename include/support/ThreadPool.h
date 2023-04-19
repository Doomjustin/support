#ifndef CPP_SUPPORT_THREAD_POOL_H
#define CPP_SUPPORT_THREAD_POOL_H

#include "FunctionWrapper.h"

#include <thread>
#include <utility>
#include <vector>
#include <future>
#include <condition_variable>
#include <queue>
#include <functional>
#include <type_traits>


namespace support {

static constexpr int DEFAULT_MAX_THREADS = 16;

// A simple thread pool
class ThreadPool {
public:
    ThreadPool(int max_threads = DEFAULT_MAX_THREADS);

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    ThreadPool(ThreadPool&& other) noexcept;
    ThreadPool& operator=(ThreadPool&& other) noexcept;

    ~ThreadPool();

    template<typename F, typename... Args>
    std::future<std::invoke_result_t<F, Args...>>
        submit(F f, Args&&... args)
    {
        using result_type = std::invoke_result_t<F, Args...>;

        std::packaged_task<result_type()> task{ std::bind(std::move(f), std::forward<Args>(args)...) };
        auto res = task.get_future();

        {
            std::unique_lock<std::mutex> lock{ m_ };
            tasks_.emplace(std::move(task));
            cv_.notify_one();
        }

        return res;
    }

    // 是否可以stop
    constexpr bool stopped() const noexcept { return done_ == false; }
    // 是否所有任务都在执行中，没有排队的任务了
    bool completed() const noexcept { return tasks_.empty(); }

    // 此时如果有未执行的任务将被抛弃
    void stop();

    // 确定调用restart前已经stop了
    void restart();

private:
    bool done_;
    std::mutex m_;
    std::condition_variable cv_;
    std::vector<std::thread> threads_;
    std::queue<FunctionWrapper> tasks_;

    void thread_work();
};

} // namespace support

#endif // CPP_SUPPORT_THREAD_POOL_H