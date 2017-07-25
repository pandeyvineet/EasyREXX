#include <fstream>
#include <process.h>
#include "EasyRexx.h"
#include "DockingFeature\SettingsDialog.h"
#include "AdvancedSettings.h"
#include "menuCmdID.h"
#include "Scintilla.h"

#pragma comment(lib, "User32.lib")


bool isTimerOn = false;
int msgTimer;

bool ReadSettings(WCHAR* addy, WCHAR* id, WCHAR* pw, WCHAR* path, bool& ck1, bool& ck2, bool& ck3)
{
	std::wstring userPath = _wgetenv(L"USERPROFILE");
	userPath.append(L"\\EasyREXX.ini");
	std::wifstream in(userPath.c_str());

	in >> addy;
	in >> id; 
	in >> pw;
	in >> path;
	in >> ck1;
	in >> ck2;
	in >> ck3;

	return true;
}

bool WriteSettings(WCHAR* addy, WCHAR* id, WCHAR* pw, WCHAR* path, bool ck1, bool ck2, bool ck3)
{
	using namespace std;
	wstring userPath = _wgetenv(L"USERPROFILE");
	userPath.append(L"\\EasyREXX.ini");
	wofstream out(userPath.c_str());	
	
	out << addy << endl << id << endl << pw << endl << path << endl << ck1 << endl << ck2 << endl << ck3 << endl;
	
	return true;
}

void ConvertTab(HWND handle)
{
	SendMessage(handle, NPPM_MENUCOMMAND, 0, IDM_EDIT_TAB2SW);
}

void ConvertCRLF(HWND handle)
{
	SendMessage(handle, SCI_CONVERTEOLS, SC_EOL_CRLF, 0);
}

HWND getCurrentScintilla(NppData nppData)
{
	int which = -1;
	::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&which);
	if (which == -1)
		return NULL;
	HWND curScintilla = (which == 0) ? nppData._scintillaMainHandle : nppData._scintillaSecondHandle;

	return curScintilla;
}

unsigned WINAPI msgboxThread(void *)
{
	MessageBox(NULL, L"Successfully uploaded file to ftp server!", L"Ftp Upload", MB_ICONINFORMATION | MB_OK);
	return 0;
}

void CALLBACK f(HWND hwnd, UINT uMsg, UINT timerId, DWORD dwTime)
{
	hwnd = NULL;
	uMsg = NULL;
	timerId = NULL;
	dwTime = NULL;

	HWND msghwnd = FindWindow(NULL, L"Ftp Upload");
	if (msghwnd != NULL)
	{
		SendMessage(msghwnd, WM_CLOSE, NULL, NULL);
		isTimerOn = true;
	}
}

int MessageBoxTimeout(HWND hWnd, LPCSTR lpText,
	LPCSTR lpCaption, UINT uType, WORD wLanguageId,
	DWORD dwMilliseconds)
{
	static MSGBOXAAPI MsgBoxTOA = NULL;

	if (!MsgBoxTOA)
	{
		HMODULE hUser32 = GetModuleHandle(_T("user32.dll"));
		if (hUser32)
		{
			MsgBoxTOA = (MSGBOXAAPI)GetProcAddress(hUser32, "MessageBoxTimeoutA");
			//fall through to 'if (MsgBoxTOA)...'
		}
		else
		{
			//stuff happened, add code to handle it here 
			//(possibly just call MessageBox())
			return 0;
		}
	}

	if (MsgBoxTOA)
	{
		return MsgBoxTOA(hWnd, lpText, lpCaption, uType, wLanguageId, dwMilliseconds);
	}

	return 0;
}
