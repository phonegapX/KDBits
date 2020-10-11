// ConfigDisk.cpp : 实现文件
//

#include "stdafx.h"
#include "KDBits.h"
#include "ConfigDisk.h"
#include "DownloadList.h"
#include "configuration.h"

// CConfigDisk 对话框

IMPLEMENT_DYNAMIC(CConfigDisk, CPropertyPage)

CConfigDisk::CConfigDisk()
	: CPropertyPage(CConfigDisk::IDD)
	, m_bPreAlloc(FALSE)
	, m_bDelFileThenDelTask(FALSE)
	, m_bDefaultClient(FALSE)
	, m_bAutoRun(FALSE)
	, m_bAutoUpdateNewVersion(FALSE)
	, m_bReCheckDataCalcSum(FALSE)
	, m_strMaxTaskNum(_T(""))
{
}

CConfigDisk::~CConfigDisk()
{
}

void CConfigDisk::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CONFIGDISK_PREALLOC_CHECKBOX, m_bPreAlloc);
	DDX_Check(pDX, IDC_CONFIGDISK_DELFILE_CHECKBOX, m_bDelFileThenDelTask);
	DDX_Check(pDX, IDC_CONFIGDISK_DEFACLIENT_CHECKBOX, m_bDefaultClient);
	DDX_Check(pDX, IDC_CONFIGDISK_AUTORUN_CHECKBOX, m_bAutoRun);
	DDX_Check(pDX, IDC_CONFIGDISK_CHECKUPDATE_CHECKBOX, m_bAutoUpdateNewVersion);
	DDX_Check(pDX, IDC_CONFIGDISK_CHECKDATACALESUM_CHECKBOX, m_bReCheckDataCalcSum);
	DDX_Text(pDX, IDC_CONFIGDISK_MAXTASK_EDIT, m_strMaxTaskNum);
	DDV_MaxChars(pDX, m_strMaxTaskNum, 2);
	DDX_Control(pDX, IDC_CONFIGDISK_MAXTASK_SPIN, m_MaxTaskSpin);
	DDX_Control(pDX, IDC_CONFIGDISK_RUNSETUP_COMBO, m_cbRunSetup);
	DDX_Control(pDX, IDC_CONFIGDISK_PROTECTSIZE_COMBO, m_cbProtectDiskSize);
	DDX_Control(pDX, IDC_CONFIGDISK_DISKCACHE_COMBO, m_cbDiskCacheSize);
}


BEGIN_MESSAGE_MAP(CConfigDisk, CPropertyPage)
	ON_EN_CHANGE(IDC_CONFIGDISK_MAXTASK_EDIT, &CConfigDisk::OnEnChangeConfigdiskMaxtaskEdit)
END_MESSAGE_MAP()


// CConfigDisk 消息处理程序

BOOL CConfigDisk::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	// TODO:  在此添加额外的初始化
	m_cbRunSetup.InsertString(down_now,  L"立刻下载");
	m_cbRunSetup.InsertString(down_late, L"稍候下载");

	m_MaxTaskSpin.SetRange(1, 10);

	m_cbProtectDiskSize.InsertString(0,  L"未设置");
	m_cbProtectDiskSize.SetItemData (0, 0);
	m_cbProtectDiskSize.InsertString(1,  L"200  MB");
	m_cbProtectDiskSize.SetItemData (1, 200);
	m_cbProtectDiskSize.InsertString(2,  L"500  MB");
	m_cbProtectDiskSize.SetItemData (2, 500);
	m_cbProtectDiskSize.InsertString(3,  L"1000 MB");
	m_cbProtectDiskSize.SetItemData (3, 1000);
	m_cbProtectDiskSize.InsertString(4,  L"2000 MB");
	m_cbProtectDiskSize.SetItemData (4, 2000);
	m_cbProtectDiskSize.InsertString(5,  L"3000 MB");
	m_cbProtectDiskSize.SetItemData (5, 3000);

	m_cbDiskCacheSize.InsertString(0,  L"未设置");
	m_cbDiskCacheSize.SetItemData (0, 0);
	m_cbDiskCacheSize.InsertString(1,  L"512  KB");
	m_cbDiskCacheSize.SetItemData (1, 512);
	m_cbDiskCacheSize.InsertString(2,  L"1024 KB");
	m_cbDiskCacheSize.SetItemData (2, 1024);
	m_cbDiskCacheSize.InsertString(3,  L"2048 KB");
	m_cbDiskCacheSize.SetItemData (3, 2048);
	m_cbDiskCacheSize.InsertString(4,  L"4096 KB");
	m_cbDiskCacheSize.SetItemData (4, 4096);
	m_cbDiskCacheSize.InsertString(5,  L"8192 KB");
	m_cbDiskCacheSize.SetItemData (5, 8192);

	m_cbRunSetup.SetCurSel(g_Configuration.RunSetup);

	switch(g_Configuration.ProtectDiskSize)
	{
	case 0:
		m_cbProtectDiskSize.SetCurSel(0);
		break;
	case 200:
		m_cbProtectDiskSize.SetCurSel(1);
		break;
	case 500:
		m_cbProtectDiskSize.SetCurSel(2);
		break;
	case 1000:
		m_cbProtectDiskSize.SetCurSel(3);
		break;
	case 2000:
		m_cbProtectDiskSize.SetCurSel(4);
		break;
	case 3000:
		m_cbProtectDiskSize.SetCurSel(5);
		break;
	default:
		m_cbProtectDiskSize.SetCurSel(0);
	    break;
	}

	switch(g_Configuration.DiskCacheSize)
	{
	case 0:
		m_cbDiskCacheSize.SetCurSel(0);
		break;
	case 512:
		m_cbDiskCacheSize.SetCurSel(1);
		break;
	case 1024:
		m_cbDiskCacheSize.SetCurSel(2);
		break;
	case 2048:
		m_cbDiskCacheSize.SetCurSel(3);
		break;
	case 4096:
		m_cbDiskCacheSize.SetCurSel(4);
		break;
	case 8192:
		m_cbDiskCacheSize.SetCurSel(5);
		break;
	default:
		m_cbDiskCacheSize.SetCurSel(0);
		break;
	}

	m_bPreAlloc             = g_Configuration.PreAlloc;
	m_bDelFileThenDelTask   = g_Configuration.DelFileThenDelTask;
	m_bDefaultClient        = g_Configuration.DefaultClient;
	m_bAutoRun              = g_Configuration.AutoRun;
	m_bAutoUpdateNewVersion = g_Configuration.AutoUpdateNewVersion;
	m_bReCheckDataCalcSum   = g_Configuration.ReCheckDataCalcSum;
	m_strMaxTaskNum         = CString(boost::lexical_cast<string>(g_Configuration.MaxTaskNum).c_str());

	UpdateData(FALSE);
