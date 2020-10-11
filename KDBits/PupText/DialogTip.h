////////////////////////////////////////////////////////////////
// Visual C++ 6.0 环境编译, Windows 98 和 NT 环境运行.
//
#pragma once

#include "subclass.h"
#include "puptext.h"

//////////////////
// Generic tip-handler to display tip for wide text in a list box.
// To use:
// - instantiate one of these for each list box
// - call Init
//
class CDialogTipHandler : public CSubclassWnd 
{
protected:
	static CPopupText g_wndTip;			 // THE tip window
	// subclass window proc
	virtual LRESULT WindowProc(UINT msg, WPARAM wp, LPARAM lp);
	// virtual fns you can override
	virtual void OnMouseMove(CPoint p);
	BOOL bTrackLeave;
	CString m_strTip;

public:
	CDialogTipHandler();
	~CDialogTipHandler();
	void Init(CWnd* pDialog, UINT uID, CString strTip);				 // initialize
};
