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
#include "menuCmdID.h"
#include "ConfigDialog.h"
//
// put the headers you need here
//
#include <shlwapi.h>
#include "BrowserDlg.h"

#ifdef _UNICODE
typedef wstring tstring;
#else
typedef string tstring;
#endif

extern tstring LangTypeStr[];
const TCHAR sectionName[] = TEXT("Setting");
const TCHAR StrNewFileType[] = TEXT("NewFile%dType");
const TCHAR StrNewFileName[] = TEXT("NewFile%dName");
const TCHAR StrNewFile1Type[] = TEXT("NewFile1Type");
const TCHAR StrNewFile2Type[] = TEXT("NewFile2Type");

const TCHAR StrTemplateCount[] = TEXT("TemplateCount");
const TCHAR StrAutoShow[] = TEXT("AutoShow");

const TCHAR configFileName[] = TEXT("NewfileBrowser.ini");
extern TCHAR JAVADefault[512];
extern TCHAR HTMLDefault[512];
CBrowserDlg _showBrowser;
ConfigDialog dlgConfig;

vector<TempFile> ArrTempfile;
TCHAR   szTempPath[_MAX_PATH]={0},szOriTempfile[_MAX_PATH]={0},szTempfile[_MAX_PATH]={0}; 

#ifdef UNICODE 
	#define generic_itoa _itow
#else
	#define generic_itoa itoa
#endif


FuncItem funcItem[ENUM_ID_CountNum+TemplateMaxCount];

//
// The data of Notepad++ that you can use in your plugin commands
//
NppData nppData;
HANDLE g_hModule;

toolbarIcons		g_TBIE;

TCHAR iniFilePath[MAX_PATH];
TCHAR Template1FilePath[MAX_PATH];
TCHAR Template2FilePath[MAX_PATH];

TCHAR TemplateFilePath[MAX_PATH];

FileInfo NewFileType[TemplateMaxCount]; // 文件格式数组
int TemplateCount; // 总共有多少文件，>2 && < TemplateMaxCount 
int NewFile1Type;
int NewFile2Type;
bool AutoShow;

int 
ID_CreateNewFile = 0,
ID_ShowBrowser,
ID_OnlyShowBrowser,
ID_SPLIT,
ID_SetConfig,
ID_CheckUpdate,
ID_AboutThis,
ID_CountNum;

typedef void (*NewFileFun)();


#define ID_LIST(IDName) IDName=ENUM_##IDName + TemplateCount;
#define CreateNewFileFun(id) void CreateNewFile##id(){CreateNewFile(id);};
//#define NewFileID(id) CreateNewFileFuns[id] = CreateNewFile##id
CreateNewFileFun(1)
CreateNewFileFun(2)
CreateNewFileFun(3)
CreateNewFileFun(4)
CreateNewFileFun(5)
CreateNewFileFun(6)
CreateNewFileFun(7)
CreateNewFileFun(8)
CreateNewFileFun(9)
CreateNewFileFun(10)
CreateNewFileFun(11)
CreateNewFileFun(12)
CreateNewFileFun(13)
CreateNewFileFun(14)
CreateNewFileFun(15)
CreateNewFileFun(16)
CreateNewFileFun(17)
CreateNewFileFun(18)
CreateNewFileFun(19)
CreateNewFileFun(20)

NewFileFun CreateNewFileFuns[TemplateMaxCount] = {
	CreateNewFile1,
	CreateNewFile2,
	CreateNewFile3,
	CreateNewFile4,
	CreateNewFile5,
	CreateNewFile6,
	CreateNewFile7,
	CreateNewFile8,
	CreateNewFile9,
	CreateNewFile10,
	CreateNewFile11,
	CreateNewFile12,
	CreateNewFile13,
	CreateNewFile14,
	CreateNewFile15,
	CreateNewFile16,
	CreateNewFile17,
	CreateNewFile18,
	CreateNewFile19,
	CreateNewFile20
};

//
// Initialize your plugin data here
// It will be called while plugin loading   
void pluginInit(HANDLE hModule)
{
	// Initialize dockable demo dialog
	g_hModule = hModule;
	_showBrowser.init((HINSTANCE)hModule, NULL);
}

