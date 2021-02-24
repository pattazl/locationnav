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

#ifndef BROWSER_DLG_H
#define BROWSER_DLG_H

#include "DockingDlgInterface.h"
#include "resource.h"

class CBrowserDlg : public DockingDlgInterface
{
public :
	CBrowserDlg();
	~CBrowserDlg();
    virtual void display(bool toShow = true) const {
        DockingDlgInterface::display(toShow);
        if (toShow)
            ::SetFocus(::GetDlgItem(_hSelf, ID_WebBrowser_EDIT));
    };
	HWND _hEDIT,_hBUTTON,_hClear;
	void setParent(HWND parent2set);
	TCHAR URLString[1024];
	void GotoURL();
protected :
	virtual BOOL CALLBACK run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam);

private :

    int getLine() const {
        BOOL isSuccessful;
        int line = ::GetDlgItemInt(_hSelf, ID_WebBrowser_EDIT, &isSuccessful, FALSE);
        return (isSuccessful?line:-1);
    };

};

#endif //BROWSER_DLG_H
