// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "KDBits.h"

#include "MainFrm.h"
#include "KDBitsDoc.h"
#include "KDBitsView.h"
#include "MessagePanelView.h"
#include "OpPropertySheet\OpPropertySheet.h"
#include "ConfigDisk.h"
#include "ConfigNetwork.h"
#include "ConfigPath.h"
#include "PreDownloadDlg.h"
#include "DownloadList.h"
#include "configuration.h"
#include "MakeTorrentFileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_MAIN_MENU_CONFIG, &CMainFrame::OnMainMenuConfig)
	ON_COMMAND(ID_MAIN_MENU_NEW_TASK, &CMainFrame::OnMainMenuNewTask)
	ON_COMMAND(ID_MAIN_MENU_MAKEFILE, &CMainFrame::OnMainMenuMakefile)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame 构造/析构

CMainFrame::CMainFrame()
: m_CreatedOK(0)
{
	// TODO: 在此添加成员初始化代码
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD|WS_VISIBLE|CBRS_TOP|CBRS_GRIPPER|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}

	m_wndToolBar.LoadTrueColorToolBar(48, IDB_NORMAL, IDB_NORMAL, IDB_DISABLE);

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}

	// TODO: 如果不需要工具栏可停靠，则删除这三行
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	cs.dwExStyle |= WS_EX_ACCEPTFILES;

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		 | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE | WS_SYSMENU;

	return TRUE;
}


// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame 消息处理程序

void CMainFrame::OnUpdateFrameTitle(BOOL bAddToTitle)
{
	// TODO: 在此添加专用代码和/或调用基类
	WCHAR strBuffer[MAX_PATH];
	LoadString(NULL, IDS_STRING_APPLICATION_TITLE, strBuffer, sizeof(strBuffer));
	SetWindowText(strBuffer);
	//CFrameWnd::OnUpdateFrameTitle(bAddToTitle);
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: 在此添加专用代码和/或调用基类
	m_CreatedOK = m_HSplitter.CreateStatic(this, 2, 1);
	if(m_CreatedOK)
	{
		m_CreatedOK = m_HSplitter.CreateView(0, 0, RUNTIME_CLASS(CKDBitsView), CSize(0, 500), pContext);
		if(m_CreatedOK)
		{
			m_CreatedOK = m_HSplitter.CreateView(1, 0, RUNTIME_CLASS(CMessagePanelView), CSize(0, 0), pContext);
			setLayout();
		}
	}
	return m_CreatedOK;
}

void CMainFrame::setLayout()
{
	if(m_CreatedOK)
	{
		if(m_HSplitter.m_hWnd)
		{
			int cy0, cy1, cymin, cy;
			m_HSplitter.GetRowInfo(0, cy0, cymin);
			m_HSplitter.GetRowInfo(1, cy1, cymin);
			cy = cy0 + cy1;
			int minCy0 = (int)(cy * 0.75);
			if(cy0 != minCy0)
			{
				cy0 = minCy0;
				m_HSplitter.SetRowInfo(0, cy0 , cymin);
			}
			m_HSplitter.RecalcLayout();
		}
	}
}

void CMainFrame::OnMainMenuConfig()
{
	// TODO: 在此添加命令处理程序代码
	CConfigNetwork dlgConfigNetwork;
	CConfigDisk    dlgConfigDisk;
	CConfigPath    dlgConfigPath;
	COpPropertySheet myps(_T("系统配置"),this,0);
	myps.AddPage(&dlgConfigDisk);
	myps.AddPage(&dlgConfigNetwork);
	myps.AddPage(&dlgConfigPath);
	myps.AddIcon(::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME)));
	myps.AddIcon(::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_KDBitsTYPE)));
	myps.AddIcon(::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_KDBitsTYPE)));
	myps.DoModal();
}

__inline bool ShowPreDownloadDialog(torrent_info & t, PreDownloadProperty & PreDownProperty)
{
	CPreDownloadDlg PreDownloadDlg(NULL, t, PreDownProperty);
	if(PreDownloadDlg.DoModal() != IDOK)
	{
		return false;
	}
	return true;
}

void CMainFrame::OnMainMenuNewTask()
{
	// TODO: 在此添加命令处理程序代
	CFileDialog FileDlg(TRUE, NULL, NULL, NULL, L"种子文件 (*.torrent)|*.torrent|所有文件 (*.*)|*.*||", NULL);
	FileDlg.m_ofn.lpstrTitle = L"请选择种子文件";
	if(FileDlg.DoModal() == IDOK)
	{
		try
		{
			path FilePath = wcstombs(wstring(FileDlg.GetPathName().GetBuffer()));
			PreDownloadProperty PreDownProperty = {g_Configuration.OtherPath, -1, -1, g_Configuration.RunSetup, g_Configuration.PreAlloc, true, g_Configuration.MaxTaskNum};
			g_DownloadList.AddTask(FilePath, PreDownProperty, bind(&ShowPreDownloadDialog, _1, _2), &AsynWaitHandler);
		}
		catch (wstring & strMsg)
		{
			AfxMessageBox(strMsg.c_str(), MB_ICONERROR);
		}
		catch (libtorrent::invalid_encoding&)
		{
			AfxMessageBox(L"无效的种子文件", MB_ICONERROR);
		}
		catch (boost::filesystem::filesystem_error&)
		{
			AfxMessageBox(L"文件操作意外失败", MB_ICONERROR);
		}
		catch (libtorrent::duplicate_torrent&)
		{
			AfxMessageBox(L"已经存在相同的下载项目", MB_ICONERROR);
		}
		catch (...)
		{
			AfxMessageBox(L"未知错误", MB_ICONERROR);
		}
	}
}

void CMainFrame::OnMainMenuMakefile()
{
	// TODO: 在此添加命令处理程序代码
	CMakeTorrentFileDlg Dlg;
	Dlg.DoModal();
}

void CMainFrame::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	UINT nFiles = ::DragQueryFile(hDropInfo, (UINT)-1, NULL, 0);
	CWinApp* pApp = AfxGetApp();
	ASSERT(pApp != NULL);
	for (UINT iFile = 0; iFile < nFiles; iFile++)
	{
		WCHAR szFileName[_MAX_PATH];
		::DragQueryFile(hDropInfo, iFile, szFileName, _MAX_PATH);
		wpath pathName(szFileName);
		if (boost::filesystem::is_directory(pathName))
		{
			
		}
		else
		{
		//	if (pathName.leaf() == L"torrent")
			{


			}
//			else
			{


			}
		}
		break;	//只处理一个对象
	}
	::DragFinish(hDropInfo);
}
