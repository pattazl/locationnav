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

#include "ConfigDialog.h"
//"TXT","PHP","C","CPP","CS","OBJC","JAVA","RC","HTML","XML","MAKEFILE","PASCAL","BATCH","INI","NFO","USER","ASP","SQL","VB","JS","CSS","PERL","PYTHON","LUA","TEX","FORTRAN","BASH","FLASH","NSIS","TCL","LISP","SCHEME","ASM","DIFF","PROPS","PS","RUBY","SMALLTALK","VHDL","KIX","AU3","CAML","ADA","VERILOG","MATLAB","HASKELL","INNO","SEARCHRESULT","CMAKE","YAML"
#ifdef _UNICODE
typedef wstring tstring;
#else
typedef string tstring;
#endif
tstring LangTypeStr[] = {TEXT("TXT"),TEXT("PHP"),TEXT("C"),TEXT("CPP"),TEXT("CS"),TEXT("OBJC"),TEXT("JAVA"),TEXT("RC"),TEXT("HTML"),TEXT("XML"),TEXT("MAKEFILE"),TEXT("PASCAL"),TEXT("BATCH"),TEXT("INI"),TEXT("NFO"),TEXT("USER"),TEXT("ASP"),TEXT("SQL"),TEXT("VB"),TEXT("JS"),TEXT("CSS"),TEXT("PERL"),TEXT("PYTHON"),TEXT("LUA"),TEXT("TEX"),TEXT("FORTRAN"),TEXT("BASH"),TEXT("FLASH"),TEXT("NSIS"),TEXT("TCL"),TEXT("LISP"),TEXT("SCHEME"),TEXT("ASM"),TEXT("DIFF"),TEXT("PROPS"),TEXT("PS"),TEXT("RUBY"),TEXT("SMALLTALK"),TEXT("VHDL"),TEXT("KIX"),TEXT("AU3"),TEXT("CAML"),TEXT("ADA"),TEXT("VERILOG"),TEXT("MATLAB"),TEXT("HASKELL"),TEXT("INNO"),TEXT("SEARCHRESULT"),TEXT("CMAKE"),TEXT("YAML")};

TCHAR HTMLDefault[512] = TEXT("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\" \"http://www.w3.org/TR/html4/loose.dtd\">\n<html>\n <head>\n  <title> New Document </title>\n  <meta name=\"Generator\" content=\"NPP-Plugin\">\n  <meta name=\"Author\" content=\"\">\n  <meta name=\"Keywords\" content=\"\">\n  <meta name=\"Description\" content=\"\">\n </head>\n <body>\n  <script type=\"text/javascript\">\n    ^!\n  </script>\n </body>\n</html>");
TCHAR JAVADefault[512] = TEXT("class ^! \n{\n	public static void main(String[] args) \n	{\n		System.out.println(\"Hello World!\");\n	}\n}\n");

void ConfigDialog::doDialog()
{
    if (!isCreated())
        create(IDD_DIALOGCONFIG);

	goToCenter();
}
void ConfigDialog::GetFileNoByCount()
{
	int maxCount = ::SendMessage( _hListCount, CB_GETCURSEL, 0, 0)+TemplateMinCount;
	/*TCHAR strHint[50]={0}; // 
	wsprintf(strHint,TEXT("_hListCount %d"),index);
	{
	::MessageBox(_hSelf,strHint,TEXT("Warning"),MB_OK);
	}*/
	::SendMessage( _hListNo, CB_RESETCONTENT, 0, 0 );
	preNoSelectID = -1;
	for ( int i=0;i< maxCount ;i++ )
	{
		TCHAR strHint[50]={0}; // 
		wsprintf(strHint,TEXT("File %d"),i+1);
		::SendMessage( _hListNo, CB_ADDSTRING, 0, (LPARAM)strHint );
	}
}
void ConfigDialog::SaveConfig2Arr()
{
	// 需要根据 preNoSelectID 来保存 _hListNo 之前的值，包括文件类型和文件名
	if ( preNoSelectID != -1)
	{
		// 获取当前的 ListType 和 FileName的内容
		int fileTypeId = ::SendMessage( _hListType, CB_GETCURSEL, 0, 0);
		tempNewFileType[preNoSelectID].FileType =  fileTypeId;
		// 获取指定长度126
		::SendMessage( _hEditTxt, WM_GETTEXT, 126, (LPARAM)tempNewFileType[preNoSelectID].FileName);
	}
}
void ConfigDialog::GetFileTypeByNo()
{
	int fileNoId = ::SendMessage( _hListNo, CB_GETCURSEL, 0, 0);
	preNoSelectID = fileNoId;
	if ( fileNoId > -1 && fileNoId < TemplateMaxCount )
	{
		::SendMessage( _hListType, CB_RESETCONTENT, 0, 0 );
		int NewFileIndex=-1;
		for ( int i=0;i<L_EXTERNAL;i++ )
		{
			tstring tmp = LangTypeStr[i];
			TCHAR strHint[500]={0}; // 
			wsprintf(strHint,TEXT("%s"),tmp.c_str());
			::SendMessage( _hListType, CB_ADDSTRING, 0, (LPARAM)strHint );
			if ( tempNewFileType[fileNoId].FileType == i)
			{
				NewFileIndex = i;
			}
		}
		// 设置当前点
		::SendMessage( _hListType, CB_SETCURSEL, NewFileIndex, 0);
		// 设置文件名信息
		::SendMessage( _hEditTxt, WM_SETTEXT, 0 , (WPARAM)tempNewFileType[fileNoId].FileName);
	}
}
void ConfigDialog::ReadConfigFile( LangType langtype,int fileTypeId)
{
	// 直接打开并根据类型设置 // 目前系统只默认识别HTML和JAVA这2种格式
	//tstring lang = LangTypeStr[langtype];
	TCHAR currTemplateFilePath[MAX_PATH];
	wsprintf(currTemplateFilePath, TemplateFilePath ,fileTypeId+1);
	// 需要判断文件是否存在
	if ( GetFileAttributes(currTemplateFilePath)==0xFFFFFFFF )
	{
		//文件不存在
		ofstream myFile; 
		FSConvert(myFile.open(currTemplateFilePath));
		myFile.close();
	}
	::SendMessage(nppData._nppHandle, NPPM_DOOPEN, 0, (LPARAM)currTemplateFilePath);

	::SendMessage(nppData._nppHandle, NPPM_SETCURRENTLANGTYPE , 0, (LPARAM)langtype);
}

