#include "support/Timer.h"
#include "support/Log.h"

#include <cstdlib>
#include <thread>
#include <iostream>


void test(int i)
{
    support::log::info("i = {}", i);
}

void once_task()
{
    support::log::info("this is an one time task");
}

struct A {
    void test_member(int x)
    {
        support::log::info("this is a class task");
    }
};


int main()
{
    A a{};

    using namespace std::chrono_literals;
    support::TimerManager timer_manager{};
    timer_manager.add_timer(5s, 2s, test, 1);
    timer_manager.add_timer(1s, once_task);
    timer_manager.add_timer(2s, 2s, &A::test_member, a, 1);

    timer_manager.run();

    return EXIT_SUCCESS;
}