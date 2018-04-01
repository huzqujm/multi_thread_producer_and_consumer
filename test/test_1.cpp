#include <iostream>
#include <thread>
#include <vector>
#include <string>

void print(const std::string& str) {
	std::cout << str << std::endl;
}

int main(int argc, char* argv[]) {
	std::thread t1(print, "hello world");
	t1.join();
	return 0;
}
