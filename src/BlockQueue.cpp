#include "inc/BlockQueue.hpp"
#include <iostream>

template <class T>
BlockQueue<T>::BlockQueue():
q_(std::queue<T>()) {}

template <class T>
void BlockQueue<T>::push(const T& t) {
	std::unique_lock<std::mutex> lock(mutex_);
	q_.push(t);
	not_empty_.notify_all();
}

template <class T>
T BlockQueue<T>::pop() {
	std::unique_lock<std::mutex> lock(mutex_);
	while (q_.empty()) {
		not_empty_.wait(lock);
	}
	T ret = q_.peek();
	q_.pop();
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
