#include <iostream>
#include <vector>
#include <string>
#include <thread>

void print(const int& i) {
	std::cout << i << std::endl;
}

int main(int argc, char* argv[]) {
	std::vector<std::thread> t_pool;
	for (int i = 0; i < 10; ++i) {
		t_pool.push_back(std::thread(print, i));
	}
	for (int i = 0; i < 10; ++i) {
		if (t_pool[i].joinable()) {
			t_pool[i].join();
		}
	}
	return 0;
}
