#include "windows.h"
#include <iostream>

using namespace std;


int main(int argc,char* argv[]) {
	setlocale(LC_ALL, "ru");
	

	wchar_t pipeName[] = L"\\\\LAPTOP-78EUHOP1\\pipe\\demo_pipe";

	HANDLE hNamedPipe;
	hNamedPipe = CreateFile(
		pipeName,			
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_MESSAGE | PIPE_WAIT,
		(LPSECURITY_ATTRIBUTES)NULL,
		OPEN_EXISTING,
		0,
		(HANDLE)NULL
	);
	int n, Sqn = 0;
	double N;
	DWORD dwBytesRead, dwBytesWrite;
	ReadFile(hNamedPipe, &n, sizeof(int), &dwBytesRead, 0);
	ReadFile(hNamedPipe, &N, sizeof(double), &dwBytesRead, 0);
	double* arr = new double[n],
		* SqArr = new double[n];
	for (int i = 0;i < n;++i)
		ReadFile(hNamedPipe, arr + i, sizeof(double), &dwBytesRead, 0);
	for (int i = 0;i < n;++i)
		if (arr[i] > N)
			SqArr[Sqn++] = arr[i];

	cout << "Исходный массив : ";
	for (int i = 0;i < n;++i)
		cout << arr[i] << "   ";
	cout << '\n';


	cout << "Полученный массив : ";
	double sSquares = 0.;
	for (int i = 0;i < Sqn;++i) {
		cout << SqArr[i] << "   ";
		SqArr[i] *= SqArr[i];
		sSquares += SqArr[i];
	}
	cout << "\nСумма квадратов : " << sSquares << '\n';

	WriteFile(hNamedPipe, &Sqn, sizeof(int), &dwBytesWrite, NULL);
	for (int i = 0;i < Sqn;++i)
		WriteFile(hNamedPipe, SqArr + i, sizeof(double), &dwBytesWrite, NULL);
	WriteFile(hNamedPipe, &sSquares, sizeof(double), &dwBytesWrite, NULL);
	CloseHandle(hNamedPipe);
}
