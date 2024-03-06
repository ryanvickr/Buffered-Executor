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

	~ConcurrentExecutor();

	// Submits an item to the executor queue for processing.
	void Submit(T& item);
	void Submit(T&& item);

	// Returns the current size of the buffer (i.e. how many items are
	// pending).
	size_t BufferSize();

 private:
	// The function used by the consumer thread.
    std::function<void(T)> executor_func_;
    std::mutex mu_;
    std::queue<T> queue_;
	std::condition_variable cv_;
};

template <typename T>
ConcurrentExecutor<T>::ConcurrentExecutor(std::function<void(T)> executor_func):
	executor_func_(std::move(executor_func)) {}

template <typename T>
ConcurrentExecutor<T>::~ConcurrentExecutor() {}

template <typename T>
void ConcurrentExecutor<T>::Submit(T& item) {
	std::lock_guard<std::mutex> lock(mu_);
	queue_.push(item);
	cv_.notify_one();
}

template <typename T>
void ConcurrentExecutor<T>::Submit(T&& item) {
	std::lock_guard<std::mutex> lock(mu_);
	queue_.emplace(item);
	cv_.notify_one();
}

template <typename T>
size_t ConcurrentExecutor<T>::BufferSize() {
	std::lock_guard<std::mutex> lock(mu_);
	return queue_.size();
}

#endif // CONCURRENT_EXECUTOR_H
