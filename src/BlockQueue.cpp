#include "inc/BlockQueue.hpp"
#include <iostream>

template <class T>
BlockQueue<T>::BlockQueue():
q_(std::queue<T>()),
max_size_(20){}

template <class T>
BlockQueue<T>::BlockQueue(const int& max_size) :
	q_(std::queue<T>()),
	max_size_(max_size)
{}

template <class T>
int BlockQueue<T>::size() const {
	std::unique_lock<std::mutex> lock(mutex_);
	return q_.size();
}

template <class T>
void BlockQueue<T>::push(const T& t) {
	// Lock for queue write and read.
	std::unique_lock<std::mutex> lock(mutex_);
	// Lock for queue size. 
	std::unique_lock<std::mutex> size_lock(size_mutex_);
	// Block this thread if queue size greater than max_size.
	while (q_.size() > max_size_) size_cv_.wait(size_lock);
	q_.push(t);
	not_empty_.notify_one();
}

template <class T>
T BlockQueue<T>::pop() {
	std::unique_lock<std::mutex> lock(mutex_);
	// Empty queue. Block this thread until notify signal.
	while (q_.empty()) {
		not_empty_.wait(lock);
	}
	T ret = q_.peek();
	q_.pop();
	// Queue size small than max_size notify one Blocked thread
	if (q_.size() < max_size_) size_cv_.notify_one();
	lock.unlock();
	return ret;
}

template <class T>
T BlockQueue<T>::peek() {
	std::unique_lock<std::mutex> lock(mutex_);
	while (q_.empty()) {
		not_empty_.wait(lock);
	}
	T ret = q_.peek();
	return ret;
}	

template <class T>
bool BlockQueue<T>::try_peek(T& t) {
	std::unique_lock<std::mutex> lock(mutex_);
	if (q_.empty()) {
		return false;
	}
	t = q_.peek();
	return true;
}

template class BlockQueue<int>;
template class BlockQueue<float>;
template class BlockQueue<std::string>;
