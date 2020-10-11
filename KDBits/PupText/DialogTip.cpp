////////////////////////////////////////////////////////////////
// Visual C++ 6.0 环境编译, Windows 98 和 NT 环境运行.
//
#include "stdafx.h"
#include "PupText.h"
#include "DialogTip.h"

// THE popup tip window
CPopupText CDialogTipHandler::g_wndTip;

CDialogTipHandler::CDialogTipHandler() : bTrackLeave(FALSE)
{
}

CDialogTipHandler::~CDialogTipHandler()
{
}

//////////////////
// Install hook. Initialize control ID from list box and create
// (invisible) tip window.
//
void CDialogTipHandler::Init(CWnd * pDialog, UINT uID, CString strTip)
{
	HookWindow(pDialog->GetDlgItem(uID));
	m_strTip = strTip;
	if (!g_wndTip) 
	{
		// create scroll tip window
		g_wndTip.Create(CPoint(0,0), pDialog, PTS_JUSTIFYLEFT|PTS_TRANSPARENT);
	}
}

//////////////////
// "Hook" function traps messages sent to listbox/control. 
//
LRESULT CDialogTipHandler::WindowProc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg) 
	{
	case WM_MOUSEMOVE:
		OnMouseMove(CPoint(GET_X_LPARAM(lp),GET_Y_LPARAM(lp)));
		break;
	case WM_MOUSELEAVE:
		bTrackLeave = FALSE;
		g_wndTip.Cancel(); // cancel popup text if any
		break;
	}
	return CSubclassWnd::WindowProc(msg, wp, lp);
}

/////////////////
// User moved the mouse.
// Get info for listbox item under mouse:
// If text is too wide to fit in list box, prepare tip window and start
// timer to display it.
//
void CDialogTipHandler::OnMouseMove(CPoint pt)
{
	if (!bTrackLeave)
	{
		TRACKMOUSEEVENT tme = {sizeof(tme), TME_LEAVE, m_hWnd};
		_TrackMouseEvent(&tme);
		bTrackLeave = TRUE;
		CWnd * pSubWnd = (CDialog*)CWnd::FromHandle(m_hWnd);
		CRect rc;
		pSubWnd->GetClientRect(&rc);
		pSubWnd->ClientToScreen(&rc);		// text rect in screen coords
		//////////////////////////
		int iHeight = rc.Height();
		rc.top    -= iHeight;
		rc.bottom -= iHeight;
		// move tip window over list text
		g_wndTip.SetWindowPos(NULL, rc.left, rc.top, rc.Width(), rc.Height(), SWP_NOZORDER|SWP_NOACTIVATE);
		g_wndTip.SetWindowText(m_strTip);	// set tip text to that of item
		g_wndTip.ShowDelayed(0); // show popup text delayed
	}
}