/***************************************************************************/

	m_ReCheckDataCalcSumTip.Init(this, IDC_STATIC_CHECKDATACALESUM, L"此选项不钩选将跳过断点续传时的数据校验过程,从而加快启动速度");

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CConfigDisk::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	if (m_cbRunSetup.GetCurSel() != CB_ERR)
	{
		g_Configuration.RunSetup = static_cast<DOWN_SETUP>(m_cbRunSetup.GetCurSel());
	}
	else
	{
		g_Configuration.RunSetup = down_now;
	}

	if (m_cbProtectDiskSize.GetCurSel() != CB_ERR)
	{
		g_Configuration.ProtectDiskSize = m_cbProtectDiskSize.GetItemData(m_cbProtectDiskSize.GetCurSel());
	}
	else
	{
		g_Configuration.ProtectDiskSize = 0;
	}

	if (m_cbDiskCacheSize.GetCurSel() != CB_ERR)
	{
		g_Configuration.DiskCacheSize = m_cbDiskCacheSize.GetItemData(m_cbDiskCacheSize.GetCurSel());
	}
	else
	{
		g_Configuration.DiskCacheSize = 0;
	}

	UpdateData(TRUE);

	g_Configuration.PreAlloc             = m_bPreAlloc;
	g_Configuration.DelFileThenDelTask   = m_bDelFileThenDelTask;
	g_Configuration.DefaultClient        = m_bDefaultClient;
	g_Configuration.AutoRun              = m_bAutoRun;
	g_Configuration.AutoUpdateNewVersion = m_bAutoUpdateNewVersion;
	g_Configuration.ReCheckDataCalcSum   = m_bReCheckDataCalcSum;

	try
	{
		g_Configuration.MaxTaskNum = boost::lexical_cast<int>(m_strMaxTaskNum.GetBuffer());
	}
	catch (boost::bad_lexical_cast &)
	{
		g_Configuration.MaxTaskNum = 5;
	}

	if(!g_Configuration.Save())
	{
		AfxMessageBox(L"无法保存设置到配置文件", MB_ICONERROR);
		return;
	}
	/**************************************************************/
	PreDownloadProperty DownProperty = 
	{
		path(), 
		0, 
		0, 
		g_Configuration.RunSetup, 
		g_Configuration.PreAlloc, 
		g_Configuration.ReCheckDataCalcSum, 
		0, 
		UPDATE_RULE()
	};
	g_DownloadList.SetDownloadProperty(DownProperty);
	/**************************************************************/
	CPropertyPage::OnOK();
}

void CConfigDisk::OnEnChangeConfigdiskMaxtaskEdit()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CPropertyPage::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	WCHAR wValue[MAX_PATH];
	HWND  hWnd = ::GetDlgItem(m_hWnd, IDC_CONFIGDISK_MAXTASK_EDIT);
	::GetWindowText(hWnd, wValue, sizeof(wValue));
	try
	{
		int nValue = boost::lexical_cast<int>(wValue);
		if (nValue > 10)
		{
			::SetWindowText(hWnd, L"10");
		}
		else if (nValue < 1)
		{
			::SetWindowText(hWnd, L"1");
		}
	}
	catch (boost::bad_lexical_cast &)
	{
		::SetWindowText(hWnd, L"5");
	}
}
