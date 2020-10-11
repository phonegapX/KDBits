////////////////////////////////////////////////////////////////
// VCKBASE -- June 2001 
// Visual C++ 6.0 环境编译, Windows 98 和 NT 环境运行.
//
#pragma once

//////////////////
// Get NONCLIENTMETRICS info: ctor calls SystemParametersInfo.
//
class CNonClientMetrics : public NONCLIENTMETRICS {
public:
	CNonClientMetrics() {
		cbSize = sizeof(NONCLIENTMETRICS);
		SystemParametersInfo(SPI_GETNONCLIENTMETRICS,0,this,0);
	}
};

//////////////////
// Popup text window, like tooltip.
// Can be right or left justified relative to creation point.
//
class CPopupText : public CWnd {
public:
	CSize m_szMargins;		// extra space around text: change if you like
	CPopupText();
	virtual ~CPopupText();
	BOOL Create(CPoint pt, CWnd* pParentWnd, UINT nStyle=0, UINT nID=0);
	void ShowDelayed(UINT msec);
	void Cancel();

protected:
	UINT	m_nStyle;			// style (see below)

	virtual void PostNcDestroy();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	void DrawText(CDC& dc, LPCTSTR lpText, CRect& rc, UINT flags);

	afx_msg LRESULT OnNcHitTest(CPoint pt);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg LRESULT OnSetText(WPARAM wp, LPARAM lp);
	DECLARE_DYNAMIC(CPopupText);
	DECLARE_MESSAGE_MAP();
};

#define PTS_JUSTIFYLEFT  0x0000
#define PTS_JUSTIFYRIGHT 0x0001
#define PTS_TRANSPARENT  0x0002


