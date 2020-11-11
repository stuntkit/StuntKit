// dllmain.cpp : Defines the entry point for the DLL application.
#include <windows.h>
#include <tchar.h>

#include <vector>
#include <sstream>

/* Based on WormKit: https://github.com/CyberShadow/WormKit
*/

std::vector<HMODULE> modules;

void LoadModules();
void UnloadModules();;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        LoadModules();
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        UnloadModules();
        break;
    }
    return TRUE;
}

void LoadModules() {
    bool failed = false;
    WIN32_FIND_DATA moduleFilesData;
    HANDLE moduleFilesHandle = FindFirstFile(_T("sk*.dll"), &moduleFilesData);
    if (moduleFilesHandle == INVALID_HANDLE_VALUE)
    {
        OutputDebugString(_T("[SK] No modules has been found"));
    }
    else
    {
        do {
            std::wstringstream ss;
            ss << "[SK] Trying to load " << moduleFilesData.cFileName;
            OutputDebugString(ss.str().c_str());

            HMODULE m = LoadLibrary(moduleFilesData.cFileName);
            if (m == NULL)
            {
                DWORD err = GetLastError();
                std::wstringstream ss;
                ss << "[SK] Module " << moduleFilesData.cFileName <<" could not be loaded\n";
                ss << "Error: " << err;
                OutputDebugString(ss.str().c_str());
                failed = true;

            }
            else
            {
                std::wstringstream ss;
                ss << "[SK] Module " << moduleFilesData.cFileName << " has been loaded\n";
                OutputDebugString(ss.str().c_str());

                modules.push_back(m);
            }
        } while (FindNextFile(moduleFilesHandle, &moduleFilesData));

        if (failed)
        {
            MessageBox(0, _T("Some modules could not be loaded, check DebugView for more info"), _T("Cannot start the game"), MB_OK | MB_ICONERROR);
        }
    }
}

void UnloadModules() {
    for (auto& module : modules) {
        FreeLibrary(module);
    }
    modules.clear();
}