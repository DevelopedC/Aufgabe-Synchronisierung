#include<iostream>
#include"windows.h"
#include<string>
#include<mutex>
#include<vector>
#include <chrono>

using namespace std;

//Поток "Читач" 
void Reader(HANDLE &hSemaphore, HANDLE &hEvent, int* pIntValue)
{
    HANDLE h[]{ hSemaphore ,hEvent };
    WaitForMultipleObjects(2, h, FALSE, INFINITE);
 
    cout << "=====Працює читач=====" << endl;   

    for (size_t i = 0; i < 10; i++)
    {
        cout<<" читач: " << pIntValue[i] << endl;
    }

    ReleaseSemaphore(hSemaphore, 1, NULL);
}

int main(int argc, char* argv[])
{
    // 0 - параметр назва файлу
    // 1 - параметр назва семафору
    // 2 - параметр події 
    SetConsoleOutputCP(1251);

    std::wstring _event = std::wstring(argv[2], argv[2] + strlen(argv[2]));
    std::wstring semaphoreName = std::wstring(argv[1], argv[1] + strlen(argv[1]));
    std::wstring filemappingName = std::wstring(argv[0], argv[0] + strlen(argv[0]));

    HANDLE hEvent = OpenEvent(EVENT_MODIFY_STATE, FALSE, _event.c_str());
    if (hEvent == NULL) { cout << "помилка відкриття події читача" << endl; }
 
    HANDLE hSemaphore = OpenSemaphore(SYNCHRONIZE, FALSE, semaphoreName.c_str());
    if (hSemaphore == NULL) { cout << "помилка відкриття семафору читача" << endl; }

    HANDLE hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, filemappingName.c_str());
    if (hMapFile == NULL) { cout << "помилка відкриття файлу читача" << endl; }

    LPVOID pData = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 0);
    int* pIntValue = static_cast<int*>(pData);

    // виклик читача
     Reader(hSemaphore, hEvent, pIntValue);
  
    // закриття ресурсів
    CloseHandle(hSemaphore);
    CloseHandle(hMapFile);
    CloseHandle(hEvent);
    UnmapViewOfFile(pData);

    return 0;
}