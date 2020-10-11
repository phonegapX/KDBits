// ConfigPath.cpp : 实现文件
//

#include "stdafx.h"
#include "KDBits.h"
#include "ConfigPath.h"
#include "pathselectdlg.h"
#include "configuration.h"

// CConfigPath 对话框

IMPLEMENT_DYNAMIC(CConfigPath, CPropertyPage)

CConfigPath::CConfigPath()
	: CPropertyPage(CConfigPath::IDD)
	, m_NetGamePath(_T(""))
	, m_LocalGamePath(_T(""))
	, m_OtherPath(_T(""))
{

}

CConfigPath::~CConfigPath()
{
}

void CConfigPath::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NETGAME_PATH, m_NetGamePath);
	DDV_MaxChars(pDX, m_NetGamePath, 512);
	DDX_Text(pDX, IDC_LOCALGAME_PATH, m_LocalGamePath);
	DDV_MaxChars(pDX, m_LocalGamePath, 512);
	DDX_Text(pDX, IDC_OTHER_PATH, m_OtherPath);
	DDV_MaxChars(pDX, m_OtherPath, 512);
}


BEGIN_MESSAGE_MAP(CConfigPath, CPropertyPage)
	ON_BN_CLICKED(IDC_NETGAME_BRW, &CConfigPath::OnBnClickedNetgameBrw)
	ON_BN_CLICKED(IDC_LOCALGAME_BRW, &CConfigPath::OnBnClickedLocalgameBrw)
	ON_BN_CLICKED(IDC_OTHER_BRW, &CConfigPath::OnBnClickedOtherBrw)
END_MESSAGE_MAP()


// CConfigPath 消息处理程序

void CConfigPath::OnBnClickedNetgameBrw()
{
	// TODO: 在此添加控件通知处理程序代码
	CPathSelectDlg PathSelectDlg(this, L"请选择一个目录");
	if(PathSelectDlg.DoModal())
	{
		m_NetGamePath = PathSelectDlg.m_strPath;
		UpdateData(FALSE);
	}
}

void CConfigPath::OnBnClickedLocalgameBrw()
{
	// TODO: 在此添加控件通知处理程序代码
	CPathSelectDlg PathSelectDlg(this, L"请选择一个目录");
	if(PathSelectDlg.DoModal())
	{
		m_LocalGamePath = PathSelectDlg.m_strPath;
		UpdateData(FALSE);
	}
}

void CConfigPath::OnBnClickedOtherBrw()
{
	// TODO: 在此添加控件通知处理程序代码
	CPathSelectDlg PathSelectDlg(this, L"请选择一个目录");
	if(PathSelectDlg.DoModal())
	{
		m_OtherPath = PathSelectDlg.m_strPath;
		UpdateData(FALSE);
	}
}

BOOL CConfigPath::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_NetGamePath   = g_Configuration.NetGamePath.native_directory_string().c_str();

	m_LocalGamePath = g_Configuration.LocalGamePath.native_directory_string().c_str();

	m_OtherPath     = g_Configuration.OtherPath.native_directory_string().c_str();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CConfigPath::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	g_Configuration.NetGamePath   = wcstombs(wstring(m_NetGamePath.GetBuffer()));
	g_Configuration.LocalGamePath = wcstombs(wstring(m_LocalGamePath.GetBuffer()));
	g_Configuration.OtherPath     = wcstombs(wstring(m_OtherPath.GetBuffer())); 

	if(!g_Configuration.Save())
	{
		AfxMessageBox(L"无法保存设置到配置文件", MB_ICONERROR);
		return;
	}

	CPropertyPage::OnOK();
}
