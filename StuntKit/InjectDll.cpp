#include "injectDll.hpp"



BOOL WINAPI InjectDlls(__in LPCWSTR targetPath, std::vector<LPCWSTR> libraries) {
    SIZE_T nLength;
    LPVOID lpLoadLibraryW = NULL;
    bool failed = false;

    std::vector<LPVOID> lpRemoteStrings;
    STARTUPINFO             startupInfo;
    PROCESS_INFORMATION     processInformation;

    memset(&startupInfo, 0, sizeof(startupInfo));
    startupInfo.cb = sizeof(STARTUPINFO);

    if (!CreateProcess(targetPath, NULL, NULL, NULL, FALSE,
        CREATE_SUSPENDED, NULL, NULL, &startupInfo, &processInformation))
    {
        OutputDebugString(TEXT("Target process is failed to start"));
        return FALSE;
    }

    lpLoadLibraryW = GetProcAddress(GetModuleHandle(L"KERNEL32.DLL"), "LoadLibraryW");

    if (!lpLoadLibraryW)
    {
        OutputDebugString(TEXT("GetProcAddress failed"));
        // close process handle
        CloseHandle(processInformation.hProcess);
        return FALSE;
    }

    for (const auto& lpcwszDll : libraries) {
        nLength = wcslen(lpcwszDll) * sizeof(WCHAR);

        // allocate mem for dll name
        LPVOID lpRemoteString = VirtualAllocEx(processInformation.hProcess, NULL, nLength + 1, MEM_COMMIT, PAGE_READWRITE);
        lpRemoteStrings.push_back(lpRemoteString);

        if (!lpRemoteString)
        {
            OutputDebugString(TEXT("VirtualAllocEx failed"));
            // close process handle
            CloseHandle(processInformation.hProcess);
            return FALSE;
        }
        // write dll name
        if (!WriteProcessMemory(processInformation.hProcess, lpRemoteString, lpcwszDll, nLength, NULL)) {
            OutputDebugString(TEXT("WriteProcessMemory failed"));
            // free allocated memory
            VirtualFreeEx(processInformation.hProcess, lpRemoteString, 0, MEM_RELEASE);
            // close process handle
            CloseHandle(processInformation.hProcess);
            return FALSE;
        }

        // call loadlibraryw
        HANDLE hThread = CreateRemoteThread(processInformation.hProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)lpLoadLibraryW, lpRemoteString, NULL, NULL);
        if (!hThread) {
            OutputDebugString(_T("CreateRemoteThread failed"));
            failed = true;
        }
        WaitForSingleObject(hThread, 4000);
    }

    if (failed) {
        TerminateProcess(processInformation.hProcess, -3);
        return FALSE;
    }
    //resume suspended process
    ResumeThread(processInformation.hThread);

    //  free allocated memory
    for (auto lpRemoteString : lpRemoteStrings) {
        VirtualFreeEx(processInformation.hProcess, lpRemoteString, 0, MEM_RELEASE);
    }
    // close process handle
    CloseHandle(processInformation.hProcess);
    return TRUE;
}