#include <iostream>
#include <string>

#include "concurrent-executor.hpp"

int main()
{
    auto func = [](std::string s) {
        std::cout << s << std::endl;
    };
    ConcurrentExecutor<std::string> exec(func);

    std::string test = "test";
    exec.Submit("hello");
    exec.Submit(test);
    std::cout << "Queue size: " << exec.BufferSize() << std::endl;
    std::cout << "Hello World" << std::endl;
}
