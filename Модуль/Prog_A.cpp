#include <cstdlib>
#include <iostream>
#include "Windows.h"
#include <string>

#define READER 8
#define WRITER 5

using namespace std;

int main(int argc, char* argv[])
{
	SetConsoleOutputCP(1251);

	HANDLE hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 1024, L"MySharedMemory");
	HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, L"Event");
	HANDLE hSemaphoreB = CreateSemaphore(NULL, 8, 8, L"Sem_read");
	HANDLE hSemaphoreC = CreateSemaphore(NULL, 1, 1, L"Sem_write");

	STARTUPINFO stiB = { 0 }, stiC = { 0 };
	PROCESS_INFORMATION piB = { 0 }, piC = { 0 };

	// ���� �� �������
	wstring ApplicationNameB(L"C:\\Users\\sulzl\\source\\repos\\������\\x64\\Debug\\Process_B.exe"),
		    ApplicationNameC(L"C:\\Users\\sulzl\\source\\repos\\������\\x64\\Debug\\Process_C.exe");
	// ��������� ���������
	LPWSTR lpwAppNameB = &ApplicationNameB[0], lpwAppNameC = &ApplicationNameC[0];

	//��������� ����� (B): ��'� �����, �������1,�������2
	wstring CommandLineB(L"MySharedMemory");
	CommandLineB += L" Sem_read";  
	CommandLineB += L" Event";   
	// ��������� ��������� B
	LPWSTR lpwCmdLineB = &CommandLineB[0];

	//��������� ����� (C): ��'� �����, �������1,�������2
	wstring CommandLineC(L"MySharedMemory");
	CommandLineC += L" Sem_write";
	CommandLineC += L" Event";
	// ��������� ��������� C
	LPWSTR lpwCmdLineC = &CommandLineC[0];
 

	//==========================� - ����������==========================

	//cout << "��������� �������� 'C' " << endl;
	for (size_t i = 0; i < WRITER; i++) {
		CreateProcess(lpwAppNameC, lpwCmdLineC, NULL, NULL, TRUE, NULL, NULL, NULL, &stiC, &piC);
	 }

	//==========================B - �����==========================

	//cout << "��������� �������� 'B' " << endl;
	for (size_t i = 0; i < READER; i++){
		CreateProcess(lpwAppNameB, lpwCmdLineB, NULL, NULL, TRUE, NULL, NULL, NULL, &stiB, &piB);
	}
	 
	getchar();
	
	//�������� �������
	CloseHandle(hSemaphoreB);
	CloseHandle(hSemaphoreC);
	CloseHandle(hEvent);
	CloseHandle(hMapFile);

	return 0;
}

