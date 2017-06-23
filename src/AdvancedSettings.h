#pragma once
#include <Windows.h>

bool WriteSettings(WCHAR* addy, WCHAR* id, WCHAR* pw, WCHAR* path);
bool ReadSettings(WCHAR* addy, WCHAR* id, WCHAR* pw, WCHAR* path);
void ConvertTab(HWND);
void ConvertLF(HWND);
HWND getCurrentScintilla(NppData);
bool isAdmin();