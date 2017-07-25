// SettingsDialog.cpp controls the dialog box that is
// associated with EasyRexx plugin command "Settings".

#include <afxwin.h>
#include <Windows.h>
#include <Windowsx.h>
#include <string>
#include <sstream>
#include "SettingsDialog.h"
#include "../AdvancedSettings.h"
#include "../resource.h"
#include "../menuCmdId.h"


/********************************************************************/
/* Function:  getCheckBoxState                                      */
/*                                                                  */
/* Purpose:   Return true if checkbox is currently checked.         */
/*            Return false otherwise.                               */
/*                                                                  */
/********************************************************************/
bool SettingsDialog::getCheckBoxState(int checknum)
{
	return SendDlgItemMessage(_hSelf, checknum, BM_GETCHECK, 0, 0);
}

/********************************************************************/
/* Function:  setCheckBoxState                                      */
/*                                                                  */
/* Purpose:   Check or uncheck checkbox                             */
/*                                                                  */
/********************************************************************/
bool SettingsDialog::setCheckBoxState(int checknum, bool check)
{
	if(check)
		return SendDlgItemMessage(_hSelf, checknum, BM_SETCHECK, BST_CHECKED, 0);
	else
		return SendDlgItemMessage(_hSelf, checknum, BM_SETCHECK, BST_UNCHECKED, 0);
}

/********************************************************************/
/* Function:  doDialog                                              */
/*                                                                  */
/* Purpose:   Create Settings dialogbox                             */
/*                                                                  */
/********************************************************************/
void SettingsDialog::doDialog()
{
	if (!isCreated())
		create(IDD_DIALOG1);

	goToCenter();
}

/********************************************************************/
/* Function:  SetDialogItems                                        */
/*                                                                  */
/* Purpose:   Set content of all items(textbox, checkbox)           */
/*            in the dialogbox                                      */
/*                                                                  */
/********************************************************************/
void SettingsDialog::SetDialogItems()
{
	SetDlgItemText(_hSelf, HOST_ADDRESS_EDIT, hostAddress);
	SetDlgItemText(_hSelf, ID_EDIT, ftpId);
	SetDlgItemText(_hSelf, PW_EDIT, ftpPw);
	SetDlgItemText(_hSelf, DIRECTORY_EDIT, remotePath);
	setCheckBoxState(BACKUP_CHECK, isBackUpChecked);
	setCheckBoxState(MSGBOX_CHECK, isMsgChecked);
	setCheckBoxState(HOTKEY_CHECK, isHotKeyChecked);
}

/********************************************************************/
/* Function:  GetDialogItems                                        */
/*                                                                  */
/* Purpose:   Retrieve content of all items(textbox, checkbox)      */
/*            in the dialogbox and put them in variables            */
/*                                                                  */
/********************************************************************/
void SettingsDialog::GetDialogItems()
{
	GetDlgItemText(_hSelf, HOST_ADDRESS_EDIT, hostAddress, 100);
	GetDlgItemText(_hSelf, ID_EDIT, ftpId, 100);
	GetDlgItemText(_hSelf, PW_EDIT, ftpPw, 100);
	GetDlgItemText(_hSelf, DIRECTORY_EDIT, remotePath, 100);
	isBackUpChecked = getCheckBoxState(BACKUP_CHECK);
	isMsgChecked = getCheckBoxState(MSGBOX_CHECK);
	isHotKeyChecked = getCheckBoxState(HOTKEY_CHECK);
}

/********************************************************************/
/* Function:  run_dlgProc                                           */
/*                                                                  */
/* Purpose:   Event handler for Settings dialogbox.                 */
/*                                                                  */
/********************************************************************/
INT_PTR CALLBACK SettingsDialog::run_dlgProc(UINT Message, WPARAM wParam, LPARAM /*lParam*/)
{
	//MessageBox(NULL, L"HELLO!", L"ERROR", MB_ICONERROR);
	switch (Message)
	{
	case WM_INITDIALOG:
	{
		
		SetDialogItems();
		return TRUE;
	}
	case WM_COMMAND:
	{		
		switch (wParam)
		{
		// When checkbox is checked
		case MSGBOX_CHECK:
		{
			if (getCheckBoxState(MSGBOX_CHECK))
				isMsgChecked = true;
			else
				isMsgChecked = false;
			return true;
		}
		// When apply button is clicked
		case IDAPPLY:
		{
			GetDialogItems();

			MessageBox(NULL, L"Settings applied.", L"EasyREXX", MB_OK | MB_ICONINFORMATION);
			display(FALSE);
			return true;
		}
		// When save button is clicked
		case IDSAVE:
		{
			GetDialogItems();

			WriteSettings(hostAddress, ftpId, ftpPw, remotePath, isBackUpChecked, isMsgChecked, isHotKeyChecked);
			MessageBox(NULL, L"Settings saved.\nIt will automatically load current settings at start of program.", L"EasyREXX", MB_OK | MB_ICONINFORMATION);
			return true;
		}
		// When dialogbox exits out
		case IDCANCEL:
		{
			display(FALSE);
			return true;
		}

		default:
		{
			break;
		}
			
		}
	}
	}
	return FALSE;
}
