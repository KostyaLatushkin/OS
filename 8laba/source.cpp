#include <iostream>
#include <Windows.h>
#include <string>
#include <sstream>

using std::cin;
using std::cout;
using std::string;


DWORD readCount = 0;
HANDLE access = CreateSemaphore(NULL, 0, 1, NULL), 
		readSum = CreateSemaphore(NULL, 0, 1, NULL);
string text = "text";
stringstream out;
struct MyStruct
{
	int const p;
};

DWORD WINAPI writer(void*p) {
	WaitForSingleObject(access, INFINITY);

	text = string("text") + to_string(((MyStruct*)p)->p);
	out << "Текст изменен на " << text << " \n";
	Sleep(20);

	ReleaseSemaphore(access, 1, NULL);
	return 0;
}
DWORD WINAPI reader(void*) {
	WaitForSingleObject(readSum, INFINITY);
	++readCount;
	if (readCount == 1)
		WaitForSingleObject(access, INFINITY);
	ReleaseSemaphore(readSum, 1, NULL);

	out << "Прочитан текст " << text << "\n";
	Sleep(20);

	WaitForSingleObject(readSum, INFINITY);
	--readCount;
	if (readCount == 0)
		ReleaseSemaphore(access, 1, NULL);
	ReleaseSemaphore(readSum, 1, NULL);
	return 0;
}




int main() {
	
	setlocale(LC_ALL, "ru");

	cout << "Введите количество писателей и читателей:\n";
	int nwriter, nreader;
	cin >> nwriter >> nreader;
	HANDLE* hwriter = new HANDLE[nwriter],
		* hreader = new HANDLE[nreader];
	DWORD* iwriter = new DWORD[nwriter],
		* ireader = new DWORD[nreader];


	for (int i = 0;i < nwriter / 2;++i)
		hwriter[i] = CreateThread(NULL, NULL, writer, new MyStruct{ i }, 0, &iwriter[i]);
	for (int i = 0;i < nreader;++i)
		hreader[i] = CreateThread(NULL, NULL, reader, NULL, 0, &ireader[i]);
	for (int i = nwriter / 2;i < nwriter;++i)
		hwriter[i] = CreateThread(NULL, NULL, writer, new MyStruct{ i }, 0, &iwriter[i]);
	
	
	WaitForMultipleObjects(nwriter, hwriter, TRUE, INFINITE);
	WaitForMultipleObjects(nreader, hreader, TRUE, INFINITE);
	cout << out.str();
	for (int i = 0;i < nreader;++i)
		CloseHandle(hreader[i]);
	for (int i = 0;i < nwriter;++i)
		CloseHandle(hwriter[i]);
  	CloseHandle(access);
	CloseHandle(readSum);
}
