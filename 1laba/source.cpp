#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <iostream>
#include <process.h>

volatile UINT count;

UINT WINAPI thread(void* m) {
	short* mas = (short*)m;
	for (int i = 0;i < count;++i) {
		if (mas[i] % 9 == 0 && mas[i] < 0)
			std::cout << mas[i] << '\n';
		Sleep(30);
	}
	return 0;
}

void fibonacci() {
	unsigned long long fst = 0, scd = 1;
	std::cout << fst << '\n';

	while (true) {
		Sleep(50);
		std::cout << scd << '\n';
		unsigned long long next = scd + fst;
		fst = scd;
		scd = next;
	}
}


int main() {
	UINT n;
	std::cin >> n;
	count = n;
	
	short* mas = new short[count];

	for (int i = 0;i < count;++i)
		std::cin >> mas[i];

	std::cout << "worker:\n";
	HANDLE worker, Count;
	UINT IDworker;
	DWORD IDCount;
	worker = (HANDLE)_beginthreadex(NULL, 0, thread, mas , 0, &IDworker);
	if (worker == NULL)
		return GetLastError();
	

	SuspendThread(worker);
	std::cout << "worker is stoped\n";
	Sleep(1000);
	std::cout << "worker is started\n";
	ResumeThread(worker);


	WaitForSingleObject(worker, INFINITE);
	
	//////////Доп задача///////////
	
	Count = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)fibonacci, NULL, CREATE_SUSPENDED, &IDCount);
	if (Count == NULL)
		return GetLastError();
	Sleep(1000);
	ResumeThread(Count);
	Sleep(5000);
	SuspendThread(Count);

	CloseHandle(Count);
	CloseHandle(worker);

	return 0; 
}
