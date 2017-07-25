#include <Windows.h>
#include <wininet.h>
#include <iostream>
#include <string>
#include <Commctrl.h>
#include <process.h>  
#include "EasyRexx.h"
#include "menuCmdID.h"
#include "AdvancedSettings.h"
#include ".\DockingFeature\SettingsDialog.h"
#include ".\DockingFeature\StaticDialog.h"
#include "resource.h"

#pragma comment(lib, "wininet")

FuncItem funcItem[nbFunc];
toolbarIcons myToolBars[nbFunc];
HINSTANCE ghModule = NULL;
NppData nppData;
static SettingsDialog sd;


/********************************************************************/
/* Function:  pluginInit                                            */
/*                                                                  */
/* Purpose:   Sets the current handle to the given input handle and */
/*            Read the existing ftp user settings.                  */
/*            Called on dll attach (dll load)                       */
/********************************************************************/
void pluginInit(HANDLE hModule)
{
	ghModule = (HINSTANCE)hModule;
	ReadSettings(sd.hostAddress, sd.ftpId, sd.ftpPw, sd.remotePath, sd.isBackUpChecked, sd.isMsgChecked, sd.isHotKeyChecked);
}


/********************************************************************/
/* Function:  pluginCleanUp                                         */
/*                                                                  */
/* Purpose:   Clean up, and save the parameters for next session    */
/*            on dll detach (dll unload)                            */
/********************************************************************/
void pluginCleanUp()
{
}


/********************************************************************/
/* Function:  FillToolBar                                           */
/*                                                                  */
/* Purpose:   Setup your plugin icon in the toolbar.                */
/*                                                                  */
/********************************************************************/
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


/********************************************************************/
/* Function:  RegisterToolBar                                       */
/*                                                                  */
/* Purpose:   Wrapper function for FillToolBar.                     */
/*            Will link your icon to specified command function     */
/*            stored in funcItem.                                   */
/*                                                                  */
/********************************************************************/
void RegisterToolBar()
{
	// Icon bitmap resource, IDB_BITMAP1 is from NppPluginDemo.rc
	FillToolBar(&myToolBars[0], MAKEINTRESOURCE(IDB_BITMAP1));

	// Use nppm command to set icon
	SendMessage(nppData._nppHandle, NPPM_ADDTOOLBARICON, (WPARAM)funcItem[1]._cmdID, (LPARAM)&(myToolBars[0]));	
}


/********************************************************************/
/* Function:  setCommand                                            */
/*                                                                  */
/* Purpose:   Initialization of your plugin commands.               */
/*            Given by the original plugin template by default.     */
/*                                                                  */
/********************************************************************/
void commandMenuInit()
{	
	// Set default hotkey to F9
	ShortcutKey* sk1 = new ShortcutKey();
	sk1->_key = 0x78;

	// Link Settings command to createDialog function	
	setCommand(0, TEXT("Settings"), createDialog, NULL, false);

	// Link Upload command to oneClick function
	setCommand(1, TEXT("Upload"), oneClick, sk1, false);
}


/********************************************************************/
/* Function:  commandMenuCleanUp                                    */
/*                                                                  */
/* Purpose:   Cleanup for your plugin commands/shortcuts.           */
/*            Given by the original plugin template by default.     */
/*                                                                  */
/********************************************************************/
void commandMenuCleanUp()
{
	// Don't forget to deallocate your shortcut here
	
}


/********************************************************************/
/* Function:  setCommand                                            */
/*                                                                  */
/* Purpose:   Matches the command name to specified function.       */
/*            Given by the original plugin template by default.     */
/*                                                                  */
/********************************************************************/
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


/********************************************************************/
/* Function:  saveFile                                              */
/*                                                                  */
/* Purpose:   Saves the current selected file                       */
/*                                                                  */
/* Input:     Path and the name of the file to be saved             */
/*                                                                  */
/* Output:    None                                                  */
/********************************************************************/
void saveFile(TCHAR* localPath, TCHAR* localName)
{
	SendMessage(nppData._nppHandle, NPPM_GETFULLCURRENTPATH, MAX_PATH, (LPARAM)localPath);
	SendMessage(nppData._nppHandle, NPPM_GETFILENAME, MAX_PATH, (LPARAM)localName);
	SendMessage(nppData._nppHandle, NPPM_SAVECURRENTFILE, 0, 0);
}


/********************************************************************/
/* Function:  truncExt                                              */
/*                                                                  */
/* Purpose:   Truncates extension from filename                     */
/*                                                                  */
/* Input:     Filename to truncate extension from                   */
/*                                                                  */
/* Output:    Filename without extension, in wstring format         */
/********************************************************************/
std::wstring truncExt(TCHAR* src)
{
	std::wstring wsrc = src;
	size_t lastindex = wsrc.find_last_of('.');
	std::wstring wdes = wsrc.substr(0, lastindex);
	return wdes;
}


/********************************************************************/
/* Function:  createDialog                                          */
/*                                                                  */
/* Purpose:   Wrapper function that will be called when the         */
/*            Settings command is executed.                         */
/*                                                                  */
/* Input:     None                                                  */
/*                                                                  */
/* Output:    None                                                  */
/********************************************************************/
void createDialog()
{
	// sets handle variables
	sd.init((HINSTANCE)ghModule, nppData);

	// create settings dialog box
	sd.doDialog();
}


/********************************************************************/
/* Function:  oneClick                                              */
/*                                                                  */
/* Purpose:   Wrapper function that will be called when the         */
/*            Upload command is executed.                           */
/*                                                                  */
/* Input:     None                                                  */
/*                                                                  */
/* Output:    None                                                  */
/********************************************************************/
void oneClick()
{
	HINTERNET hInternet;
	HINTERNET hFtpSession;
	TCHAR* localPath = new TCHAR[MAX_PATH]{ 0, };
	TCHAR* localName = new TCHAR[MAX_PATH]{ 0, };

	// convert TAB to SPACE
	ConvertTab(nppData._nppHandle);

	// convert any EOL to CRLF
	ConvertCRLF(getCurrentScintilla(nppData));	

	// save file before upload
	saveFile(localPath, localName); 

	// now upload the saved file 
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
		MessageBox(NULL, L"Couldn't upload file to ftp server!", L"ERROR", MB_ICONERROR);
		InternetCloseHandle(hFtpSession);
		InternetCloseHandle(hInternet);
		return;
	}

	UINT uiFlags = MB_OK | MB_ICONINFORMATION;

	if (sd.isMsgChecked)
	{
		MessageBoxTimeout(NULL, "Sucessfully uploaded file to ftp server.", "Success", uiFlags, 0, 3000);
	}
	else
	{
		MessageBoxA(NULL, "Sucessfully uploaded file to ftp server.", "Success", uiFlags);
	}

	InternetCloseHandle(hFtpSession);
	InternetCloseHandle(hInternet);
	return;

}
