// StuntKit.cpp : Defines the entry point for the application.
//

#include "StuntKit.hpp"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    std::vector<LPCWSTR> libraries = { L"StuntLib.dll", L"HookLib.dll" };
    for (const auto& library : libraries) {
        if (!std::filesystem::exists(library)) {
            std::wstringstream errorMessage;
            errorMessage << "StuntKit couldn't find \"" << library << "\" library.";

            MessageBox(NULL, errorMessage.str().c_str(), _T("Unable to load StuntKit"), MB_OK | MB_ICONERROR);
            return EXIT_FAILURE;
        }
    }
    if (!InjectDlls(EXE_NAME, libraries))
    {
        MessageBox(NULL, _T("There was a problem while loading StuntKit."), _T("Unable to load StuntKit"), MB_OK | MB_ICONERROR);
        OutputDebugString(_T("Could not load StuntKit"));
        return EXIT_FAILURE;
    }
    return 0;
}