#include <iostream>
#include <windows.h>

using namespace std;
wchar_t const* mes[4]{
	L"R_Exit",L"R_A",L"R_B",L"R_C"
};

int main() {
	HANDLE hSemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, L"Semaphore");
	WaitForSingleObject(hSemaphore, INFINITE);
	HANDLE mesEv[4];
	for (int i = 0;i < 4;++i)
		mesEv[i] = OpenEvent(EVENT_ALL_ACCESS, FALSE, mes[i]);
	while (true) {
		DWORD m = WaitForMultipleObjects(4, mesEv, FALSE, INFINITE);
		if (m == WAIT_OBJECT_0)
			break;
		wcout << mes[m] + 2 << L'\n';
	}
	SetEvent(mesEv[0]);
	ReleaseSemaphore(hSemaphore, 1, NULL);
}
