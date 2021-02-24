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

#ifndef PLUGINDEFINITION_H
#define PLUGINDEFINITION_H
//
// All difinitions of plugin interface
//
#include "PluginInterface.h"
#include <string>
#include <fstream>
#include <atlbase.h>
#include <vector>
#include "menuCmdID.h"
struct TempFile 
{
	TCHAR FileName[_MAX_PATH];
};
using   namespace   std;
#define TemplateMaxSize 4096
#define TemplateMaxCount 20
#define TemplateMinCount 2
//locale loc = locale::global(locale("")); //要打开的文件路径含中文，设置全局locale为本地环境
//locale::global(loc);//恢复全局locale
#ifdef UNICODE 
#define FSConvert(x) x
#else
#define FSConvert(x) locale loc=locale::global(locale(""));x;locale::global(loc);
#endif
//-------------------------------------//
//-- STEP 1. DEFINE YOUR PLUGIN NAME --//
//-------------------------------------//
// Here define your plugin name
//
const TCHAR NPP_PLUGIN_NAME[] = TEXT("Newfile&&Browser");

//-----------------------------------------------//
//-- STEP 2. DEFINE YOUR PLUGIN COMMAND NUMBER --//
//-----------------------------------------------//
//
// Here define the number of your plugin commands
//

// Initialization of your plugin data
// It will be called while plugin loading
//
void pluginInit(HANDLE hModule);

//
// Cleaning of your plugin
// It will be called while plugin unloading
//
void pluginCleanUp();

//
//Initialization of your plugin commands
//
void commandMenuInit();

//
//Clean up your plugin commands allocation (if any)
//
void commandMenuCleanUp();

//
// Function which sets your command 
//
bool setCommand(size_t index, TCHAR *cmdName, PFUNCPLUGINCMD pFunc, ShortcutKey *sk = NULL, bool check0nInit = false);


//
// Your plugin command functions
//
enum{
	ENUM_ID_CreateNewFile = 0,
	ENUM_ID_ShowBrowser,
	ENUM_ID_OnlyShowBrowser,
	ENUM_ID_SPLIT,
	ENUM_ID_SetConfig,
	ENUM_ID_CheckUpdate,
	ENUM_ID_AboutThis,
	ENUM_ID_CountNum,
};
extern int 
	ID_CreateNewFile,
	ID_ShowBrowser,
	ID_OnlyShowBrowser,
	ID_SPLIT,
	ID_SetConfig,
	ID_CheckUpdate,
	ID_AboutThis,
	ID_CountNum;
struct FileInfo 
{
	TCHAR FileName[_MAX_PATH];
	int FileType;
};
void CreateNewFile(int);
extern bool DoBrowser;
extern FileInfo NewFileType[TemplateMaxCount];
extern int TemplateCount;
extern int NewFile1Type;
extern int NewFile2Type;
extern bool AutoShow;
void ShowBrowser();
void OnlyShowBrowser();
void SetConfig();
void checkUpdate();
void AboutThis();

extern toolbarIcons		g_TBIE;

#endif //PLUGINDEFINITION_H
