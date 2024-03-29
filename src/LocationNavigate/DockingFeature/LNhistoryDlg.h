//this file is part of notepad++
//Copyright (C) 2011 AustinYoung<pattazl@gmail.com>
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

#ifndef LNHISTORY_DLG_H
#define LNHISTORY_DLG_H

#include "DockingDlgInterface.h"
#include "PluginDefinition.h"
#include "SelfCtrl.h"
#include "resource.h"

#define SELF_REFRESH WM_USER+9

enum MarkType{
	// 需要和 MarkTypeArr 统一
	MarkHightLight = 0,
	MarkUnderLine  ,
	MarkRect ,
	MarkRoundrect ,
	MarkCircle ,
	MarkTriangle,
	MarkArrow ,
	////////////////此处为列表的次序
	MarkBookmark, // 改为书签标签，不用书签功能，此表示使用书签，定义的值不在 SC_MARK 中
	MarkArrows,  // 新增的标签
};
const TCHAR sectionName[] = TEXT("Setting");
const TCHAR strMaxOffset[] = TEXT("MaxOffset");
const TCHAR strMaxList[] = TEXT("MaxList");
const TCHAR strAutoClean[] = TEXT("AutoClean");
const TCHAR strAlwaysRecord[] = TEXT("AlwaysRecord");
const TCHAR strSaveRecord[] = TEXT("SaveRecord");
const TCHAR strInCurr[] = TEXT("InCurr");
const TCHAR strAutoRecord[] = TEXT("AutoRecord");
const TCHAR strNeedMark[] = TEXT("NeedMark");
const TCHAR strByBookMark[] = TEXT("ByBookMark");
const TCHAR strMarkColor[] = TEXT("MarkColor");
const TCHAR strSaveColor[] = TEXT("SaveColor");
const TCHAR strRecordContent[] = TEXT("RecordContent");
const TCHAR configFileName[] = TEXT("LocationNavigate.ini");
extern TCHAR iniFilePath[MAX_PATH];

extern deque<LocationInfo>LocationList;
extern deque<LocationInfo>LocationSave;// 用于关闭前保存数据的数组
extern long LocationPos;  // 队列中的位置
extern bool PositionSetting; // 是否进行跳转
extern int MaxOffset;
extern int MaxList ;
extern bool AutoClean ;
extern bool AlwaysRecord;
extern bool SaveRecord;
extern bool InCurr;
extern bool bAutoRecord;
extern bool NeedMark;
extern MarkType ByBookMark;
extern long MarkColor;
extern long SaveColor;

extern HWND curScintilla;
//extern CRITICAL_SECTION criCounter;
extern FuncItem funcItem[nbFunc];
extern bool menuState[nbFunc];
extern HWND hToolbar;
extern int IconID[nbFunc];
extern NppData nppData;

extern void AddList(bool flag=false);
extern void ClearLocationList();

void SetPosByIndex();
void EnableTBButton(menuList flagIndex,bool state);
#define TB_ENABLEBUTTON         (WM_USER + 1)
class LocationNavigateDlg : public DockingDlgInterface
{
public :
	LocationNavigateDlg() : DockingDlgInterface(IDD_LOCATIONNAVIGATE){};

    virtual void display(bool toShow = true) const{
        DockingDlgInterface::display(toShow);
        if (toShow)
            ::SetFocus(::GetDlgItem(_hSelf, IDOK));

		::SendMessage( _hSelf, SELF_REFRESH, LocationPos, 0);
    };
	void refreshDlg(){
		// 负责清理相同的数据
		//::MessageBox(NULL, TEXT("refreshDlg"), TEXT(""), MB_OK);
		//EnterCriticalSection(&criCounter);
		int firstChg=-1,lastChg=-1;
		for (int i=0;i<LocationList.size();i++)
		{
			//还可能出现一样的位置的删除 
			// || LocationList[i-1].bufferID == 0 
			if ( (i >0 && 
				LocationList[i].position == LocationList[i-1].position &&
				(LocationList[i].bufferID == LocationList[i-1].bufferID )) )
			{
				LocationList.erase(LocationList.begin()+i);
				// LocationPos 改变
				if(LocationPos>=i)LocationPos--;
				if (LocationPos<0)
				{
					LocationPos =0;
				}
				//删除后此位置需要重新计算
				i--;
			}
			// 如果是按下了只在本文中查找，那么只检查自己文件的
			if ( InCurr )
			{
				// 不是同一个文件，跳过
				if( lstrcmp(LocationList[i].FilePath,currFile) != 0 ){
					continue;
				}
			}
			if ( firstChg==-1 && LocationList[i].changed )
			{
				firstChg = i;
			}
			if ( LocationList[i].changed )
			{
				lastChg = i;
			}
		}
		if ( LocationPos <= 0)
		{
			EnableTBButton(menuPrevious,false);
		}else
		{
			EnableTBButton(menuPrevious,true);
		}

		if ( LocationPos == LocationList.size()-1 || LocationList.size()==0 )
		{
			EnableTBButton(menuNext,false);
		}else
		{
			EnableTBButton(menuNext,true);
		}
		
		if ( firstChg == -1 || LocationPos <= firstChg)
		{
			EnableTBButton(menuChgPrevious,false);
		}else
		{
			EnableTBButton(menuChgPrevious,true);
		}
		if ( lastChg == -1 || LocationPos >= lastChg)
		{
			EnableTBButton(menuChgNext,false);
		}else
		{
			EnableTBButton(menuChgNext,true);
		}

		//LeaveCriticalSection(&criCounter);
		if ( this->isCreated() )
		{
			::SendMessage( _hSelf, SELF_REFRESH, LocationPos, 0);
		}
	};
	void setParent(HWND parent2set){
		_hParent = parent2set;
	};
	HWND _hListBox,_hUG_T,_hUG_E,_hUG_OK,_hMark,_hUG_T2,_hUG_E2,_hABOUT,_hAuto,_hClear,_hAlways,_hSaveRecord,_hInCurr,_hBookmark,_hColor,_hSaveColor;
protected :
	virtual BOOL CALLBACK run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam);
	SelfCtrl _color,_savecolor;
private :
	void refreshValue();
};

#endif //LNHISTORY_DLG_H
