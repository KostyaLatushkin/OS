#include <iostream>
#include <thread>
#include <chrono>
#include <vector>

void thread(std::vector<short> const& v, uint32_t const& len) {
	int _count = 0;
	for (int i = 0;i < len;++i) {
		if (v[i] % 9 == 0 && v[i] < 0) {
			++_count;
			std::cout << v[i] << '\n';
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(30));
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
	std::thread thr(thread, std::cref(v), std::cref(n));

	thr.join();
}
