// ConfigNetwork.cpp : 实现文件
//

#include "stdafx.h"
#include "KDBits.h"
#include "ConfigNetwork.h"
#include "configuration.h"
#include "DownloadList.h"

static CString strNotLimit = L"无限制";
// CConfigNetwork 对话框

IMPLEMENT_DYNAMIC(CConfigNetwork, CPropertyPage)

CConfigNetwork::CConfigNetwork()
	: CPropertyPage(CConfigNetwork::IDD)
	, m_UpLimit(_T(""))
	, m_DownLimit(_T(""))
	, m_ListenPort(_T(""))
	, m_ProxyIP(_T(""))
	, m_ProxyPort(_T(""))
	, m_ProxyUserName(_T(""))
	, m_ProxyPassword(_T(""))
	, m_bOpenUPnP(FALSE)
	, m_bJoinDHT(FALSE)
{
}

CConfigNetwork::~CConfigNetwork()
{
}

void CConfigNetwork::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_UPLOADLIMIT, m_UpLimit);
	DDV_MaxChars(pDX, m_UpLimit, 4);
	DDX_Text(pDX, IDC_DOWNLIMIT, m_DownLimit);
	DDV_MaxChars(pDX, m_DownLimit, 4);
	DDX_Text(pDX, IDC_LISTEN_PORT, m_ListenPort);
	DDV_MaxChars(pDX, m_ListenPort, 5);
	DDX_Text(pDX, IDC_PROXY_IP_EDIT, m_ProxyIP);
	DDV_MaxChars(pDX, m_ProxyIP, 15);
	DDX_Text(pDX, IDC_PROXY_PORT_EDIT, m_ProxyPort);
	DDV_MaxChars(pDX, m_ProxyPort, 5);
	DDX_Text(pDX, IDC_PROXY_USERNAME_EDIT, m_ProxyUserName);
	DDV_MaxChars(pDX, m_ProxyUserName, 32);
	DDX_Text(pDX, IDC_PROXY_PASSWORD_EDIT, m_ProxyPassword);
	DDV_MaxChars(pDX, m_ProxyPassword, 32);
	DDX_Control(pDX, IDC_UPLOADLIMIT_SPIN, m_UpLimitSpin);
	DDX_Control(pDX, IDC_DOWNLIMIT_SPIN, m_DownLimitSpin);
	DDX_Control(pDX, IDC_LISTEN_PORT_SPIN, m_ListenPortSpin);
	DDX_Check(pDX, IDC_OPEN_UPNP_CHECK, m_bOpenUPnP);
	DDX_Check(pDX, IDC_JOIN_DHT_CHECK, m_bJoinDHT);
	DDX_Control(pDX, IDC_PROXY_TYPE_COMBO, m_cbProxType);
}

BEGIN_MESSAGE_MAP(CConfigNetwork, CPropertyPage)
	ON_EN_CHANGE(IDC_UPLOADLIMIT, &CConfigNetwork::OnEnChangeUploadlimit)
	ON_EN_CHANGE(IDC_DOWNLIMIT, &CConfigNetwork::OnEnChangeDownlimit)
	ON_EN_CHANGE(IDC_LISTEN_PORT, &CConfigNetwork::OnEnChangeListenPort)
	ON_EN_CHANGE(IDC_PROXY_PORT_EDIT, &CConfigNetwork::OnEnChangeProxyPortEdit)
	ON_CBN_SELCHANGE(IDC_PROXY_TYPE_COMBO, &CConfigNetwork::OnCbnSelchangeProxyTypeCombo)
	ON_BN_CLICKED(IDC_PROXY_IS_VALIDATE_CHECK, &CConfigNetwork::OnBnClickedProxyIsValidateCheck)
	ON_EN_KILLFOCUS(IDC_PROXY_IP_EDIT, &CConfigNetwork::OnEnKillfocusProxyIpEdit)
END_MESSAGE_MAP()

