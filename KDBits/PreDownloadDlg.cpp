// PreDownloadDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "KDBits.h"
#include "PreDownloadDlg.h"


// CPreDownloadDlg 对话框

IMPLEMENT_DYNAMIC(CPreDownloadDlg, CDialog)

CPreDownloadDlg::CPreDownloadDlg(CWnd* pParent, torrent_info & t, PreDownloadProperty & PreDownProperty)
	: CDialog(CPreDownloadDlg::IDD, pParent), m_GeneralPage(t, PreDownProperty), m_OtherPage(t), m_SetupPage(PreDownProperty)
{
}

CPreDownloadDlg::~CPreDownloadDlg()
{
}

void CPreDownloadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPreDownloadDlg, CDialog)
END_MESSAGE_MAP()


// CPreDownloadDlg 消息处理程序

BOOL CPreDownloadDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME), FALSE);		// Set small icon

	m_Sheet.AddPage(&m_GeneralPage);
	m_Sheet.AddPage(&m_OtherPage);
	m_Sheet.AddPage(&m_SetupPage);
	m_Sheet.Create(this, WS_CHILD | WS_VISIBLE, WS_EX_CONTROLPARENT);

	m_Sheet.SetWindowPos(NULL, 5, 5, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CPreDownloadDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	if(IsWindow(m_GeneralPage)) m_GeneralPage.OnOK();
	if(IsWindow(m_OtherPage))   m_OtherPage.OnOK();
	if(IsWindow(m_SetupPage))   m_SetupPage.OnOK();

	CDialog::OnOK();
}
