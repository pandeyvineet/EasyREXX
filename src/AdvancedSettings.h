#ifndef ADVANCEDSETTINGS_H
#define ADVANCEDSETTINGS_H

#include <Windows.h>

#include "DockingFeature\SettingsDialog.h"



bool WriteSettings(WCHAR* addy, WCHAR* id, WCHAR* pw, WCHAR* path, bool, bool, bool);
bool ReadSettings(WCHAR* addy, WCHAR* id, WCHAR* pw, WCHAR* path, bool&, bool&, bool&);
void ConvertTab(HWND);
void ConvertCRLF(HWND);
HWND getCurrentScintilla(NppData);

unsigned WINAPI msgboxThread(void*);
void CALLBACK f(HWND hwnd, UINT uMsg, UINT timerId, DWORD dwTime);
void successMessageBox(SettingsDialog, std::string);

typedef int(__stdcall *MSGBOXAAPI)(IN HWND hWnd,
	IN LPCSTR lpText, IN LPCSTR lpCaption,
	IN UINT uType, IN WORD wLanguageId, IN DWORD dwMilliseconds);

int MessageBoxTimeout(HWND hWnd, LPCSTR lpText,
	LPCSTR lpCaption, UINT uType, WORD wLanguageId,
	DWORD dwMilliseconds);


#endif