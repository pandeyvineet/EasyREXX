#include <windows.h>
#include <string>
#include "StaticDialog.h"
#include "../PluginDefinition.h"


class SettingsDialog : public StaticDialog
{

public:
	SettingsDialog() : StaticDialog(), _HSource(nullptr) {};

	void init(HINSTANCE hInst, NppData nppData)
	{
		_nppData = nppData;
		Window::init(hInst, nppData._nppHandle);
	};

	void doDialog();
	bool isCheckBoxOn(int);
	virtual void destroy() {};
	
	WCHAR hostAddress[100] = L"mcevs1.mainz.de.ibm.com";
	WCHAR ftpId[100] = L"abc123";
	WCHAR ftpPw[100] = L"password123";
	WCHAR remotePath[100] = L"abc123.clist";

	bool converttab = false;
	bool convertlf = false;

protected:
	virtual INT_PTR CALLBACK run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam);

private:
	/* Handles */
	NppData			_nppData;
	HWND			_HSource;

	

};

