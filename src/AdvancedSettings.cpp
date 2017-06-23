#include "PluginDefinition.h"
#include "menuCmdID.h"
#include "Scintilla.h"
#include <fstream>

bool ReadSettings(WCHAR* addy, WCHAR* id, WCHAR* pw, WCHAR* path)
{
	std::wstring userPath = _wgetenv(L"USERPROFILE");
	userPath.append(L"\\EasyREXX.ini");
	std::wifstream in(userPath.c_str());
	in >> addy;
	in >> id; 
	in >> pw;
	in >> path;
	return true;
}

bool WriteSettings(WCHAR* addy, WCHAR* id, WCHAR* pw, WCHAR* path)
{
	std::wstring userPath = _wgetenv(L"USERPROFILE");
	userPath.append(L"\\EasyREXX.ini");
	std::wofstream out(userPath.c_str());
	out << addy << std::endl << id << std::endl << pw << std::endl << path;
	return true;
}

void ConvertTab(HWND handle)
{
	SendMessage(handle, NPPM_MENUCOMMAND, 0, IDM_EDIT_TAB2SW);
}

void ConvertLF(HWND handle)
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