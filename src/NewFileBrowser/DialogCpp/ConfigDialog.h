/*
This file is part of Explorer Plugin for Notepad++
Copyright (C)2006 Jens Lorenz <jens.plugin.npp@gmx.de>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/


#ifndef CONFIG_DEFINE_H
#define CONFIG_DEFINE_H

#include "StaticDialog2.h"
#include "resource.h"
#include "PluginDefinition.h"

const TCHAR strTemplate1[] = TEXT("template1.dat");
const TCHAR strTemplate2[] = TEXT("template2.dat");
const TCHAR strTemplate[] = TEXT("template%d.dat");
extern TCHAR Template1FilePath[MAX_PATH];
extern TCHAR Template2FilePath[MAX_PATH];
extern TCHAR TemplateFilePath[MAX_PATH];

extern NppData nppData;

class ConfigDialog : public StaticDialog2
{

public:
	ConfigDialog() : StaticDialog2() {};
    
    void init(HINSTANCE hInst, NppData nppData)
	{
		_nppData = nppData;
		Window::init(hInst, nppData._nppHandle);
	};

   	void doDialog();

    virtual void destroy() {
		StaticDialog2::destroy();
		_hSelf = NULL;
    };
	FileInfo tempNewFileType[TemplateMaxCount];
	void ReadConfigFile(LangType langtype,int fileTypeId);
	int preNoSelectID;
	void GetFileNoByCount();
	void GetFileTypeByNo();
	void SaveConfig2Arr();

protected :
	virtual BOOL CALLBACK run_dlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	/* Handles */
	NppData			_nppData;
    HWND			_HSource,_hListType,_hListNo,_hCkBox,_hListCount, _hEditTxt;
	
};



#endif // CONFIG_DEFINE_H