//
// Here you can do the clean up, save the parameters (if any) for the next session
//
void pluginCleanUp()
{	
	TCHAR str[500]={0};	
	wsprintf(str,TEXT("%d"),TemplateCount);
	::WritePrivateProfileString(sectionName,StrTemplateCount,str,iniFilePath);

	// 循环写入
	for(int i=0;i<TemplateCount;i++)
	{
		TCHAR currStrNewFileType[MAX_PATH]={0};
		wsprintf(currStrNewFileType,StrNewFileType,i+1);
		wsprintf(str, TEXT("%d"),NewFileType[i].FileType);
		::WritePrivateProfileString(sectionName,currStrNewFileType,str,iniFilePath);
		TCHAR currStrNewFileName[MAX_PATH]={0};
		wsprintf(currStrNewFileName,StrNewFileName,i+1);
		::WritePrivateProfileString(sectionName, currStrNewFileName,NewFileType[i].FileName,iniFilePath);

	}
	wsprintf(str,TEXT("%d"),AutoShow?1:0);
	::WritePrivateProfileString(sectionName,StrAutoShow,str,iniFilePath);

	for (int i=0;i<ArrTempfile.size();i++)
	{
		DeleteFile(ArrTempfile[i].FileName);
	}
}

//
// Initialization of your plugin commands
// You should fill your plugins commands here
void commandMenuInit()
{
	//
	// Firstly we get the parameters from your plugin config file (if any)
	//
	// get path of plugin configuration
	::SendMessage(nppData._nppHandle, NPPM_GETPLUGINSCONFIGDIR, MAX_PATH, (LPARAM)iniFilePath);

	// if config path doesn't exist, we create it
	if (PathFileExists(iniFilePath) == FALSE)
	{
		::CreateDirectory(iniFilePath, NULL);
	}
	lstrcpy(Template1FilePath,iniFilePath);
	lstrcpy(Template2FilePath,iniFilePath);
	lstrcpy(TemplateFilePath,iniFilePath);
	// make your plugin config file full file path name
	PathAppend(iniFilePath, configFileName);
	PathAppend(Template1FilePath, strTemplate1);
	PathAppend(Template2FilePath, strTemplate2);
	PathAppend(TemplateFilePath, strTemplate);

	if ( GetFileAttributes(Template1FilePath)==0xFFFFFFFF )
	{
		// HTML L"D:\\ProgramFiles\\编辑工具\\npp.5.9.6.bin\\ansi\\plugins\\Config\\111111.dat"
		ofstream myFile; 
		FSConvert( myFile.open(Template1FilePath) );
		USES_CONVERSION;
		myFile.write(T2A(HTMLDefault),lstrlen(HTMLDefault));
		myFile.close();
	}
	if ( GetFileAttributes(Template2FilePath)==0xFFFFFFFF )
	{
		// JAVA
		ofstream myFile;
		FSConvert(myFile.open(Template2FilePath));
		USES_CONVERSION;
		myFile.write(T2A(JAVADefault),lstrlen(JAVADefault));
		myFile.close();
	}
	// get the parameter value from plugin config
	TemplateCount = ::GetPrivateProfileInt(sectionName, StrTemplateCount, TemplateMinCount, iniFilePath);
	if (TemplateCount<2 || TemplateCount > TemplateMaxCount)
	{
		TemplateCount = 2; // 最少需要2个文件
	}
	ID_LIST(ID_ShowBrowser)
	ID_LIST(ID_OnlyShowBrowser)
	ID_LIST(ID_SPLIT)
	ID_LIST(ID_SetConfig)
	ID_LIST(ID_CheckUpdate)
	ID_LIST(ID_AboutThis)
	ID_LIST(ID_CountNum)
	//funcItem = new FuncItem[ID_CountNum];
	// 根据文件总数获取所有的配置
	for(int i=0;i<TemplateCount;i++)
	{
		// 默认文件
		int L_Default = L_TEXT;
		switch(i){
			case 0:
				L_Default = L_HTML;
			break;
			case 1:
				L_Default = L_JAVA;
			break;
		}
		TCHAR currStrNewFileName[MAX_PATH]={0};
		wsprintf(currStrNewFileName,StrNewFileName,i+1);
		::GetPrivateProfileString(sectionName, currStrNewFileName, TEXT("") ,NewFileType[i].FileName,MAX_PATH, iniFilePath);
		TCHAR currStrNewFileType[MAX_PATH]={0};
		wsprintf(currStrNewFileType,StrNewFileType,i+1);
		NewFileType[i].FileType = ::GetPrivateProfileInt(sectionName, currStrNewFileType, L_Default , iniFilePath);
		// 根据文件数量生成对应的函数
	}
	AutoShow = ::GetPrivateProfileInt(sectionName, StrAutoShow, 1, iniFilePath)==1;

    //--------------------------------------------//
    //-- STEP 3. CUSTOMIZE YOUR PLUGIN COMMANDS --//
    //--------------------------------------------//
	for (int i=0;i<TemplateCount;i++)
	{
		ShortcutKey *new1Key = new ShortcutKey;
		switch(i)
		{
		case 0:
			new1Key->_isAlt = false;
			new1Key->_isCtrl = true;
			new1Key->_isShift = true;
			new1Key->_key = 0x4E ; //VK_N
			break;
		case 1:
			new1Key->_isAlt = true;
			new1Key->_isCtrl = true;
			new1Key->_isShift = true;
			new1Key->_key = 0x4E ; //VK_N
		    break;
		default:
			new1Key = NULL;
		    break;
		}
		TCHAR strMenu[MAX_PATH]={0}; // 
		wsprintf(strMenu,TEXT("Create %s file %d, %s"),LangTypeStr[NewFileType[i].FileType].c_str(),i+1,NewFileType[i].FileName);
		setCommand(ID_CreateNewFile+i, strMenu, CreateNewFileFuns[i], new1Key, false);
	}


	ShortcutKey *BrowserKey = new ShortcutKey;
	BrowserKey->_isAlt = false;
	BrowserKey->_isCtrl = true;
	BrowserKey->_isShift = false;
	BrowserKey->_key = 0x45 ; //VK_E        0x45
	setCommand(ID_ShowBrowser, TEXT("Show in Browser"), ShowBrowser, BrowserKey, false);
	
	setCommand(ID_OnlyShowBrowser, TEXT("Only show Browser"), OnlyShowBrowser, NULL, false);
	// Here you insert a separator
	setCommand(ID_SPLIT, TEXT("---"), NULL, NULL, false);

	setCommand(ID_SetConfig, TEXT("Option"), SetConfig, NULL, false);
	setCommand(ID_CheckUpdate, TEXT("Check for update"), checkUpdate, NULL, false);
	setCommand(ID_AboutThis, TEXT("About Newfile&&Browser"), AboutThis, NULL, false);
}


