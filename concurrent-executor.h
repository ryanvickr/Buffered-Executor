// Author: Ryan Vickramasinghe
#ifndef CONCURRENT_EXECUTOR_H
#define CONCURRENT_EXECUTOR_H

#include <queue>

// This class is designed to accept arbitrary objects, and queue
// them for concurrent execution, by an arbitrary executor.
template <typename T>
class ConcurrentExecutor {
 public:
    explicit ConcurrentExecutor(std::function<void(T)> executor_func);

 private:
    std::function<void(T)> executor_func_;
};

#endif // CONCURRENT_EXECUTOR_H
