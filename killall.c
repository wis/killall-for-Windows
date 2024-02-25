#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>

int kill_process_by_name(const char *procName) {
    HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);
    PROCESSENTRY32 pEntry;
    pEntry.dwSize = sizeof (pEntry);
    BOOL hRes = Process32First(hSnapShot, &pEntry);
    while (hRes) {
        if (strcmp(pEntry.szExeFile, procName) == 0) {
            HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0, (DWORD) pEntry.th32ProcessID);
            if (hProcess != NULL) {
                TerminateProcess(hProcess, 9);
                CloseHandle(hProcess);
                return 0;
            }
        }
        hRes = Process32Next(hSnapShot, &pEntry);
    }
    CloseHandle(hSnapShot);
    return -1;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s process_name\n", argv[0]);
        return 1;
    }
    int res = kill_process_by_name(argv[1]);
    if (res < 0) {
        printf("Error: process \"%s\" not found.\n", argv[1]);
        return 1;
    }
    printf("Process \"%s\" killed or already not running.\n", argv[1]);
    return 0;
}
