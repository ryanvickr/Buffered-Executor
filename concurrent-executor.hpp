// Author: Ryan Vickramasinghe
#ifndef CONCURRENT_EXECUTOR_H
#define CONCURRENT_EXECUTOR_H

#include <functional>
#include <queue>
#include <utility>

/* This class is designed to accept arbitrary objects, and queue them for 
concurrent execution by an arbitrary executor. */
template <typename T>
class ConcurrentExecutor{
 public:
   explicit ConcurrentExecutor(std::function<void(T)> executor_func);

   void Submit(T item);
//    void Submit(T&& item);

 private:
   std::function<void(T)> executor_func_;
};

template <typename T>
ConcurrentExecutor<T>::ConcurrentExecutor(std::function<void(T)> executor_func):
	executor_func_(std::move(executor_func)) {}

#endif // CONCURRENT_EXECUTOR_H
