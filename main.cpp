#include <windows.h>
#include "worker.h"

#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* argv[])
{   
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    if (argc >= 2 && string(argv[1]) == "child")
    {
        doWork();
        return 0;
    }

    cout << "====================================\n";
    cout << " 멀티스레드 vs 멀티프로세스 비교(수정2)\n";
    cout << "====================================\n\n";

    testMultiThread();

    testMultiProcess(argv[0]);

    return 0;
}