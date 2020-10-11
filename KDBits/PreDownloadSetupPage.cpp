// PreDownloadSetupPage.cpp : 实现文件
//

#include "stdafx.h"
#include "KDBits.h"
#include "PreDownloadSetupPage.h"
#include "DownloadList.h"

static CString strNotLimit = L"无限制";

// CPreDownloadSetupPage 对话框

IMPLEMENT_DYNAMIC(CPreDownloadSetupPage, CPropertyPage)

CPreDownloadSetupPage::CPreDownloadSetupPage(PreDownloadProperty & PreDownProperty)
	: CPropertyPage(CPreDownloadSetupPage::IDD)
	, m_UploadLimit(_T(""))
	, m_DownloadLimit(_T(""))
	, m_bPreAlloc(FALSE)
	, m_PreDownProperty(PreDownProperty)
{
}

CPreDownloadSetupPage::~CPreDownloadSetupPage()
{
}

void CPreDownloadSetupPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SETUP_UPLOADLIMIT, m_UploadLimit);
	DDV_MaxChars(pDX, m_UploadLimit, 4);
	DDX_Text(pDX, IDC_SETUP_DOWNLIMIT, m_DownloadLimit);
	DDV_MaxChars(pDX, m_DownloadLimit, 4);
	DDX_Control(pDX, IDC_SETUP_RUNSETUP_COMBO, m_cbRunSetup);
	DDX_Control(pDX, IDC_SETUP_UPLOADLIMIT_SPIN, m_UploadLimitSpin);
	DDX_Control(pDX, IDC_SETUP_DOWNLIMIT_SPIN, m_DownloadLimitSpin);
	DDX_Check(pDX, IDC_SETUP_PREALLOC_CHECKBOX, m_bPreAlloc);
}


BEGIN_MESSAGE_MAP(CPreDownloadSetupPage, CPropertyPage)
	ON_EN_CHANGE(IDC_SETUP_UPLOADLIMIT, &CPreDownloadSetupPage::OnEnChangeSetupUploadlimit)
	ON_EN_CHANGE(IDC_SETUP_DOWNLIMIT, &CPreDownloadSetupPage::OnEnChangeSetupDownlimit)
END_MESSAGE_MAP()


// CPreDownloadSetupPage 消息处理程序

BOOL CPreDownloadSetupPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_cbRunSetup.InsertString(down_now,  L"立刻下载");
	m_cbRunSetup.InsertString(down_late, L"稍候下载");

	m_cbRunSetup.SetCurSel(m_PreDownProperty.RunSetup);

	m_UploadLimitSpin.SetRange(0, 9999);
	m_DownloadLimitSpin.SetRange(0, 9999);

	string Temp;

	Temp = boost::lexical_cast<string>(m_PreDownProperty.UploadLimit);
	m_UploadLimit       = Temp.c_str();

	Temp = boost::lexical_cast<string>(m_PreDownProperty.DownloadLimit);
	m_DownloadLimit     = Temp.c_str();

	m_bPreAlloc = m_PreDownProperty.bPreAlloc;

	UpdateData(FALSE);

	OnEnChangeSetupUploadlimit();
	OnEnChangeSetupDownlimit();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CPreDownloadSetupPage::OnEnChangeSetupUploadlimit()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CPropertyPage::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	WCHAR wValue[MAX_PATH];
	HWND  hWnd = ::GetDlgItem(m_hWnd, IDC_SETUP_UPLOADLIMIT);
	::GetWindowText(hWnd, wValue, sizeof(wValue));
	try
	{
		if (boost::lexical_cast<int>(wValue) <= 0)
		{
			::SetWindowText(hWnd, strNotLimit);
		}
	}
	catch (boost::bad_lexical_cast &)
	{
		if (wValue != strNotLimit)
		{
			::SetWindowText(hWnd, strNotLimit);
		}
	}
}

void CPreDownloadSetupPage::OnEnChangeSetupDownlimit()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CPropertyPage::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	WCHAR wValue[MAX_PATH];
	HWND  hWnd = ::GetDlgItem(m_hWnd, IDC_SETUP_DOWNLIMIT);
	::GetWindowText(hWnd, wValue, sizeof(wValue));
	try
	{
		if (boost::lexical_cast<int>(wValue) <= 0)
		{
			::SetWindowText(hWnd, strNotLimit);
		}
	}
	catch (boost::bad_lexical_cast &)
	{
		if (wValue != strNotLimit)
		{
			::SetWindowText(hWnd, strNotLimit);
		}
	}
}

void CPreDownloadSetupPage::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	UpdateData(TRUE);
	
	try
	{
		m_PreDownProperty.UploadLimit = boost::lexical_cast<int>(m_UploadLimit.GetBuffer());
	}
	catch (boost::bad_lexical_cast &)
	{
		m_PreDownProperty.UploadLimit = -1;
	}

	try
	{
		m_PreDownProperty.DownloadLimit = boost::lexical_cast<int>(m_DownloadLimit.GetBuffer());
	}
	catch (boost::bad_lexical_cast &)
	{
		m_PreDownProperty.DownloadLimit = -1;
	}

	m_PreDownProperty.RunSetup = static_cast<DOWN_SETUP>(m_cbRunSetup.GetCurSel());

	m_PreDownProperty.bPreAlloc = m_bPreAlloc;

	CPropertyPage::OnOK();
}
