#include <thread>


#include <iostream>

void test() {
	std::cout << "test" << std::endl;
}


int main (int argc, char** argv) {
std::thread t(test);
t.join();
}