//
// Here you can do the clean up (especially for the shortcut)
//
void commandMenuCleanUp()
{
	// Don't forget to deallocate your shortcut here
	for (int i=0;i<TemplateCount;i++)
	{
		delete funcItem[ID_CreateNewFile+i]._pShKey;
	}
	delete funcItem[ID_ShowBrowser]._pShKey;
}

//----------------------------------------------//
//-- STEP 4. DEFINE YOUR ASSOCIATED FUNCTIONS --//
//----------------------------------------------//
void CreateNewFile(int flag)
{
	TCHAR currTemplateFilePath[MAX_PATH]={0};
	wsprintf(currTemplateFilePath, TemplateFilePath ,flag);
	// 检测原始文件
	if ( GetFileAttributes(currTemplateFilePath)==0xFFFFFFFF )
	{
		::MessageBox(nppData._nppHandle,TEXT("No template file, please set first in option!"),TEXT("No Template"),MB_OK);
		return;
	}
	int which = -1;
	::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&which);
	if (which == -1)
		return ;
	HWND curScintilla = (which == 0)?nppData._scintillaMainHandle:nppData._scintillaSecondHandle;
	if ( curScintilla == (HWND)-1 )
	{
		::MessageBox(nppData._nppHandle,TEXT("Could not get handle !"),TEXT(""),MB_OK);
		return;
	}
	//TCHAR fileContent[TemplateMaxSize];
	//ifstream myFile;
	int toSetLangType = L_TEXT;
	if ( flag < 0 || flag > TemplateMaxCount)
	{
		::MessageBox(nppData._nppHandle,TEXT("NewFile Flag Overflow!"),TEXT(""),MB_OK);
		return;
	}
	toSetLangType = NewFileType[flag-1].FileType;

	//char buffer[TemplateMaxSize]={0};
	//
	//myFile.seekg(0, ios::end);
	//long sizeLen = myFile.tellg();
	//myFile.seekg(0, ios::beg);
	//myFile.read(buffer,TemplateMaxSize-1);
	//myFile.close();

	//char* StrPos = strstr( buffer,"^!");
	//int pos = 0;
	//if ( StrPos != NULL)
	//{
	//	pos = StrPos - buffer;
	//}
	//// 将后面的数据移动上来
	//int iPos = pos;
	//while (buffer[iPos+2]!=0)
	//{
	//	buffer[iPos++]=buffer[iPos+2];
	//}
	//buffer[iPos+1]=buffer[iPos]=0;
	// 将buffer数据写到临时文件中，然后打开临时文件
	// 得到临时文件名称
	try
	{
		////////////////////////////////////////////////////////////////////////// 处理临时目录
		GetTempPath(_MAX_PATH, szTempPath);  
		// 再添加个文件夹
		lstrcat(szTempPath,TEXT("NPPPlugin\\"));
		CreateDirectory(szTempPath,NULL);
		/// 处理临时文件
		GetTempFileName(szTempPath,_T("NFB"),0,szOriTempfile);
		int len = lstrlen(szOriTempfile);
		// 去掉最后的 .tmp 还需要加上后缀
		memset(szTempfile,0,sizeof(szTempfile));
		_tcsncpy(szTempfile,szOriTempfile,len-4);
		tstring tmp = LangTypeStr[toSetLangType];
		TCHAR strExt[20]={0}; // 
		wsprintf(strExt,TEXT(".%s"),tmp.c_str());
		lstrcat(szTempfile,strExt );
		//MoveFile(szOriTempfile,szTempfile);
		DeleteFile(szOriTempfile);
		CopyFile(currTemplateFilePath,szTempfile,FALSE);
	}
	catch (...)
	{
		::MessageBox(nppData._nppHandle,TEXT("Create Temp file fail!"),TEXT(""),MB_OK);
		return;
	}
	// 存于队列，用于退出时删除
	TempFile tmp;
	GetLongPathName(szTempfile,tmp.FileName,MAX_PATH);
	ArrTempfile.push_back(tmp);
	// 文件改写
	//ofstream wFile;

	//FSConvert(wFile.open(szTempfile,ios::binary));

	//wFile.write(buffer,sizeLen);//strlen(buffer)
	//wFile.close();
	::SendMessage(nppData._nppHandle, NPPM_DOOPEN, 0, (LPARAM)szTempfile );
	::SendMessage(curScintilla, SCI_SETUNDOCOLLECTION, 0, 0 );
	//::SendMessage(curScintilla, SCI_SETUNDOCOLLECTION, 0, 0 );
	::SendMessage(curScintilla, SCI_EMPTYUNDOBUFFER, 0, 0 );
	// 查找到 ^! 然后替换
	int flags = SCFIND_MATCHCASE | SCFIND_WHOLEWORD;
	//TextToFind ttf;
	//ttf.chrg.cpMin = 0;
	//ttf.chrg.cpMax = ::SendMessage(curScintilla,SCI_GETLENGTH, 0, 0);
	//ttf.lpstrText = "^!";
	char *chText = "^!";
	//int pos = ::SendMessage(curScintilla,SCI_FINDTEXT, flags, (LPARAM)&ttf);
	::SendMessage(curScintilla, SCI_SETUNDOCOLLECTION, 1, 0 ); // 一定要设置，表明文本替换后已修改需要重新保存
	//::SendMessage(curScintilla, SCI_BEGINUNDOACTION ,0,0); 
	int pos =::SendMessage(curScintilla,SCI_SEARCHNEXT, flags, (LPARAM)chText);
	if (-1 != pos)
	{
		// 找到替换
		char *chText = "";
		::SendMessage(curScintilla,SCI_REPLACESEL, flags, (LPARAM)chText);
		// 登记为已经修改
		//::MessageBox(nppData._nppHandle,TEXT("!!!!"),TEXT(""),MB_OK);
	}
	//::SendMessage(curScintilla, SCI_SETTEXT, 0, (LPARAM)"aaa" );
	//::SendMessage(curScintilla, SCI_ENDUNDOACTION ,0,0);  
	//::SendMessage(curScintilla, SCI_SETUNDOCOLLECTION, 0, 0 ); // 将导致无法保存
	::SendMessage(curScintilla, SCI_EMPTYUNDOBUFFER, 0, 0 ); // 清除历史操作记录(文本替换记录)

	//::SendMessage(curScintilla, SCI_GOTOPOS,pos, 0);
	::SendMessage(nppData._nppHandle, NPPM_SETCURRENTLANGTYPE , 0, (LPARAM)toSetLangType);
	// 如果文件不存在，当 langtype 为 JAVA 时，有默认内容
}

