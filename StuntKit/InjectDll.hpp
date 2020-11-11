#pragma once
#include "windows.h"
#include <tchar.h>

#include <vector>

BOOL WINAPI InjectDlls(__in LPCWSTR targetPath, std::vector<LPCWSTR> libraries);