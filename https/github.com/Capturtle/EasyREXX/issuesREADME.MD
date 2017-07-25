# EasyREXX

EasyREXX is a Notepad++ plugin designed to help REXX programming in Windows environment

Just in one-click, you can
- Optimize your code for zOS environment: Replace TAB to SPACE, EOL to CRLF.
- Save and upload file to remote zOS server by FTP.

How to use:

1) Put EasyREXX.dll in C:\Program Files (x86)\Notepad++\plugins
2) Open Notepad++ and goto Plugins-EasyREXX-Settings

	![settings tutorial](https://user-images.githubusercontent.com/21282908/28587763-4a345608-7146-11e7-9ce2-58f5a7e4d828.png)



3) On the Settings dialogbox, configure your FTP settings and press save/apply

	![settings tutorial2](https://user-images.githubusercontent.com/21282908/28587764-4b8c3f48-7146-11e7-8b5c-0e6c73f080b0.png)



4) Press the green button or F9 to upload your file to server

	![button](https://user-images.githubusercontent.com/21282908/28587765-4ce5b31a-7146-11e7-9f97-d08966e70936.png)



For developers: 

This project is built using the Notepad++ template(https://github.com/npp-plugins/plugindemo)
Here is a simple explanation for source files that I added on the template.

- EasyRexx.cpp: Contains the core functions for this plugin.
		oneClick() function is called when you press the green button.
		
- SettingsDialog.cpp: Contains the callback function(event handler) for the Settings dialog.
			Inherits from the StaticDialog class in the template.
			
- AdvancedSettings.cpp: Contains functions that deal with settings. (save/load, eol conversion, msgbox timeout)

Source files other than the above 3 are from the template.
Notepad++ also supports many commands that can be useful in plugin development:
1) NPPM commands (http://docs.notepad-plus-plus.org/index.php/Messages_And_Notifications)
2) Look up menuCmdID.h for the list of internal notepad++ commands
3) Scintilla commands



