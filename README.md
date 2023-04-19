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
