//this file is part of notepad++
//Copyright (C)2003 Don HO ( donho@altern.org )
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

#include <atlbase.h>
#include <atlwin.h>
#include <windows.h>



#include "BrowserDlg.h"
#include "PluginDefinition.h"
CComModule _Module;
//#pragma comment(lib,"atls")
#pragma comment(lib,"User32.lib")

extern NppData nppData;

RECT rc;
IWebBrowser2* iWebBrowser;
VARIANT varMyURL;
static CAxWindow WinContainer;
LPCOLESTR pszName=OLESTR("shell.Explorer.2");//OLESTR("Microsoft.IExplorer.4");
static TCHAR szAppName[]=TEXT("WebBrowser");
static TCHAR szClassName[]=TEXT("WebBrowser");
void CBrowserDlg::setParent(HWND parent2set){
	_hParent = parent2set;
};

CBrowserDlg::CBrowserDlg() :DockingDlgInterface(IDD_PLUGINWebBrowser)
{
	lstrcpy(URLString,_T("about:blank"));
};
CBrowserDlg::~CBrowserDlg()
{
}
void CBrowserDlg::GotoURL()
{
	if(!this->isVisible())return;
	//::MessageBox(NULL,URLString,URLString,MB_OK);
	VariantInit(&varMyURL);
	varMyURL.vt = VT_BSTR;
	USES_CONVERSION;
	varMyURL.bstrVal = SysAllocString(T2W(URLString)); // http://www.baidu.com
	SetDlgItemText(_hSelf,ID_WebBrowser_EDIT,URLString);
	iWebBrowser-> Navigate2(&varMyURL,0,0,0,0);
	VariantClear(&varMyURL);
	WinContainer.SetFocus();
}
INT_PTR CALLBACK CBrowserDlg::run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) 
	{
		case WM_SIZE:
		case WM_MOVE:
		{
			RECT rc;
			getClientRect(rc);
			int TopHeight = 24;
			WinContainer.MoveWindow(rc.top,rc.left+TopHeight,rc.right,rc.bottom-TopHeight, TRUE);
			::MoveWindow(_hEDIT,rc.left+50, rc.top, rc.right-150,20,TRUE);
			::MoveWindow(_hBUTTON,rc.right-100 , rc.top, 50, 20,TRUE);
			::MoveWindow(_hClear,rc.right-40 , rc.top, 40, 20,TRUE);
		}
		break;
		case WM_DESTROY:
		{
			iWebBrowser->Release();
			WinContainer.DestroyWindow();
			CoUninitialize();
		}
		break;
		case WM_INITDIALOG:
			{
				_hEDIT=	GetDlgItem(_hSelf, ID_WebBrowser_EDIT);
				_hBUTTON =	GetDlgItem(_hSelf, IDOK);
				_hClear = GetDlgItem(_hSelf, IDC_BUTTON1); 

				CoInitialize(NULL);
				AtlAxWinInit();
				IID   iid;
				IIDFromString(OLESTR("{EAB22AC0-30C1-11CF-A7EB-0000C05BAE0B}"),   &iid);
				HRESULT hRes   =   _Module.Init(NULL, this->getHinst(),   &iid); 
				rc.top = 24;
				rc.left = 0;
				rc.bottom = 250;
				rc.right = 500;
				HRESULT res ;
				WinContainer.Create(this->getHSelf(), rc, 0,WS_CHILD |WS_VISIBLE);
				res = WinContainer.CreateControl(pszName);
				res = WinContainer.QueryControl(__uuidof(IWebBrowser2),(void**)&iWebBrowser); 
			}
		break;
		case WM_COMMAND : 
		{
			switch (wParam)
			{
				case MAKELONG(IDC_BUTTON1,BN_CLICKED):
				{
					lstrcpy(URLString,TEXT("about:blank"));
					GotoURL();
				}break;
				case IDOK :
				{
					GetDlgItemText(_hSelf,ID_WebBrowser_EDIT,URLString,1024);
					GotoURL();
					return TRUE;
				}
			}
				return FALSE;
		}

		default :
			return DockingDlgInterface::run_dlgProc(message, wParam, lParam);
	}
	return FALSE;
}

