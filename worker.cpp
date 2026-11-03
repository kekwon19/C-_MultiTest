#include "worker.h"

#include <windows.h>

#include <atomic>
#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

using namespace std;
using namespace chrono;

const int WORKER_COUNT = 10;
const int WORK_COUNT = 50'000'000;

atomic<int> shared_counter = 0;

void doWork()
{
    volatile long long sum = 0;

    for (int i = 0; i < WORK_COUNT; i++)
    {
        sum += i;
    }

    shared_counter++;
}

void testMultiThread()
{
    shared_counter = 0;

    vector<thread> threads;

    auto startTime = high_resolution_clock::now();

    for (int i = 0; i < WORKER_COUNT; i++)
    {
        threads.emplace_back(doWork);
    }

    for (thread& worker : threads)
    {
        worker.join();
    }

    auto endTime = high_resolution_clock::now();

    auto elapsedTime =
        duration_cast<milliseconds>(endTime - startTime).count();

    cout << "========== 멀티스레드 ==========\n";
    cout << "스레드 수: " << WORKER_COUNT << '\n';
    cout << "shared_counter: "
         << shared_counter.load() << '\n';
    cout << "실행시간: "
         << elapsedTime << " ms\n\n";
}

void testMultiProcess(const string& executablePath)
{
    shared_counter = 0;

    vector<HANDLE> processHandles;

    auto startTime = high_resolution_clock::now();

    for (int i = 0; i < WORKER_COUNT; i++)
    {
        string command =
            "\"" + executablePath + "\" child";

        vector<char> commandBuffer(
            command.begin(),
            command.end()
        );

        commandBuffer.push_back('\0');

        STARTUPINFOA startupInfo{};
        PROCESS_INFORMATION processInfo{};

        startupInfo.cb = sizeof(startupInfo);

        BOOL success = CreateProcessA(
            nullptr,
            commandBuffer.data(),
            nullptr,
            nullptr,
            FALSE,
            CREATE_NO_WINDOW,
            nullptr,
            nullptr,
            &startupInfo,
            &processInfo
        );

        if (!success)
        {
            cerr << "프로세스 생성 실패: "
                 << GetLastError() << '\n';
            continue;
        }

        processHandles.push_back(
            processInfo.hProcess
        );

        CloseHandle(processInfo.hThread);
    }

    for (HANDLE processHandle : processHandles)
    {
        WaitForSingleObject(
            processHandle,
            INFINITE
        );

        CloseHandle(processHandle);
    }

    auto endTime = high_resolution_clock::now();

    auto elapsedTime =
        duration_cast<milliseconds>(endTime - startTime).count();

    cout << "========== 멀티프로세스 ==========\n";
    cout << "프로세스 수: "
         << processHandles.size() << '\n';
    cout << "부모 shared_counter: "
         << shared_counter.load() << '\n';
    cout << "실행시간: "
         << elapsedTime << " ms\n\n";
}