#include <iostream>
#include <windows.h>

using namespace std;

HANDLE Reader[4];
HANDLE Writer[4];

wchar_t const* Wmes[4]{
	L"W_Exit",L"W_A",L"W_B",L"W_C"
};
wchar_t const* Rmes[4]{
	L"R_Exit",L"R_A",L"R_B",L"R_C"
};


int main() {
	setlocale(LC_ALL, "ru");
	for (int i = 0;i < 4;++i) {
		Writer[i] = CreateEvent(NULL, FALSE, FALSE, Wmes[i]);
		Reader[i] = CreateEvent(NULL, FALSE, FALSE, Rmes[i]);
	}
	int n;
	cin >> n;
	HANDLE hMutex = CreateMutex(NULL, FALSE, L"Mutex"),
		hSemaphore = CreateSemaphore(NULL, 1, 1, L"Semaphore");
	STARTUPINFO* Wstp = new STARTUPINFO[n],
		* Rstp = new STARTUPINFO[n];
	PROCESS_INFORMATION* Wpi = new PROCESS_INFORMATION[n],
		* Rpi = new PROCESS_INFORMATION[n];
	for (int i = 0;i < n;++i) {
		ZeroMemory(&Wstp[i], sizeof(STARTUPINFO));
		Wstp[i].cb = sizeof(STARTUPINFO);
		ZeroMemory(&Rstp[i], sizeof(STARTUPINFO));
		Rstp[i].cb = sizeof(STARTUPINFO);
		wstring w = L"Writer.exe",
			r = L"Reader.exe";
		CreateProcess(NULL, &w[0], NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &Wstp[i], &Wpi[i]);
		CreateProcess(NULL, &r[0], NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &Rstp[i], &Rpi[i]);
	}


	for (int i = 0;i < n;++i) {
		while (true) {
			DWORD mes = WaitForMultipleObjects(4, Writer, FALSE, INFINITE);
			SetEvent(Reader[mes]);
			if (mes == WAIT_OBJECT_0)
				break;
			wcout << L"Получено : " << Wmes[mes] + 2 << L'\n';
		}
		cout << "Writer " << i << " завершил работу\n";
		WaitForSingleObject(Reader[0], INFINITE);
		cout << "Reader " << i << " завершил работу\n";
	}

	for (int i = 0;i < 4;++i) {
		CloseHandle(Reader[i]);
		CloseHandle(Writer[i]);
	}
	for (int i = 0;i < n;++i) {
		CloseHandle(Wpi[i].hThread);
		CloseHandle(Wpi[i].hProcess);
		CloseHandle(Rpi[i].hThread);
		CloseHandle(Rpi[i].hProcess);
	}
	CloseHandle(hMutex);
	CloseHandle(hSemaphore);
}
