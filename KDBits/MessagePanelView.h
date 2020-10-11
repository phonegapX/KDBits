#pragma once
#include "kdpanelsheet.h"


// CMessagePanelView 视图

class CMessagePanelView : public CView
{
	DECLARE_DYNCREATE(CMessagePanelView)

protected:
	CMessagePanelView();           // 动态创建所使用的受保护的构造函数
	virtual ~CMessagePanelView();

public:
	virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	CKDPanelSheet m_Sheet;
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
