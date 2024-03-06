#include <iostream>
#include <string>

#include "concurrent-executor.hpp"

int main()
{
    auto func = [](std::string s) {
        std::cout << s << std::endl;
    };
    ConcurrentExecutor<std::string> exec(func);
    std::cout << "Hello World" << std::endl;
}
