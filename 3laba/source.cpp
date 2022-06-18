#include <windows.h>
#include <iostream>

using namespace std;



CRITICAL_SECTION CSArr, CSSum;
HANDLE SumEEvent;
int k, SumResult;

DWORD WINAPI work(void* p) {
	EnterCriticalSection(&CSArr);
	DWORD count;
	cout << "Enter count : ";
	cin >> count;
	double* arr = (double*)p;

	for (int i = 1;i < k;++i) {
		for (int j = 0;j < k - i;++j)
			if (arr[j] > arr[j + 1]) {
				double r = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = r;
			}
		Sleep(count);
	}
	LeaveCriticalSection(&CSArr);
	return 0;
}

DWORD WINAPI SumElement(void* p) {
	EnterCriticalSection(&CSSum);
	WaitForSingleObject(SumEEvent, INFINITE);
	
	double* arr = (double*)p;
	double res = 0.;
	for (int i = 0;i < k;++i)
		res += arr[i];
	SumResult = res;
	cout << "SumElement result from SumElement : " << res << '\n';
	LeaveCriticalSection(&CSSum);
	return 0;
}

int main() {
	setlocale(LC_ALL, "ru");
	InitializeCriticalSection(&CSArr);
	InitializeCriticalSection(&CSSum);
	SumEEvent = CreateEvent(NULL, FALSE, FALSE, L"SumEEvent");
	HANDLE hWork, hSumE;
	DWORD iWork, iSumE;
	int n;
	cin >> n;
	double* arr = new double[n];
	for (int i = 0;i < n;++i)
		cin >> arr[i];
	
	hWork = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)work, arr, CREATE_SUSPENDED, &iWork);
	cout << "Enter k : ";
	cin >> k;
	ResumeThread(hWork);
	hSumE = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)SumElement, arr, NULL, &iSumE);
	Sleep(50);

	EnterCriticalSection(&CSArr);
	cout << "First k elements : ";
	for (int i = 0;i < k;++i)
		cout << arr[i] << "  ";
	cout << '\n';
	LeaveCriticalSection(&CSArr);
	SetEvent(SumEEvent);

	EnterCriticalSection(&CSSum);
	cout << "SumElement result from Main : " << SumResult << '\n';
	cout << "Print after k-th elem : ";
	for (int i = k;i < n;++i)
		cout << arr[i] << "  ";
	LeaveCriticalSection(&CSSum);

	
	DeleteCriticalSection(&CSArr);
	DeleteCriticalSection(&CSSum);
	CloseHandle(hWork);
	CloseHandle(hSumE);
	CloseHandle(SumEEvent);
}
