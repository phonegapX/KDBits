// UPnpDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UPnp.h"
#include "UPnpDlg.h"
#include "UPnP\upnpasynthreads.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUPnpDlg 对话框




CUPnpDlg::CUPnpDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUPnpDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUPnpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CUPnpDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CUPnpDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CUPnpDlg 消息处理程序

BOOL CUPnpDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CUPnpDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CUPnpDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CUPnpDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	WORD wVersionRequested = MAKEWORD( 2, 2 );
	WSADATA wsaData;

	WSAStartup( wVersionRequested, &wsaData );



CUPnpNatMapping mapping;

mapping.m_wInternalPort = 23333;				// Port mapping internal port
mapping.m_wExternalPort = 33333;				// Port mapping external port
mapping.m_strProtocol = _T("TCP");// Protocol-> TCP (UPNPNAT_PROTOCOL:UNAT_TCP) || UDP (UPNPNAT_PROTOCOL:UNAT_UDP)
mapping.m_strDescription = _T("ZMX__TEST");				// Port mapping description
//mapping.m_strInternalClient = _T("192.168.1.246");

/*
	CUPnpMgr Mgr;
	Mgr.AddNATPortMapping(mapping);
*/

	static CUPnpMgr	Mgr;
	CUPnpAsynThreads::AddNatPortMappingAsyn(&Mgr, mapping, 0, 0, 0, 0, FALSE);

	AfxMessageBox(L"OK");


//	OnOK();
}
