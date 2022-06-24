#include <windows.h>
#include <iostream>
typedef unsigned long long ull;


int main(int argc, char* argv[]) {
	ull fst = 0ull,
		scd = 1ull;
	int count = 1;
	std::cout << count++ << " : " << scd << '\n';
	while (true) {
		Sleep(100);
		ull help = fst + scd;
		fst = scd;
		scd = help;
		cout << count++ << " : " << help << '\n';
	}
}
