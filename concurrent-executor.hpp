// Author: Ryan Vickramasinghe
#ifndef CONCURRENT_EXECUTOR_H
#define CONCURRENT_EXECUTOR_H

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
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
	void LaunchExecutors(int thread_count);
	void Executor(int id);

	// The function used by the consumer thread.
    std::function<void(T)> executor_func_;
	// The thread pool
	std::vector<std::thread> executor_threads_;

    std::mutex mu_;
    std::queue<T> queue_;
	std::condition_variable cv_;
	std::atomic_bool done_;
};

template <typename T>
ConcurrentExecutor<T>::ConcurrentExecutor(std::function<void(T)> executor_func):
	executor_func_(std::move(executor_func)) {
	LaunchExecutors(1);
}

template <typename T>
ConcurrentExecutor<T>::~ConcurrentExecutor() {
	done_.exchange(true);
	for (auto& thread : executor_threads_) {
		thread.join();
	}
}

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

template <typename T>
void ConcurrentExecutor<T>::LaunchExecutors(int thread_count) {
	for (int i = 0; i < thread_count; i++) {
		executor_threads_.emplace_back(&ConcurrentExecutor::Executor, this, i);
	}
}

template <typename T>
void ConcurrentExecutor<T>::Executor(int id) {
	std::cout << "Started executor " << id << std::endl;

	while (!done_) {
		// Get the next item from the queue.
		std::unique_lock<std::mutex> lock(mu_);
		cv_.wait(lock, [this]{ return !queue_.empty(); });
		T data = queue_.front();
		queue_.pop();
		lock.unlock();

		// Do the work outside of the lock.
		executor_func_(data);
	}

	std::cout << "Exited executor " << id << std::endl;
}

#endif // CONCURRENT_EXECUTOR_H
