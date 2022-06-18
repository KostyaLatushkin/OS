#include "windows.h"
#include <iostream>

using namespace std;


int main() {
	setlocale(LC_ALL, "ru");
	cout << "Введите размер массива и элементы массива : ";
	int n;
	cin >> n;
	double* arr = new double[n], N;
	for (int i = 0;i < n;++i)
		cin >> arr[i];
	cout << "Введите число N : ";
	cin >> N;


	HANDLE hSumEnableRead, hServerEnableRead;
	hSumEnableRead = CreateEvent(NULL, FALSE, FALSE, L"SumRead");
	hServerEnableRead = CreateEvent(NULL, FALSE, FALSE, L"ServerRead");

	
	HANDLE hWritePipe, hReadPipe;
	SECURITY_ATTRIBUTES sa;

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	CreatePipe(&hReadPipe, &hWritePipe, &sa, 0);

	STARTUPINFO stp;
	PROCESS_INFORMATION pi;
	ZeroMemory(&stp, sizeof(STARTUPINFO));
	stp.cb = sizeof(STARTUPINFO);
	wchar_t commandLine[80];
	wsprintf(commandLine, L"Sum.exe %d %d", (int)hWritePipe, (int)hReadPipe);
	CreateProcess(NULL, commandLine, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &stp, &pi);


	DWORD dwBytesWritten, dwBytesRead;
	WriteFile(hWritePipe, &n, sizeof(int), &dwBytesWritten, NULL);
	WriteFile(hWritePipe, &N, sizeof(double), &dwBytesWritten, NULL);
	for (int i = 0;i < n;++i)
		WriteFile(hWritePipe, arr + i, sizeof(double), &dwBytesWritten, NULL);
	SetEvent(hSumEnableRead);


	WaitForSingleObject(hServerEnableRead, INFINITE);
	int nNew;
	ReadFile(hReadPipe, &nNew, sizeof(int), &dwBytesRead, NULL);
	double* SqArr = new double[nNew], sum;
	for (int i = 0;i < nNew;++i)
		ReadFile(hReadPipe, SqArr + i, sizeof(double), &dwBytesRead, NULL);
	ReadFile(hReadPipe, &sum, sizeof(double), &dwBytesRead, NULL);

	cout << "Полученный массив : ";
	for (int i = 0;i < nNew;++i)
		cout << SqArr[i] << "   ";
	cout << "\nСумма : " << sum << '\n';



	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(hServerEnableRead);
	CloseHandle(hSumEnableRead);
	CloseHandle(hReadPipe);
	CloseHandle(hWritePipe);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}