BOOL CConfigNetwork::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	// TODO:  在此添加额外的初始化
	m_cbProxType.InsertString(0, L"No Proxy");
	m_cbProxType.InsertString(1, L"HTTP 1.1");
	m_cbProxType.SetCurSel(0);

	m_UpLimitSpin.SetRange(0, 9999);
	m_DownLimitSpin.SetRange(0, 9999);
	m_ListenPortSpin.SetRange32(0, 65535);

	string Temp;

	Temp = boost::lexical_cast<string>(g_Configuration.UpLimit);
	m_UpLimit       = Temp.c_str();

	Temp = boost::lexical_cast<string>(g_Configuration.DownLimit);
	m_DownLimit     = Temp.c_str();

	Temp = boost::lexical_cast<string>(g_Configuration.ListenPort);
	m_ListenPort    = Temp.c_str();

	m_bOpenUPnP     = g_Configuration.OpenUPnP;

	m_bJoinDHT      = g_Configuration.JoinDHT;

	m_ProxyIP       = g_Configuration.ProxyIP.c_str();

	m_ProxyPort     = g_Configuration.ProxyPort.c_str();

	m_ProxyUserName = g_Configuration.ProxyUserName.c_str();

	m_ProxyPassword = g_Configuration.ProxyPassword.c_str();

	UpdateData(FALSE);

	if (m_ProxyIP == "")
		m_cbProxType.SetCurSel(0);
	else
		m_cbProxType.SetCurSel(1);

	if (m_ProxyUserName != "")
		CheckDlgButton(IDC_PROXY_IS_VALIDATE_CHECK, BST_CHECKED);
	else
		CheckDlgButton(IDC_PROXY_IS_VALIDATE_CHECK, BST_UNCHECKED);

	OnEnChangeUploadlimit();
	OnEnChangeDownlimit();
	OnEnChangeListenPort();
	OnEnChangeProxyPortEdit();

	OnBnClickedProxyIsValidateCheck();

	OnCbnSelchangeProxyTypeCombo();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CConfigNetwork::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	UpdateData(TRUE);

	try
	{
		g_Configuration.UpLimit = boost::lexical_cast<int>(m_UpLimit.GetBuffer());
	}
	catch (boost::bad_lexical_cast &)
	{
		g_Configuration.UpLimit = -1;
	}

	try
	{
		g_Configuration.DownLimit = boost::lexical_cast<int>(m_DownLimit.GetBuffer());
	}
	catch (boost::bad_lexical_cast &)
	{
		g_Configuration.DownLimit = -1;
	}

	try
	{
		g_Configuration.ListenPort = boost::lexical_cast<USHORT>(m_ListenPort.GetBuffer());
	}
	catch (boost::bad_lexical_cast &)
	{
		g_Configuration.ListenPort = 19825;
	}

	g_Configuration.OpenUPnP      = m_bOpenUPnP;
	g_Configuration.JoinDHT       = m_bJoinDHT;
	g_Configuration.ProxyIP       = wcstombs(wstring(m_ProxyIP.GetBuffer()));
	g_Configuration.ProxyPort     = wcstombs(wstring(m_ProxyPort.GetBuffer()));
	g_Configuration.ProxyUserName = wcstombs(wstring(m_ProxyUserName.GetBuffer()));
	g_Configuration.ProxyPassword = wcstombs(wstring(m_ProxyPassword.GetBuffer()));

	if(!g_Configuration.Save())
	{
		AfxMessageBox(L"无法保存设置到配置文件", MB_ICONERROR);
		return;
	}
	/****************************************************************/
	g_DownloadList.SetTransferSpeed(g_Configuration.UpLimit, g_Configuration.DownLimit);
	g_DownloadList.BindPort(g_Configuration.ListenPort, g_Configuration.ListenPort);
	/****************************************************************/
	CPropertyPage::OnOK();
}

void CConfigNetwork::OnEnChangeUploadlimit()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CPropertyPage::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	WCHAR wValue[MAX_PATH];
	HWND  hWnd = ::GetDlgItem(m_hWnd, IDC_UPLOADLIMIT);
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

void CConfigNetwork::OnEnChangeDownlimit()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CPropertyPage::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	WCHAR wValue[MAX_PATH];
	HWND  hWnd = ::GetDlgItem(m_hWnd, IDC_DOWNLIMIT);
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