BOOL CALLBACK ConfigDialog::run_dlgProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message) 
	{
        case WM_INITDIALOG :
		{
			// 复制节点
			memcpy( tempNewFileType,NewFileType,sizeof(tempNewFileType));
			// 界面元素
			_hListType = GetDlgItem(_hSelf,IDC_COMBOTYPE);
			_hListNo = GetDlgItem(_hSelf,IDC_COMBONO);
			_hListCount = GetDlgItem(_hSelf,IDC_COMBOCOUNT);
			_hCkBox = GetDlgItem(_hSelf,IDC_CHECK1);
			_hEditTxt = GetDlgItem(_hSelf,IDC_EDIT1);
			
			
			::SendMessage( _hEditTxt, WM_SETTEXT, 0, 0 );
			::SendMessage( _hListType, CB_RESETCONTENT, 0, 0 );
			::SendMessage( _hListNo, CB_RESETCONTENT, 0, 0 );
			preNoSelectID = -1;
			::SendMessage( _hListCount, CB_RESETCONTENT, 0, 0 );
			// 循环添加清单
			for ( int i=TemplateMinCount-1;i<TemplateMaxCount;i++ )
			{
				TCHAR strHint[50]={0}; // 
				wsprintf(strHint,TEXT("%d"),i+1);
				::SendMessage( _hListCount, CB_ADDSTRING, 0, (LPARAM)strHint );
			}
			// 设置当前点
			::SendMessage( _hListCount, CB_SETCURSEL, TemplateCount-TemplateMinCount, 0);
			// 设置文件清单
			GetFileNoByCount();
			// 设置checkbox
			::SendMessage( _hCkBox, BM_SETCHECK ,(LPARAM)(AutoShow?1:0),0);
			return TRUE;
		}
		break;
		case WM_COMMAND : 
		{
			switch (wParam)
			{
				case MAKELONG(IDC_COMBOCOUNT,CBN_SELCHANGE ):
				{
					GetFileNoByCount();
				}
				break;
				case MAKELONG(IDC_COMBONO,CBN_SELCHANGE ):
				{
					SaveConfig2Arr();
					GetFileTypeByNo();
				}
				break;
				case MAKELONG(IDC_BUTTON1,BN_CLICKED):
				{
					int NewFileIndex=-1;
					int fileTypeId = ::SendMessage( _hListNo, CB_GETCURSEL, 0, 0);
					NewFileIndex = ::SendMessage( _hListType, CB_GETCURSEL, 0, 0);
					if (NewFileIndex==-1)
					{
						::MessageBox(_hSelf,TEXT("Please select file type!"),TEXT("Warning"),MB_OK);
						return TRUE;
					}
					ReadConfigFile((LangType)NewFileIndex,fileTypeId);
				}
				break;
                case IDCANCEL :
					destroy();
				break;
				case IDOK :
					if (wParam == IDOK)
					{
						SaveConfig2Arr();

						TemplateCount = ::SendMessage( _hListCount, CB_GETCURSEL, 0, 0)+TemplateMinCount; //

						AutoShow = (::SendMessage(_hCkBox, BM_GETCHECK,0,0))==1;

						memcpy( NewFileType , tempNewFileType,sizeof(NewFileType)); // 复制信息回去
					}
					display(FALSE);
					return TRUE;
				break;
				default :
					break;
			}
		}
		break;
	}
	return FALSE;
}

