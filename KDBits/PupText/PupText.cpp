////////////////////////////////////////////////////////////////
// VCKBASE -- June 2001 
// Visual C++ 6.0 环境编译, Windows 98 和 NT 环境运行.
//
#include "stdafx.h"
#include "puptext.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CPopupText,CWnd)
BEGIN_MESSAGE_MAP(CPopupText,CWnd)
	ON_WM_NCHITTEST()
	ON_WM_PAINT()
	ON_MESSAGE(WM_SETTEXT, OnSetText)
	ON_WM_TIMER()
END_MESSAGE_MAP()

CPopupText::CPopupText()
{
	m_szMargins = CSize(4,4);
}

CPopupText::~CPopupText()
{
}

//////////////////
// Create window. pt is upper left or upper right corner depending on nStyle.
//
int CPopupText::Create(CPoint pt, CWnd* pParentWnd, UINT nStyle, UINT nID)
{
	m_nStyle = nStyle;
	return CreateEx(0,
		NULL,
		NULL,
		WS_POPUP|WS_VISIBLE,
		CRect(pt,CSize(0,0)),
		pParentWnd,
		nID);
}

LRESULT CPopupText::OnNcHitTest(CPoint pt)
{
	if (m_nStyle & PTS_TRANSPARENT)		 // transparent?
		return HTTRANSPARENT;				 // ..make it so
	return CWnd::OnNcHitTest(pt);			 // otherwise return default
}

//////////////////
// text changed: resize window to fit
//
LRESULT CPopupText::OnSetText(WPARAM wp, LPARAM lp)
{
	CRect rc;
	GetWindowRect(&rc);
	int x = (m_nStyle & PTS_JUSTIFYRIGHT) ? rc.right : rc.left;
	int y = rc.top;
	//CClientDC dc = this;
	CClientDC dc(this);
	DrawText(dc, CString((LPCTSTR)lp), rc, DT_CALCRECT);
	rc.InflateRect(m_szMargins);
	if (m_nStyle & PTS_JUSTIFYRIGHT) {
		x -= rc.Width();
	}
	SetWindowPos(NULL,x,y,rc.Width(),rc.Height(),SWP_NOZORDER|SWP_NOACTIVATE);
	return Default();
}

void CPopupText::DrawText(CDC& dc, LPCTSTR lpText, CRect& rc, UINT flags)
{
	CBrush b(GetSysColor(COLOR_INFOBK)); // use tooltip bg color
	dc.FillRect(&rc, &b);
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(GetSysColor(COLOR_INFOTEXT)); // tooltip text color
	CFont* pOldFont = dc.SelectObject(GetParent()->GetFont());
	dc.DrawText(lpText, &rc, flags);
	dc.SelectObject(pOldFont);
}

//////////////////
// Paint text using system colors
//
void CPopupText::OnPaint()
{
	CRect rc;
	GetClientRect(&rc);
	CString s;
	GetWindowText(s);
	CPaintDC dc(this);
	DrawText(dc, s, rc, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
}

//////////////////
// Register class if needed
//
BOOL CPopupText::PreCreateWindow(CREATESTRUCT& cs) 
{
	static CString sClassName;
	if (sClassName.IsEmpty())
		sClassName = AfxRegisterWndClass(0);
	cs.lpszClass = sClassName;
	cs.style = WS_POPUP|WS_BORDER;
	cs.dwExStyle |= WS_EX_TOOLWINDOW;
	return CWnd::PreCreateWindow(cs);
}

//////////////////
// CPopupText is intended to be used on the stack,
// not heap, so don't auto-delete.
//
void CPopupText::PostNcDestroy()
{
	// don't delete this
}

//////////////////
// Show window with delay. No delay means show now.
//
void CPopupText::ShowDelayed(UINT msec)
{
	if (msec==0) {
		// no delay: show it now
		OnTimer(1);
	} else {
		// delay: set time
		SetTimer(1, msec, NULL);
	}
}

//////////////////
// Cancel text: kill timer and hide window
//
void CPopupText::Cancel()
{
	KillTimer(1);
	ShowWindow(SW_HIDE);
}

//////////////////
// Timer popped: display myself and kill timer
//
void CPopupText::OnTimer(UINT nIDEvent)
{
	ShowWindow(SW_SHOWNA);
	Invalidate();
	UpdateWindow();
	KillTimer(1);
}
