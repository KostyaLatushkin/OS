#include <iostream>
#include <thread>
#include <chrono>
#include <vector>

void thread(std::vector<short> const& v) {
	int _count = 0;
	for (auto val : v) {
		if (val % 9 == 0 && val < 0) {
			++_count;
			std::cout << val << '\n';
		}
		std::this_thread::sleep_for(boost::chrono::microseconds(30));
	}
	if (!_count)
		std::cout << "Элементы не найдены!\n";
}

int main() {
	setlocale(LC_ALL, "ru");
	std::cout << "Введите размер массива и его элементы:\n";
	uint32_t n;
	std::cin >> n;
	std::vector<short> v(n);
	for (int i = 0;i < n;++i)
		std::cin >> v[i];

	std::cout << "Worker:\n";
	std::thread thr(thread, std::cref(v));

	thr.join();
}
