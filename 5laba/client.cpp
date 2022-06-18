#include "windows.h"
#include <iostream>

using namespace std;


int main(int argc,char* argv[]) {
	setlocale(LC_ALL, "ru");
	HANDLE hWritePipe = (HANDLE)atoi(argv[1]),
			hReadPipe = (HANDLE)atoi(argv[2]);
	HANDLE hSumEnableRead, hServerEnableRead;
	hSumEnableRead = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"SumRead");
	hServerEnableRead = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"ServerRead");

	WaitForSingleObject(hSumEnableRead, INFINITE);
	
	int n, Sqn = 0;
	double N;
	DWORD dwBytesRead, dwBytesWrite;
	ReadFile(hReadPipe, &n, sizeof(int), &dwBytesRead, 0);
	ReadFile(hReadPipe, &N, sizeof(double), &dwBytesRead, 0);
	double* arr = new double[n],
		* SqArr = new double[n];
	for (int i = 0;i < n;++i)
		ReadFile(hReadPipe, arr + i, sizeof(double), &dwBytesRead, 0);
	for (int i = 0;i < n;++i)
		if (arr[i] > N)
			SqArr[Sqn++] = arr[i];

	cout << "Полученный массив : ";
	double sSquares = 0.;
	for (int i = 0;i < Sqn;++i) {
		cout << SqArr[i] << "   ";
		SqArr[i] *= SqArr[i];
		sSquares += SqArr[i];
	}
	cout << "\nСумма квадратов : " << sSquares << '\n';

	WriteFile(hWritePipe, &Sqn, sizeof(int), &dwBytesWrite, NULL);
	for (int i = 0;i < Sqn;++i)
		WriteFile(hWritePipe, SqArr + i, sizeof(double), &dwBytesWrite, NULL);
	WriteFile(hWritePipe, &sSquares, sizeof(double), &dwBytesWrite, NULL);
	SetEvent(hServerEnableRead);
	cin.get();
}