//void CreateNewFile1(){CreateNewFile(1);};
//void CreateNewFile2(){CreateNewFile(2);};

bool DoBrowser = false;
void OnlyShowBrowser()
{
	_showBrowser.setParent(nppData._nppHandle);
	tTbData	data = {0};
	if (!_showBrowser.isCreated())
	{
		_showBrowser.create(&data);

		// define the default docking behaviour
		data.pszModuleName = _showBrowser.getPluginFileName();
		//_tcscpy(data.pszName, TEXT("Inner WebBrowser"));//TEXT("Inner WebBrowser");
		data.uMask			= DWS_DF_CONT_BOTTOM | DWS_ICONTAB;
		
		data.hIconTab		= (HICON)::LoadImage((HINSTANCE)g_hModule, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 0, 0, LR_LOADMAP3DCOLORS | LR_LOADTRANSPARENT);
		// the dlgDlg should be the index of funcItem where the current function pointer is
		data.dlgID = ID_ShowBrowser;
		::SendMessage(nppData._nppHandle, NPPM_DMMREGASDCKDLG, 0, (LPARAM)&data);
	}
	_showBrowser.display();
}
void ShowBrowser()
{
	OnlyShowBrowser();
	// 将当前文件保存，并用浏览器显示
	TCHAR path[MAX_PATH];
	::SendMessage(nppData._nppHandle, NPPM_GETFULLCURRENTPATH , 0, (LPARAM)path);
	if ( GetFileAttributes(path)==0xFFFFFFFF )
	{
		//文件不存在
		return ;
	}
	DoBrowser = true;
	::SendMessage(nppData._nppHandle, NPPM_SAVECURRENTFILE , 0, 0);
	lstrcpy(_showBrowser.URLString,path);
	_showBrowser.GotoURL();
	DoBrowser = false;
}
void SetConfig(){
	dlgConfig.init((HINSTANCE)g_hModule, nppData);
	dlgConfig.doDialog();
};
void AboutThis(){
	::MessageBox(nppData._nppHandle,TEXT("You can use Ctrl+Shift+N or Ctrl+Shift+Alt+N to open a new file with default text which is defined yourself in template.\nIn template ^! means the default cursor position.\nIt has innner browser with IE kernel, use Ctrl+E to run current file in browser.\nCan auto show in browser when saved(HTML file)\nThe file's encode is defined by template\nCan define and name 20 templates for new files.\n\n Version: 0.1.3   Author: Austin Young<pattazl@gmail.com>"),TEXT("About"),MB_OK);
};

void checkUpdate()
{
	ShellExecute(NULL, TEXT("open"),TEXT("http://sourceforge.net/projects/locationnav/files"), NULL, NULL, SW_SHOW);
}

bool setCommand(size_t index, TCHAR *cmdName, PFUNCPLUGINCMD pFunc, ShortcutKey *sk, bool check0nInit) 
{
	if (index >= ID_CountNum)
		return false;

	if (!pFunc)
		return false;

	lstrcpy(funcItem[index]._itemName, cmdName);
	funcItem[index]._pFunc = pFunc;
	funcItem[index]._init2Check = check0nInit;
	funcItem[index]._pShKey = sk;

	return true;
}