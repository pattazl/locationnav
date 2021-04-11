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
	// ��Ҫ�� MarkTypeArr ͳһ
	MarkHightLight = 0,
	MarkUnderLine  ,
	MarkRect ,
	MarkRoundrect ,
	MarkCircle ,
	MarkTriangle,
	MarkArrow ,
	////////////////�˴�Ϊ�б�Ĵ���
	MarkBookmark, // ��Ϊ��ǩ��ǩ��������ǩ���ܣ��˱�ʾʹ����ǩ�������ֵ���� SC_MARK ��
	MarkArrows,  // �����ı�ǩ
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
extern deque<LocationInfo>LocationSave;// ���ڹر�ǰ�������ݵ�����
extern long LocationPos;  // �����е�λ��
extern bool PositionSetting; // �Ƿ������ת
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
		// ����������ͬ������
		//::MessageBox(NULL, TEXT("refreshDlg"), TEXT(""), MB_OK);
		//EnterCriticalSection(&criCounter);
		int firstChg=-1,lastChg=-1;
		for (int i=0;i<LocationList.size();i++)
		{
			//�����ܳ���һ����λ�õ�ɾ�� 
			// || LocationList[i-1].bufferID == 0 
			if ( (i >0 && 
				LocationList[i].position == LocationList[i-1].position &&
				(LocationList[i].bufferID == LocationList[i-1].bufferID )) )
			{
				LocationList.erase(LocationList.begin()+i);
				// LocationPos �ı�
				if(LocationPos>=i)LocationPos--;
				if (LocationPos<0)
				{
					LocationPos =0;
				}
				//ɾ�����λ����Ҫ���¼���
				i--;
			}
			// ����ǰ�����ֻ�ڱ����в��ң���ôֻ����Լ��ļ���
			if ( InCurr )
			{
				// ����ͬһ���ļ�������
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
