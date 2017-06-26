#include <afxwin.h>
#include <Windows.h>
#include <Windowsx.h>
#include <string>
#include <sstream>
#include "SettingsDialog.h"
#include "resource.h"
#include "../AdvancedSettings.h"


bool SettingsDialog::isCheckBoxOn(int checknum)
{
	return SendDlgItemMessage(_hSelf, checknum, BM_GETCHECK, 0, 0);
}

void SettingsDialog::doDialog()
{
	if (!isCreated())
		create(IDD_DIALOG1);

	goToCenter();
}

INT_PTR CALLBACK SettingsDialog::run_dlgProc(UINT Message, WPARAM wParam, LPARAM /*lParam*/)
{
	switch (Message)
	{
	case WM_INITDIALOG:
	{
		
		SetDlgItemText(_hSelf, MYIDC_EDIT1, hostAddress);
		SetDlgItemText(_hSelf, IDC_EDIT2, ftpId);
		SetDlgItemText(_hSelf, IDC_EDIT3, ftpPw);
		SetDlgItemText(_hSelf, IDC_EDIT4, remotePath);

		return TRUE;
	}
	case WM_COMMAND:
	{		
		switch (wParam)
		{
		case IDAPPLY:			
			GetDlgItemText(_hSelf, MYIDC_EDIT1, hostAddress, 100);
			GetDlgItemText(_hSelf, IDC_EDIT2, ftpId, 100);
			GetDlgItemText(_hSelf, IDC_EDIT3, ftpPw, 100);
			GetDlgItemText(_hSelf, IDC_EDIT4, remotePath, 100);

			if (isCheckBoxOn(IDC_TABCHECK))
				converttab = true;			
			if (isCheckBoxOn(IDC_LFCHECK))
				convertlf = true;	

			MessageBox(NULL, L"Settings applied.", L"EasyREXX", MB_OK| MB_ICONINFORMATION);
			display(FALSE);
			return true;

		case IDSAVE:
			GetDlgItemText(_hSelf, MYIDC_EDIT1, hostAddress, 100);
			GetDlgItemText(_hSelf, IDC_EDIT2, ftpId, 100);
			GetDlgItemText(_hSelf, IDC_EDIT3, ftpPw, 100);
			GetDlgItemText(_hSelf, IDC_EDIT4, remotePath, 100);

			WriteSettings(hostAddress, ftpId, ftpPw, remotePath);
			MessageBox(NULL, L"Settings saved.\nIt will automatically load current settings at start of program.", L"EasyREXX", MB_OK | MB_ICONINFORMATION);
			return true;

		case IDCANCEL:
			display(FALSE);
			return true;

		default:
			break;
		}
	}
	}
	return FALSE;
}
