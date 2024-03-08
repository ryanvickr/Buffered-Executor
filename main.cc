#include <iostream>
#include <string>
#include <thread>

#include "concurrent-executor.hpp"

int main()
{
    auto func = [](std::string s) {
        std::cout << s << std::endl;
    };
    ConcurrentExecutor<std::string> exec(func, 2);

    std::string test = "test";
    exec.Submit("hello");
    exec.Submit(test);
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "Queue size: " << exec.BufferSize() << std::endl;

    return 0;
}
