#include <iostream>
#include <string>
#include <thread>

#include "concurrent-executor.hpp"

int main()
{
    auto func = [](std::string s) {
        std::cout << s << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    };
    ConcurrentExecutor<std::string> exec(func);

    std::string test = "test";
    exec.Submit("hello");
    exec.Submit(test);
    std::cout << "Queue size: " << exec.BufferSize() << std::endl;

    return 0;
}
