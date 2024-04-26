#include<iostream>
#include"windows.h"
#include<string>
#include <random>

using namespace std;
 
//����� "����������" 
void Writer(HANDLE& hSemaphore, HANDLE &hEvent, int* pIntValue)
{
    WaitForSingleObject(hSemaphore, INFINITE);
    SetEvent(hEvent);
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    cout << "=====������ ����������=====" << endl;

    for (size_t i = 0; i < 10; i++)
    {
        pIntValue[i] = std::rand();
    }

    ResetEvent(hEvent);
    ReleaseSemaphore(hSemaphore, 1, NULL);
}

int main(int argc, char* argv[])
{
    // 0 - �������� ����� �����
    // 1 - �������� ����� ��������
    // 2 - �������� ��䳿 
    SetConsoleOutputCP(1251);

    std::wstring _event = std::wstring(argv[2], argv[2] + strlen(argv[2]));
    std::wstring semaphoreName = std::wstring(argv[1], argv[1] + strlen(argv[1]));
    std::wstring filemappingName = std::wstring(argv[0], argv[0] + strlen(argv[0]));

    //�������� ��䳿
    HANDLE hEvent = OpenEvent(EVENT_MODIFY_STATE, FALSE, _event.c_str());
    if (hEvent == NULL) { cout << "������� �������� ��䳿 �����������" << endl; }

    //�������� ��������
    HANDLE hSemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, semaphoreName.c_str());
    if (hSemaphore == NULL) { cout << "������� �������� �������� �����������" << endl; }

    //�������� �����
    HANDLE hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, filemappingName.c_str());
    if (hMapFile == NULL) { cout << "������� �������� ����� �����������" << endl; }

    //��������� ���� int
    LPVOID pData = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 0);
    int* pIntValue = static_cast<int*>(pData);

    // ������ �����������
    Writer(hSemaphore, hEvent, pIntValue);

    // �������� �������
    CloseHandle(hSemaphore);
    CloseHandle(hMapFile);
    CloseHandle(hEvent);
    UnmapViewOfFile(pData);

    return 0;
}