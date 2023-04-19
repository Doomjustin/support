#include "support/ThreadPool.h"

#include <cstdlib>
#include <iostream>
#include <cmath>
#include <functional>
#include <mutex>


int f(int x, int y) { return std::pow(x, y); }

std::mutex m;

void g(int i)
{
    std::lock_guard<std::mutex> lock{ m };
    std::cout << "g(" << i <<")" << std::endl;
}

constexpr static int TASKS = 10000;

int main()
{
    support::ThreadPool tp{};

    auto res = tp.submit(f, 2, 9);

    std::cout << res.get() << std::endl;

    for (auto i = 0; i < TASKS; ++i)
        tp.submit(g, i);

    while (!tp.completed());

    return EXIT_SUCCESS;
}