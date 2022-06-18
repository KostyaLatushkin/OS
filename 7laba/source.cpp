#include "windows.h"
#include <iostream>

using namespace std;


int main() {
	setlocale(LC_ALL, "ru");
	HANDLE hNamedPipe;
	hNamedPipe = CreateNamedPipe(
		L"\\\\.\\pipe\\demo_pipe",
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_MESSAGE | PIPE_WAIT,
		1,
		0,
		0,
		INFINITE,
		(LPSECURITY_ATTRIBUTES)NULL
	);
	ConnectNamedPipe(
		hNamedPipe,
		(LPOVERLAPPED)NULL
	);
	cout << "Клиент подключился!\n";
	
	cout << "Введите размер массива и элементы массива : ";
	int n;
	cin >> n;
	double* arr = new double[n], N;
	for (int i = 0;i < n;++i)
		cin >> arr[i];
	cout << "Введите число N : ";
	cin >> N;


	DWORD dwBytesWritten, dwBytesRead;
	WriteFile(hNamedPipe, &n, sizeof(int), &dwBytesWritten, NULL);
	WriteFile(hNamedPipe, &N, sizeof(double), &dwBytesWritten, NULL);
	for (int i = 0;i < n;++i)
		WriteFile(hNamedPipe, arr + i, sizeof(double), &dwBytesWritten, NULL);

	Sleep(100);
	int nNew;
	ReadFile(hNamedPipe, &nNew, sizeof(int), &dwBytesRead, NULL);
	double* SqArr = new double[nNew], sum;
	for (int i = 0;i < nNew;++i)
		ReadFile(hNamedPipe, SqArr + i, sizeof(double), &dwBytesRead, NULL);
	ReadFile(hNamedPipe, &sum, sizeof(double), &dwBytesRead, NULL);

	cout << "Полученный массив : ";
	for (int i = 0;i < nNew;++i)
		cout << SqArr[i] << "   ";
	cout << "\nСумма : " << sum << '\n';
  CloseHandle(hNamedPipe);
}
