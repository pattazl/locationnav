//this file is part of notepad++
//Copyright (C)2011 Austin Young <pattazl@gmail.com>
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

#include "PluginDefinition.h"
#include <shlwapi.h>
#include "BrowserDlg.h"

extern vector<TempFile> ArrTempfile;
extern FuncItem funcItem[ENUM_ID_CountNum+TemplateMaxCount];//[ID_CountNum];
extern NppData nppData;
extern bool doCloseTag;

extern HANDLE				g_hModule;
extern CBrowserDlg _showBrowser;

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  reasonForCall, 
                       LPVOID lpReserved )
{
    switch (reasonForCall)
    {
      case DLL_PROCESS_ATTACH:
        pluginInit(hModule);
        break;

      case DLL_PROCESS_DETACH:
        pluginCleanUp();
        break;

      case DLL_THREAD_ATTACH:
        break;

      case DLL_THREAD_DETACH:
        break;
    }

    return TRUE;
}


extern "C" __declspec(dllexport) void setInfo(NppData notpadPlusData)
{
	nppData = notpadPlusData;
	commandMenuInit();
}

extern "C" __declspec(dllexport) const TCHAR * getName()
{
	return NPP_PLUGIN_NAME;
}

extern "C" __declspec(dllexport) FuncItem * getFuncsArray(int *nbF)
{
	*nbF = ID_CountNum;
	return funcItem;
}

extern "C" __declspec(dllexport) void beNotified(SCNotification *notifyCode)
{
	if ((notifyCode->nmhdr.hwndFrom == nppData._nppHandle) && 
		(notifyCode->nmhdr.code == NPPN_TBMODIFICATION))
	{
		/* add toolbar icon */
		g_TBIE.hToolbarBmp = (HBITMAP)::LoadImage((HINSTANCE)g_hModule, MAKEINTRESOURCE(IDB_BITMAP1), IMAGE_BITMAP, 0,0, (LR_DEFAULTSIZE | LR_LOADMAP3DCOLORS));
		::SendMessage(nppData._nppHandle, NPPM_ADDTOOLBARICON, (WPARAM)funcItem[ID_ShowBrowser]._cmdID, (LPARAM)&g_TBIE);

	}
	switch (notifyCode->nmhdr.code) 
	{
		case NPPN_SHUTDOWN:
		{
			commandMenuCleanUp();
		}
		break;
		case NPPN_FILESAVED:
		{
			// 如果文件名在 临时队列中，需要提醒
			TCHAR path[MAX_PATH];
			::SendMessage(nppData._nppHandle, NPPM_GETFULLCURRENTPATH , 0, (LPARAM)path);
			for (int i=0;i<ArrTempfile.size();i++)
			{
				// && !HaveDoingSaved
				if ( lstrcmp( ArrTempfile[i].FileName,path ) == 0 && !DoBrowser )
				{
					::MessageBox(nppData._nppHandle,TEXT("File is saved to temporary path, it will be removed when exit, pls save as to another!"),TEXT("Warning"),MB_OK);
					break;
					//::SendMessage(nppData._nppHandle, NPPM_MENUCOMMAND, 0, IDM_FILE_SAVEAS);
					//::SendMessage(nppData._nppHandle,NPPM_SAVECURRENTFILEAS  , 0, (LPARAM)p);
				}
			}
			// 如果路径相同并且不是主动刷新的，那么直接刷新
			if( lstrcmp(path,_showBrowser.URLString) == 0 && !DoBrowser && AutoShow )
			{
				_showBrowser.GotoURL();
			}
			
		}
		break;
		case NPPN_FILEBEFORESAVE:
		{
			

		}
		break;
		case SCN_CHARADDED:
		{


		}
		break;

		default:
			return;
	}
}


// Here you can process the Npp Messages 
// I will make the messages accessible little by little, according to the need of plugin development.
// Please let me know if you need to access to some messages :
// http://sourceforge.net/forum/forum.php?forum_id=482781
//
extern "C" __declspec(dllexport) LRESULT messageProc(UINT Message, WPARAM wParam, LPARAM lParam)
{/*
	if (Message == WM_MOVE)
	{
		::MessageBox(NULL, "move", "", MB_OK);
	}
*/
	return TRUE;
}

#ifdef UNICODE
extern "C" __declspec(dllexport) BOOL isUnicode()
{
    return TRUE;
}
#endif //UNICODE
