#ifndef INCLUDE_BLOCKQUEUE_HPP_
#define INCLUDE_BLOCKQUEUE_HPP_

#include <queue>
#include <vector>
#include <mutex>

template <class T>
class BlockQueue {
public:
	BlockQueue();
	~BlockQueue();
	
	void push(const T&);
	T pop();
	T peek();
	bool try_peek(T& );
private:
	std::mutex mutex_;
	std::condition_variable not_empty_;
	std::queue<T> q_;
	BlockQueue(const BlockQueue&);
};
#endif
