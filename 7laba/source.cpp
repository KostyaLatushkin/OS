#include <iostream>
#include <Windows.h>

using std::cin;
using std::cout;


class MonitorStack {
	double* arr;
	size_t const n;
	size_t ind;

	HANDLE synchronized_print, access;
public:
	MonitorStack(size_t const& n, HANDLE& synchronized_print) :n(n), arr(new double[n]), ind(0), synchronized_print(synchronized_print) {
		access = CreateSemaphore(NULL, 1, 1, NULL);
	};
	~MonitorStack() {
		delete[] arr;
		CloseHandle(synchronized_print);
		CloseHandle(access);
	}

	void Add(double const& elem) {
		while (ind >= n);
		WaitForSingleObject(access, INFINITE);
		arr[ind++] = elem;
		WaitForSingleObject(synchronized_print, INFINITE);
		ReleaseSemaphore(access, 1, NULL);
	}
	double Pop() {
		while (ind <= 0);
		WaitForSingleObject(access, INFINITE);
		double v = arr[--ind];
		WaitForSingleObject(synchronized_print, INFINITE);
		ReleaseSemaphore(access, 1, NULL);
		return v;
	}
};

struct MyStruct
{
	double* arr;
	UINT N;
};
MonitorStack* MSt;
HANDLE synchronized_print = CreateSemaphore(NULL, 1, 1, NULL);

DWORD WINAPI producer(void* p) {
	MyStruct* str = (MyStruct*)p;
	for (int i = 0;i < str->N;++i) {
		MSt->Add(str->arr[i]);
		printf("\tПроизведен элемент (продукт) %.2f \n", str->arr[i]);
		ReleaseSemaphore(synchronized_print, 1, NULL);
	}
	return 0;
}
DWORD WINAPI consumer(void* p) {
	UINT* N = (UINT*)p;
	for (int i = 0;i < *N;++i) {
		double p = MSt->Pop();
		printf("\tУпотреблен элемент (продукт) %.2f \n", p);
		ReleaseSemaphore(synchronized_print, 1, NULL);
	}
	return 0;
}

int main() {
	setlocale(LC_ALL, "ru");
	
	cout << "Введите количество процессов и размер стека:\n";
	int n, len;
	cin >> n >> len;
	MSt = new MonitorStack(len, synchronized_print);
	HANDLE* hprod = new HANDLE[n],
		* hcons = new HANDLE[n];
	DWORD* iprod = new DWORD[n],
		* icons = new DWORD[n];

	double** arr = new double* [n];
	UINT* K1 = new UINT[n];
	UINT* K2 = new UINT[n];
	for (int i = 0;i < n;++i) {
		cout << "Поток №" << i + 1 << '\n';
		cout << "Введите число добавляемых продуктов:\n";
		cin >> K1[i];
		arr[i] = new double[K1[i]];
		cout << "Введите добавляемые продукты:\n";
		for (int j = 0;j < K1[i];++j)
			cin >> arr[i][j];
		cout << "Введите число удаляемых продуктов:\n";
		cin >> K2[i];
	}



	for (int i = 0;i < n;++i) {
		hprod[i] = CreateThread(NULL, NULL, producer, new MyStruct{arr[i],K1[i]}, 0, &iprod[i]);
		hcons[i] = CreateThread(NULL, NULL, consumer, &(K2[i]), 0, &icons[i]);
	}

	WaitForMultipleObjects(n, hprod, TRUE, INFINITE);
	WaitForMultipleObjects(n, hcons, TRUE, INFINITE);
	for (int i = 0;i < n;++i) {
		CloseHandle(hprod[i]);
		CloseHandle(hcons[i]);
	}
	delete[] K1;
	delete[] K2;
	for (int i = 0;i < n;++i)
		delete[] arr[i];
	delete[] arr;
}

