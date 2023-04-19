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

4. [ ] net tools(unfinished)

    收集在support/net/下，核心在于Connection，客户端和服务器端都能基于Connection进行通信。(目前只有基于posix的实现。)

    - 对于服务器端，提供Acceptor

    ```c++
    support::net::Acceptor acceptor{}; // 默认IPv4, TCP协议
    // Acceptor acceptor{ Domain::IPv6, Type::UDP } // 也可以手动指定协议
    acceptor.bind(Endpoint{ 12345 });  // bind
    acceptor.listen();                 // 默认5
    auto connection = acceptor.accept();  // 通过此connection来与client通信

    // 和cilent交互
    auto received = connection->receive(1024); // 指定本次读取的最大数据量
    connection->send(received);   // 将收到的数据原样放回，实现echo server的效果
    ```

    - 对于客户端，提供Connector

    ```c++
    support::net::Connector connector{}; // 默认IPv4, TCP协议
    // Connector connector{ Domain::IPv6, Type::UDP }; // 也可以手动指定协议
    auto connection = connector.bind(Endpiont{ "localhost", 12345 });  // 通过此connection来与server通信

    // 和server交互
    connection->send("hello world");
    auto received = connection->receive(1024);
    ```

    - 基于Acceptor的简单迭代式服务器，BasicServer，继承BasicServer并实现on_connected方法。可参考examples/simple_server.cpp

    ```c++
    using namespace support::net;

    class EchoServer: public BasicServer {
    public:
        EchoServer(const Endpoint& self)
          : BasicServer{ self }
        {}

        ~EchoServer() {}

        void on_connected(std::shared_ptr<Connection> connection) override
        {
            // 这是一个echo server
            auto received_ = connection->receive(1024);
            connection->send(received_);
        }

        // 非必须实现
        void on_start() override
        {
            support::log::info("echo server started");
        }

        // 非必须实现
        void on_stop() override
        {
            support::log::info("echo server stopped");
        }
    };
    ```

    - 基于Connector实现的简单迭代式客户端，BasicClient，继承BasicServer并实现on_connected方法。可参考examples/simple_client.cpp。
      和BasicServer的不同在于，start后只会执行一次on_connected方法，而BasicServer则是不停轮询等待新的连接到来

    ```c++
    using namespace support::net;

    class EchoClient: public BasicClient {
    public:
        EchoClient(const Endpoint& peer)
          : BasicClient{ peer }
        {}

        ~EchoClient() {}

        void on_connected(std::shared_ptr<Connection> connection) override
        {
            connection->send("hello world");
            auto received = connection->receive(1024);
            fmt::print("received from server: {}\n", received);
        }
    };
    ```
