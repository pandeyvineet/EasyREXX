//this file is part of notepad++
//Copyright (C)2003 Don HO <donho@altern.org>
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either
//version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#include <Windows.h>
#include <wininet.h>
#include <iostream>
#include <string>
#include <Commctrl.h>
#include "PluginDefinition.h"
#include "menuCmdID.h"
#include "AdvancedSettings.h"
#include ".\DockingFeature\SettingsDialog.h"
#include ".\DockingFeature\StaticDialog.h"
#include ".\DockingFeature\resource.h"

#pragma comment(lib, "wininet")

//
// The plugin data that Notepad++ needs
//
FuncItem funcItem[nbFunc];
toolbarIcons myToolBars[nbFunc];
HINSTANCE ghModule = NULL;
NppData nppData;
SettingsDialog sd;


void pluginInit(HANDLE hModule)
{
	ghModule = (HINSTANCE)hModule;
}

//
// Here you can do the clean up, save the parameters (if any) for the next session
//
void pluginCleanUp()
{
}


void FillToolBar(toolbarIcons* pToolBar, LPCWSTR nResId)
{
	pToolBar->hToolbarIcon = 0;
	pToolBar->hToolbarBmp = LoadBitmap(ghModule, nResId);
	if (NULL == pToolBar->hToolbarBmp)
	{
		DWORD dwError = GetLastError();
		TCHAR sz[MAX_PATH];
		wsprintf(sz, L"%d", dwError);
	}
}

void RegisterToolBar()
{
	FillToolBar(&myToolBars[0], MAKEINTRESOURCE(IDB_BITMAP1));
	::SendMessage(nppData._nppHandle, NPPM_ADDTOOLBARICON, (WPARAM)funcItem[1]._cmdID, (LPARAM)&(myToolBars[0]));	
}

void createDialog()
{
	sd.init((HINSTANCE)ghModule, nppData);
	sd.doDialog();
}

//
// Initialization of your plugin commands
void commandMenuInit()
{	
	ReadSettings(sd.hostAddress, sd.ftpId, sd.ftpPw, sd.remotePath);
	setCommand(0, TEXT("Settings"), createDialog, NULL, false);
	setCommand(1, TEXT("Upload"), saveUpload, NULL, false);
}

//
// Here you can do the clean up (especially for the shortcut)
//
void commandMenuCleanUp()
{
	// Don't forget to deallocate your shortcut here
}


bool setCommand(size_t index, TCHAR *cmdName, PFUNCPLUGINCMD pFunc, ShortcutKey *sk, bool check0nInit) 
{
    if (index >= nbFunc)
        return false;

    if (!pFunc)
        return false;

    lstrcpy(funcItem[index]._itemName, cmdName);
    funcItem[index]._pFunc = pFunc;
    funcItem[index]._init2Check = check0nInit;

    funcItem[index]._pShKey = sk;

    return true;
}

void saveFile(TCHAR* localPath, TCHAR* localName)
{
	SendMessage(nppData._nppHandle, NPPM_GETFULLCURRENTPATH, MAX_PATH, (LPARAM)localPath);
	SendMessage(nppData._nppHandle, NPPM_GETFILENAME, MAX_PATH, (LPARAM)localName);
	SendMessage(nppData._nppHandle, NPPM_SAVECURRENTFILE, 0, 0);
}

std::wstring truncExt(TCHAR* src)
{
	std::wstring wsrc = src;
	size_t lastindex = wsrc.find_last_of('.');
	std::wstring wdes = wsrc.substr(0, lastindex);
	return wdes;
}

void saveUpload()
{
	HINTERNET hInternet;
	HINTERNET hFtpSession;
	TCHAR* localPath = new TCHAR[MAX_PATH]{ 0, };
	TCHAR* localName = new TCHAR[MAX_PATH]{ 0, };

	if (sd.converttab)	//convert TAB to SPACE
		ConvertTab(nppData._nppHandle);
	if (sd.convertlf) //convert any EOL to CRLF
		ConvertLF(getCurrentScintilla(nppData));

	saveFile(localPath, localName);

	hInternet = InternetOpen(NULL, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	hFtpSession = InternetConnect(hInternet, sd.hostAddress, 21, sd.ftpId, sd.ftpPw, INTERNET_SERVICE_FTP, 0, 0);

	if (!hFtpSession)
	{
		MessageBox(NULL, L"Login Error\nCheck your Host Address, ID and PW", L"ERROR", MB_ICONERROR);
		InternetCloseHandle(hFtpSession);
		InternetCloseHandle(hInternet);
		return;
	}

	if (!FtpSetCurrentDirectory(hFtpSession, sd.remotePath))
	{
		MessageBox(NULL, L"Upload Directory Error\nDo not append the master username in front of the upload directory.\n", L"ERROR", MB_ICONERROR);
		InternetCloseHandle(hFtpSession);
		InternetCloseHandle(hInternet);
		return;
	}


	if (!FtpPutFile(hFtpSession, localPath, truncExt(localName).c_str(), FTP_TRANSFER_TYPE_ASCII, 0))
	{
		MessageBox(NULL, L"Couldn't upload log to ftp server!", L"ERROR", MB_ICONERROR);
		InternetCloseHandle(hFtpSession);
		InternetCloseHandle(hInternet);
		return;
	}

	MessageBox(NULL, L"Successfully uploaded log to ftp server!", L"Ftp Upload", MB_ICONINFORMATION|MB_OK);

	InternetCloseHandle(hFtpSession);
	InternetCloseHandle(hInternet);
	return;

}
