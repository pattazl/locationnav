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

#include "LNhistoryDlg.h"
#include "PluginDefinition.h"
extern void InitBookmark();
extern vector<MarkData> MarkHistory;
void LocationNavigateDlg::refreshValue()
{
	TCHAR strHint[500]={0};	
	wsprintf(strHint,TEXT("%d"),MaxOffset);
	::SendMessage( _hUG_E, WM_SETTEXT ,0, (WPARAM)strHint);
	wsprintf(strHint,TEXT("%d"),MaxList);
	::SendMessage( _hUG_E2, WM_SETTEXT,0,(WPARAM)strHint);
//BM_SETCHECK BM_SETCHECK:MF_UNCHECKED
	::SendMessage( _hAuto, BM_SETCHECK ,(LPARAM)(AutoClean?1:0),0);
	::SendMessage( _hAlways, BM_SETCHECK ,(LPARAM)(AlwaysRecord?1:0),0);
	::SendMessage( _hSaveRecord, BM_SETCHECK ,(LPARAM)(SaveRecord?1:0),0);
	::SendMessage( _hInCurr, BM_SETCHECK ,(LPARAM)(InCurr?1:0),0);
	::SendMessage( _hMark, BM_SETCHECK ,(LPARAM)(NeedMark?1:0),0);
	

	//::SendMessage( _hBookmark, BM_SETCHECK ,(LPARAM)(ByBookMark?1:0),0);
	::SendMessage(_hBookmark, CB_SETCURSEL, ByBookMark, 0);
}
void EnableTBButton(menuList flagIndex,bool state)
{
	if ( menuState[flagIndex] !=state )
	{
		//TCHAR buffer[100]={0};
		//wsprintf(buffer,TEXT("position=%d,%d"),flagIndex,state);
		//::MessageBox(NULL, buffer, TEXT(""), MB_OK);
		// ״̬�����ϣ���Ҫ�ı�
		::EnableMenuItem(::GetMenu(nppData._nppHandle),
			funcItem[flagIndex]._cmdID,MF_BYCOMMAND|(state?MF_ENABLED:MF_GRAYED));
		if ( IconID[flagIndex] !=-1)
		{
			::SendMessage(hToolbar,TB_ENABLEBUTTON,(WPARAM)IconID[flagIndex],MAKELONG((state?TRUE:FALSE), 0));
			menuState[flagIndex] = state;
		}
		
	}
}
void SetPosByIndex()
{
	long maxLen = LocationList.size()-1;
	if (maxLen==-1 || LocationPos<0 || LocationPos>maxLen)
	{
		return;
	}
	PositionSetting = true;
	long pos = LocationList[LocationPos].position;
	long res = ::SendMessage(nppData._nppHandle, NPPM_DOOPEN, 0 , (LPARAM)LocationList[LocationPos].FilePath);
	if (pos != -1 && res ==1)
	{
		// Get the current scintilla
		//int which = -1;
		//::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&which);
		//if (which == -1)
		//	return ;
		//HWND curScintilla = (which == 0)?nppData._scintillaMainHandle:nppData._scintillaSecondHandle;

		// �˴�Ϊ���ж�λ��������Ϊ���붨λ
		//long line = ::SendMessage(curScintilla, SCI_LINEFROMPOSITION, pos , 0);
		//::SendMessage(curScintilla, SCI_ENSUREVISIBLE, line, 0);

		// ��Ҫ��ֱ���У����붨λ
		// ��ȡ��ʼ��ʾ������,��Щ��׼ȷ
		long beginLines = ::SendMessage(curScintilla, SCI_GETFIRSTVISIBLELINE , 0 , 0);
		// ��ȡ��ǰ��ʾ������
		//long currLines = ::SendMessage(curScintilla, SCI_GETCURLINE , 0 , 0);
		//long position = ::SendMessage(curScintilla, SCI_GETCURRENTPOS, 0, 0);
		long targetLine = ::SendMessage(curScintilla, SCI_LINEFROMPOSITION, pos, 0);
		// ��ȡ��Ļ�߶�
		long screenLines = ::SendMessage(curScintilla, SCI_LINESONSCREEN , 0 , 0);
		

		// ���ݵ�ǰλ�ú�Ŀ��λ�ý��м��㣬����������
		long currCenterLine = (int)(beginLines+screenLines/2);
		long scrollLine = 0; // currLines2-(int)(beginLines+screenLines/2)
		// ֱ�Ӹ��ݵ�ǰ�������к�Ŀ���еľ�����й������ã��������������ж�λ��׼���ݲ���
		//scrollLine = targetLine - currCenterLine ;
		//::SendMessage(curScintilla, SCI_LINESCROLL , 0 , scrollLine); 
		// ����
		if(targetLine < beginLines){
			scrollLine = -screenLines/2;
		}else if ( targetLine > beginLines+screenLines)
		{
			scrollLine = screenLines/2;
		}
		// ��λ������λ��
		::SendMessage(curScintilla, SCI_GOTOPOS, pos, 0);
		// ���������Ļ
		::SendMessage(curScintilla, SCI_LINESCROLL , 0 , scrollLine ); 
		// ��ֹ�������������λ��ȷ�����ڿɼ���Χ
		::SendMessage(curScintilla, SCI_SCROLLCARET , 0 , 0);
		
		//::SendMessage(curScintilla, SCI_GOTOLINE, line-1, 0);
		//long caretPos = ::SendMessage(curScintilla, SCI_GETCURRENTPOS, 0, 0);
		//::SendMessage(curScintilla, SCI_GETCURRENTPOS, 0, 0);
		// ::SendMessage(nppData._nppHandle, NPPM_DOOPEN, 0 , (LPARAM)list_data[0].filename);
	}else
	{
		PositionSetting = false;
	}
}
void ClearLocationList()
{
	LocationList.clear();
	LocationPos = -1;
}
BOOL CALLBACK LocationNavigateDlg::run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) 
	{
		case WM_COMMAND : 
		{
			switch (wParam)
			{
				case MAKELONG(IDC_BUTTON_CLEAR,BN_CLICKED):
				{
					ClearLocationList();
					refreshDlg();
					::SendMessage(curScintilla, SCI_MARKERDELETEALL, _MARK_INDEX,0);
					::SendMessage(curScintilla, SCI_MARKERDELETEALL, _SAVE_INDEX,0);
					MarkHistory.clear();
				}
				break;
				case MAKELONG(IDC_CHECK_MARK,BN_CLICKED):
					{
						NeedMark  = (::SendMessage(_hMark, BM_GETCHECK,0,0))==1;
						InitBookmark();
						// ˢ�²˵�
						::CheckMenuItem(::GetMenu(nppData._nppHandle), funcItem[menuNeedMark]._cmdID, MF_BYCOMMAND | (NeedMark?MF_CHECKED:MF_UNCHECKED));
					}
				break;
				case MAKELONG(IDC_CHECK_INCURR,BN_CLICKED):
				{
					InCurr  = (::SendMessage(_hInCurr, BM_GETCHECK,0,0))==1;
					// ˢ�²˵�
					::CheckMenuItem(::GetMenu(nppData._nppHandle), funcItem[menuInCurr]._cmdID, MF_BYCOMMAND | (InCurr?MF_CHECKED:MF_UNCHECKED));
					refreshDlg();
				}
				break;
				case MAKELONG(IDC_CHECK_SAVERECORD,BN_CLICKED):
				case MAKELONG(IDC_CHECK_AUTO,BN_CLICKED):
				case MAKELONG(IDC_CHECK_ALWAYS,BN_CLICKED):
				case MAKELONG(IDC_CHECK_BOOKMARK,CBN_SELCHANGE):

				case MAKELONG(ID_LNHISTORY_EDIT,EN_KILLFOCUS):
				case MAKELONG(ID_LNHISTORY_EDIT2,EN_KILLFOCUS):

				{
					BOOL isSuccessful;
					int val = ::GetDlgItemInt(_hSelf, ID_LNHISTORY_EDIT, &isSuccessful, FALSE);
					if ( val >=2 && val <=5000 )
					{
						MaxOffset = val;
					}
					val = ::GetDlgItemInt(_hSelf, ID_LNHISTORY_EDIT2, &isSuccessful, FALSE);
					if ( val >=5 && val <=300 )
					{
						MaxList = val;
					}
					AutoClean = (::SendMessage(_hAuto, BM_GETCHECK,0,0))==1;
					AlwaysRecord = (::SendMessage(_hAlways, BM_GETCHECK,0,0))==1;
					SaveRecord  = (::SendMessage(_hSaveRecord, BM_GETCHECK,0,0))==1;
					int preByBookMark = ByBookMark;
					ByBookMark = (MarkType)::SendMessage(_hBookmark, CB_GETCURSEL,0,0);
					if ( ByBookMark != preByBookMark)
					{
						//��Ҫ�ı����������ʽ
						//if( MarkBookmark!= ByBookMark)
						{
							InitBookmark();
						}
					}
					refreshValue();
					//::MessageBox(this->getHSelf(),TEXT("Setting is Saved!") , TEXT("Info"), MB_OK);
				}
				break;
				// ��Ӧ˫���Ͱ�ť�¼�
				case MAKELONG(IDC_LOC_LIST,LBN_DBLCLK):
				case IDOK :
				{
					long index = ::SendMessage( _hListBox, LB_GETCURSEL, 0, 0);
					if ( index > -1 && index < LocationList.size())
					{
						LocationPos = index;
						SetPosByIndex();
					}
					//::SendMessage(nppData._nppHandle, NPPM_GETPOSFROMBUFFERID, 0, (LPARAM)&which);
					refreshDlg();
					return TRUE;
				}
			break;
			}
				return FALSE;
		}
		break;
		case SELF_REFRESH://WM_TIMER
		{
			// ������Ҫ��� �ṹ�Ƿ�ı��
			// ��ȡ����ˢ�µ�ҳ��
			::SendMessage( _hListBox, LB_RESETCONTENT, 0, 0 );
			//::MessageBox(NULL,TEXT("111") , TEXT(""), MB_OK);%d,,LocationList[i].bufferID
			for ( int i=0;i<LocationList.size();i++ )
			{
				TCHAR strHint[500]={0};
				wsprintf(strHint,TEXT("%c,%d,%s"),LocationList[i].changed?'!':'=',LocationList[i].position+1,LocationList[i].FilePath);
				::SendMessage( _hListBox, LB_ADDSTRING, 0, (LPARAM)strHint );
			}
			// ���õ�ǰ��
			::SendMessage( _hListBox, LB_SETCURSEL, LocationPos, 0);

			//refreshValue();
		}
		break;
		case WM_INITDIALOG:
			{
			_hListBox =	GetDlgItem(_hSelf, IDC_LOC_LIST);
			_hUG_T =	GetDlgItem(_hSelf, ID_UGO_STATIC);
			_hUG_E =	GetDlgItem(_hSelf, ID_LNHISTORY_EDIT);

			_hUG_T2 =	GetDlgItem(_hSelf, ID_UGO_STATIC2);
			_hUG_E2 =	GetDlgItem(_hSelf, ID_LNHISTORY_EDIT2);

			_hUG_OK =	GetDlgItem(_hSelf, IDOK);
			_hMark =	GetDlgItem(_hSelf, IDC_CHECK_MARK);

			_hABOUT = GetDlgItem(_hSelf, IDC_STATIC_ABOUT);

			_hAuto = GetDlgItem(_hSelf, IDC_CHECK_AUTO);
			_hAlways = GetDlgItem(_hSelf, IDC_CHECK_ALWAYS);

			_hSaveRecord = GetDlgItem(_hSelf, IDC_CHECK_SAVERECORD);
			
			_hClear = GetDlgItem(_hSelf, IDC_BUTTON_CLEAR);

			_hInCurr = GetDlgItem(_hSelf, IDC_CHECK_INCURR);

			_hBookmark = GetDlgItem(_hSelf, IDC_CHECK_BOOKMARK);

			::SendMessage( _hBookmark, CB_ADDSTRING ,0,(LPARAM)TEXT("Mark with highlight"));
			::SendMessage( _hBookmark, CB_ADDSTRING ,0,(LPARAM)TEXT("Mark with underline"));
			::SendMessage( _hBookmark, CB_ADDSTRING ,0,(LPARAM)TEXT("Mark with rect"));
			::SendMessage( _hBookmark, CB_ADDSTRING ,0,(LPARAM)TEXT("Mark with roundrect"));
			::SendMessage( _hBookmark, CB_ADDSTRING ,0,(LPARAM)TEXT("Mark with circle"));
			::SendMessage( _hBookmark, CB_ADDSTRING ,0,(LPARAM)TEXT("Mark with triangle"));
			::SendMessage( _hBookmark, CB_ADDSTRING ,0,(LPARAM)TEXT("Mark with arrow"));
			::SendMessage( _hBookmark, CB_ADDSTRING ,0,(LPARAM)TEXT("Mark with bookmark"));
			::SendMessage(_hBookmark, CB_ADDSTRING, 0, (LPARAM)TEXT("Mark with arrows"));

			_hColor = GetDlgItem(_hSelf, ID_STATIC_COLOR);
			_color.init(_hInst, _hSelf);
			// 0x86c2ff
			_color.create(_hColor,_MARK_INDEX,MarkColor);

			_hSaveColor = GetDlgItem(_hSelf, ID_STATIC_SAVECOLOR);
			_savecolor.init(_hInst, _hSelf);
			_savecolor.create(_hSaveColor,_SAVE_INDEX,SaveColor);
			refreshValue();
			// 
			//SetTimer(_hSelf,1,500,NULL);
			}
			break;
		case WM_SIZE:
		case WM_MOVE:
			{
				// ��������λ��
				RECT rc;
				getClientRect(rc);

				::MoveWindow(_hListBox,rc.left, rc.top, rc.right, rc.bottom-70,TRUE);

				::MoveWindow(_hUG_T, rc.left+60, rc.bottom-62, 70, 18, TRUE);
				::MoveWindow(_hUG_E, rc.left+135, rc.bottom-65, 40, 18, TRUE);

				::MoveWindow(_hUG_T2, rc.left+60, rc.bottom-42, 70, 18, TRUE);
				::MoveWindow(_hUG_E2, rc.left+135, rc.bottom-45, 40, 18, TRUE);

				::MoveWindow(_hUG_OK, rc.left+10, rc.bottom-65, 40,18, TRUE);
				::MoveWindow(_hMark, rc.left+10, rc.bottom-45, 40,18, TRUE);
				
				::MoveWindow(_hABOUT, rc.left+185, rc.bottom-70, 400,40, TRUE);
				::MoveWindow(_hClear, rc.left+180, rc.bottom-38, 55,16, TRUE);

				::MoveWindow(_hColor,  rc.left+245, rc.bottom-38, 70,16, TRUE);
				::MoveWindow(_hSaveColor,  rc.left+315, rc.bottom-38, 70,16, TRUE);
				::MoveWindow( _hAuto ,  rc.left+390, rc.bottom-38, 150,16, TRUE);

				::MoveWindow(_hInCurr, rc.left+10, rc.bottom-22, 50,16, TRUE);
				::MoveWindow(_hSaveRecord, rc.left+60, rc.bottom-22, 180,16, TRUE);
				::MoveWindow( _hBookmark,  rc.left+250, rc.bottom-22, 120,16, TRUE);
				::MoveWindow(_hAlways,  rc.left+390, rc.bottom-22, 110,16, TRUE);

				redraw();
			}
		break;
		case WM_NOTIFY: 
		{
			LPNMHDR	pnmh	= (LPNMHDR)lParam;
			if (pnmh->hwndFrom == _hParent)
			{
				switch (LOWORD(pnmh->code))
				{
				case DMN_CLOSE:
					{
						break;
					}
				default:
					break;
				}
			}
			break;
		}
		default :
			return DockingDlgInterface::run_dlgProc(message, wParam, lParam);
	}
	return TRUE;
}
deque<LocationInfo>LocationList;
deque<LocationInfo>LocationSave;
long LocationPos=0;
bool PositionSetting = false;
int MaxOffset=100;
int MaxList = 50;
bool AutoClean = false;
HWND curScintilla=0;
bool AlwaysRecord=false;
bool SaveRecord = false;
bool InCurr = false;
bool bAutoRecord = true;
bool NeedMark = false;
MarkType ByBookMark = MarkHightLight;
long MarkColor = DefaultColor;
long SaveColor = DefaultSaveColor;
//CRITICAL_SECTION criCounter;