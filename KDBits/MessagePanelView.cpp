// MessagePanelView.cpp : 实现文件
//

#include "stdafx.h"
#include "KDBits.h"
#include "MessagePanelView.h"


// CMessagePanelView

IMPLEMENT_DYNCREATE(CMessagePanelView, CView)

CMessagePanelView::CMessagePanelView() : m_Sheet(L"")
{

}

CMessagePanelView::~CMessagePanelView()
{
}

BEGIN_MESSAGE_MAP(CMessagePanelView, CView)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CMessagePanelView 绘图

void CMessagePanelView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 在此添加绘制代码
}


// CMessagePanelView 诊断

#ifdef _DEBUG
void CMessagePanelView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CMessagePanelView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMessagePanelView 消息处理程序

void CMessagePanelView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	// TODO: 在此添加专用代码和/或调用基类
	m_Sheet.Create(this, WS_CHILD | WS_VISIBLE, 0);
	//使选项卡的按钮在下面
	::SetWindowLong(m_Sheet.GetTabControl()->m_hWnd, GWL_STYLE, ::GetWindowLong(m_Sheet.GetTabControl()->m_hWnd, GWL_STYLE)|TCS_BOTTOM);
	if(m_Sheet.m_hWnd)
		m_Sheet.ShowWindow(SW_MAXIMIZE);//显示选项卡
}

void CMessagePanelView::OnSize(UINT nType, int cx, int cy)
{
	// TODO: 在此处添加消息处理程序代码
	ShowWindow(SW_MAXIMIZE);
	CWnd *psheet = GetWindow(GW_CHILD);//m_sheet是resultview的唯一顶层子窗口
	if(psheet != NULL)
	{
		CRect rect;
		GetWindowRect(&rect);
		psheet->ScreenToClient(&rect);
		psheet->SetWindowPos(NULL, 0, 0, rect.Width(), rect.Height(), SWP_NOZORDER|SWP_NOMOVE|SWP_NOACTIVATE);
		CWnd * ptab = m_Sheet.GetTabControl();
		if(ptab != NULL)
		{
			ptab->ScreenToClient(&rect);
			ptab->SetWindowPos(NULL, 0, 0, rect.Width(), rect.Height(), SWP_NOZORDER|SWP_NOACTIVATE);
			//让PropertySheet自动调整page的大小
			m_Sheet.SetActivePage(m_Sheet.GetActiveIndex());
		}
	}
}
