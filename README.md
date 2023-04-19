# C++ Support Library

## Components

1. [x] Log
    > 参见test/test_log.cpp

    ```c++
    support::log::info("support {} format", "fmt");
    support::log::error("support {} format", 1);
    ```

2. [x] thread pool
    > 参见test/test_thread_pool.cpp

    ```c++
    int f(int x, int y) { return std::pow(x, y); }

    support::ThreadPool tp{};
    auto res = tp.submit(f, 2, 9);

    // res.get() is 2**9
    ```

3. [x] timer
    > 参见test/test_timer.cpp

    ```c++
    using namespace std::chrono_literals;
    support::TimerManager timer_manager{};
    timer_manager.add_timer(5s, 2s, test, 1);                   // 5s后执行test，且每隔2s再次执行
    timer_manager.add_timer(1s, once_task);                     // 1s后执行一次once_task
    timer_manager.add_timer(2s, 2s, &A::test_member, a, 1);     // 2s后执行类A的成员函数test_member，每隔2s再次执行

    timer_manager.run();        // 启动manager以开始管理执行注册的timer
    ```