void CConfigNetwork::OnEnChangeListenPort()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CPropertyPage::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	WCHAR wValue[MAX_PATH];
	HWND  hWnd = ::GetDlgItem(m_hWnd, IDC_LISTEN_PORT);
	::GetWindowText(hWnd, wValue, sizeof(wValue));
	try
	{
		if (boost::lexical_cast<int>(wValue) > 65535)
		{
			::SetWindowText(hWnd, boost::lexical_cast<wstring>(65535).c_str());
		}
	}
	catch (boost::bad_lexical_cast &)
	{
		::SetWindowText(hWnd, boost::lexical_cast<wstring>(0).c_str());
	}
}

void CConfigNetwork::OnEnChangeProxyPortEdit()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CPropertyPage::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	WCHAR wValue[MAX_PATH];
	HWND  hWnd = ::GetDlgItem(m_hWnd, IDC_PROXY_PORT_EDIT);
	::GetWindowText(hWnd, wValue, sizeof(wValue));
	try
	{
		if (boost::lexical_cast<int>(wValue) > 65535)
		{
			::SetWindowText(hWnd, boost::lexical_cast<wstring>(65535).c_str());
		}
	}
	catch (boost::bad_lexical_cast &)
	{
		::SetWindowText(hWnd, boost::lexical_cast<wstring>(0).c_str());
	}
}

void CConfigNetwork::OnCbnSelchangeProxyTypeCombo()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_cbProxType.GetCurSel() == 0)
	{
		::SetWindowText(::GetDlgItem(m_hWnd, IDC_PROXY_IP_EDIT),       L"");
		::SetWindowText(::GetDlgItem(m_hWnd, IDC_PROXY_PORT_EDIT),     L"");
		::SetWindowText(::GetDlgItem(m_hWnd, IDC_PROXY_USERNAME_EDIT), L"");
		::SetWindowText(::GetDlgItem(m_hWnd, IDC_PROXY_PASSWORD_EDIT), L"");

		::EnableWindow(::GetDlgItem(m_hWnd, IDC_PROXY_IP_EDIT),           FALSE);
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_PROXY_PORT_EDIT),         FALSE);
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_PROXY_IS_VALIDATE_CHECK), FALSE);
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_PROXY_USERNAME_EDIT),     FALSE);
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_PROXY_PASSWORD_EDIT),     FALSE);
	}
	else
	{
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_PROXY_IP_EDIT),           TRUE);
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_PROXY_PORT_EDIT),         TRUE);
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_PROXY_IS_VALIDATE_CHECK), TRUE);
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_PROXY_USERNAME_EDIT),     TRUE);
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_PROXY_PASSWORD_EDIT),     TRUE);

		OnBnClickedProxyIsValidateCheck();
	}
}

void CConfigNetwork::OnBnClickedProxyIsValidateCheck()
{
	// TODO: 在此添加控件通知处理程序代码

	if (IsDlgButtonChecked(IDC_PROXY_IS_VALIDATE_CHECK) == BST_CHECKED)
	{
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_PROXY_USERNAME_EDIT),     TRUE);
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_PROXY_PASSWORD_EDIT),     TRUE);
	}
	else
	{
		::SetWindowText(::GetDlgItem(m_hWnd, IDC_PROXY_USERNAME_EDIT), L"");
		::SetWindowText(::GetDlgItem(m_hWnd, IDC_PROXY_PASSWORD_EDIT), L"");

		::EnableWindow(::GetDlgItem(m_hWnd, IDC_PROXY_USERNAME_EDIT),     FALSE);
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_PROXY_PASSWORD_EDIT),     FALSE);
	}
}

void CConfigNetwork::OnEnKillfocusProxyIpEdit()
{
	// TODO: 在此添加控件通知处理程序代码
	CHAR szValue[MAX_PATH];
	HWND hWnd = ::GetDlgItem(m_hWnd, IDC_PROXY_IP_EDIT);
	::GetWindowTextA(hWnd, szValue, sizeof(szValue));
	string strValue(szValue);
	strValue.erase(std::remove(strValue.begin(), strValue.end(), ' '), strValue.end());
	string::size_type nCount =  std::count(strValue.begin(), strValue.end(), '.');
	if (strValue != "" && (inet_addr(strValue.c_str()) == INADDR_NONE || nCount != 3))
	{
		AfxMessageBox(L"非法IP地址,请重新输入", MB_ICONERROR);
		::SetWindowText(hWnd, L"");
		::SetFocus(hWnd);
	}
	else
	{
		::SetWindowTextA(hWnd, strValue.c_str());
	}
}
