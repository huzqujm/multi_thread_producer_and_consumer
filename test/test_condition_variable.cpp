#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

bool is_ready = false;
std::mutex mtx;
std::condition_variable c_var;

void print(const int& num) {
	std::unique_lock<std::mutex> lock(mtx);
	while (!is_ready) {
		c_var.wait(lock);
	}
	std::cout << num << std::endl;
}

void go() {
	std::unique_lock<std::mutex> lock(mtx);
	is_ready = true;
	std::cout << "All thread are ready, go!" << std::endl;
	c_var.notify_all();
}

int main(int argc, char* argv[]) {
	std::vector<std::thread> t_pool;
	for (int i = 0; i < 10; ++i) {
		t_pool.push_back(std::thread(print, i));
	}
	go();
	for (int i = 0; i < 10; ++i) {
		if (t_pool[i].joinable()) {
			t_pool[i].join();
		}
	}
	return 0;
}
