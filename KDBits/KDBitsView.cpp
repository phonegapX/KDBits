// KDBitsView.cpp : CKDBitsView 类的实现
//

#include "stdafx.h"
#include "KDBits.h"

#include "KDBitsDoc.h"
#include "KDBitsView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKDBitsView

IMPLEMENT_DYNCREATE(CKDBitsView, CView)

BEGIN_MESSAGE_MAP(CKDBitsView, CView)
	ON_WM_SIZE()
	ON_COMMAND(ID_MAIN_MENU_LOAD_GAMELIST, &CKDBitsView::OnMainMenuLoadGamelist)
	ON_COMMAND(ID_MENU_DOWNLIST_START_TASK, &CKDBitsView::OnMenuDownlistStartTask)
	ON_COMMAND(ID_MENU_DOWNLIST_STOP_TASK, &CKDBitsView::OnMenuDownlistStopTask)
	ON_COMMAND(ID_MENU_DOWNLIST_DELETE_TASK, &CKDBitsView::OnMenuDownlistDeleteTask)
	ON_COMMAND(ID_MAIN_MENU_START_ALL, &CKDBitsView::OnMainMenuStartAll)
	ON_COMMAND(ID_MAIN_MENU_STOP_ALL, &CKDBitsView::OnMainMenuStopAll)
	ON_COMMAND(ID_MAIN_MENU_DELETE_ALL, &CKDBitsView::OnMainMenuDeleteAll)
END_MESSAGE_MAP()

// CKDBitsView 构造/析构

CKDBitsView::CKDBitsView() : m_sheet(L"")
{
	// TODO: 在此处添加构造代码

}

CKDBitsView::~CKDBitsView()
{
}

BOOL CKDBitsView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CKDBitsView 绘制

void CKDBitsView::OnDraw(CDC* /*pDC*/)
{
	CKDBitsDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CKDBitsView 诊断

#ifdef _DEBUG
void CKDBitsView::AssertValid() const
{
	CView::AssertValid();
}

void CKDBitsView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKDBitsDoc* CKDBitsView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKDBitsDoc)));
	return (CKDBitsDoc*)m_pDocument;
}
#endif //_DEBUG


// CKDBitsView 消息处理程序

void CKDBitsView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	// TODO: 在此添加专用代码和/或调用基类
	m_sheet.Create(this, WS_CHILD | WS_VISIBLE, 0);
	if(m_sheet.m_hWnd)
		m_sheet.ShowWindow(SW_MAXIMIZE);//显示选项卡
}

void CKDBitsView::OnSize(UINT nType, int cx, int cy)
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
		CWnd * ptab = m_sheet.GetTabControl();
		if(ptab != NULL)
		{
			ptab->ScreenToClient(&rect);
			ptab->SetWindowPos(NULL, 0, 0, rect.Width(), rect.Height(), SWP_NOZORDER|SWP_NOACTIVATE);
			//让PropertySheet自动调整page的大小
			m_sheet.SetActivePage(m_sheet.GetActiveIndex());
		}
	}
}

void CKDBitsView::OnMainMenuLoadGamelist()
{
	// TODO: 在此添加命令处理程序代码
	m_sheet.ReloadGameList();
}

void CKDBitsView::OnMenuDownlistStartTask()
{
	// TODO: 在此添加命令处理程序代码
	if (::IsWindow(m_sheet.m_DownloadListPage))
	{
		m_sheet.m_DownloadListPage.SendMessage(WM_COMMAND, ID_MENU_DOWNLIST_START_TASK);
	}
}

void CKDBitsView::OnMenuDownlistStopTask()
{
	// TODO: 在此添加命令处理程序代码
	if (::IsWindow(m_sheet.m_DownloadListPage))
	{
		m_sheet.m_DownloadListPage.SendMessage(WM_COMMAND, ID_MENU_DOWNLIST_STOP_TASK);
	}
}

void CKDBitsView::OnMenuDownlistDeleteTask()
{
	// TODO: 在此添加命令处理程序代码
	if (::IsWindow(m_sheet.m_DownloadListPage))
	{
		m_sheet.m_DownloadListPage.SendMessage(WM_COMMAND, ID_MENU_DOWNLIST_DELETE_TASK);
	}
}

void CKDBitsView::OnMainMenuStartAll()
{
	// TODO: 在此添加命令处理程序代码
	m_sheet.m_DownloadListPage.EventStartAllTask();
}

void CKDBitsView::OnMainMenuStopAll()
{
	// TODO: 在此添加命令处理程序代码
	m_sheet.m_DownloadListPage.EventStopAllTask();
}

void CKDBitsView::OnMainMenuDeleteAll()
{
	// TODO: 在此添加命令处理程序代码
	m_sheet.m_DownloadListPage.EventDeleteAllTask();
}
