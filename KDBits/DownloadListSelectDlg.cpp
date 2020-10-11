// DownloadListSelectDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "KDBits.h"
#include "DownloadListSelectDlg.h"
#include "DownloadList.h"
#include "configuration.h"
#include "pathselectdlg.h"

// CDownloadListSelectDlg 对话框

IMPLEMENT_DYNAMIC(CDownloadListSelectDlg, CDialog)

CDownloadListSelectDlg::CDownloadListSelectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDownloadListSelectDlg::IDD, pParent)
	, m_bQuitExecute(false)
	, m_Index(0)
{

}

CDownloadListSelectDlg::~CDownloadListSelectDlg()
{
}

void CDownloadListSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_UPDATERULE, m_cbRule);
	DDX_Control(pDX, IDC_LIST_DOWNLOADLIST_SELECT, m_lcSelectList);
	DDX_Control(pDX, IDC_EDIT_PATH, m_edtPath);
	DDX_Control(pDX, IDC_STATIC_DOWNLOADSEED, m_StaticView);
	DDX_Control(pDX, IDC_PROGRESS_DOWNLOADSEED, m_ProgressCtrl);
}


BEGIN_MESSAGE_MAP(CDownloadListSelectDlg, CDialog)
	ON_BN_CLICKED(IDCANCEL, &CDownloadListSelectDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CDownloadListSelectDlg::OnBnClickedOk)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_DOWNLOADLIST_SELECT, &CDownloadListSelectDlg::OnLvnItemchangedListDownloadlistSelect)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, &CDownloadListSelectDlg::OnBnClickedButtonBrowse)
	ON_CBN_SELCHANGE(IDC_COMBO_UPDATERULE, &CDownloadListSelectDlg::OnCbnSelchangeComboUpdaterule)
END_MESSAGE_MAP()


// CDownloadListSelectDlg 消息处理程序

void CDownloadListSelectDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bQuitExecute = true;
	OnCancel();
}

void CDownloadListSelectDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bQuitExecute = false;
	try
	{
		m_ProgressCtrl.ShowWindow(SW_SHOW);
		m_StaticView.ShowWindow(SW_SHOW);
		for (vector< tuple<ResInfoNode, path> >::const_iterator i = m_GameList.begin(); i != m_GameList.end(); ++i)
		{
			wstring strView = mbstowcs(i->get<0>().Name);
			strView = (L"下载种子：" + strView) + L"                    ";
			m_StaticView.SetWindowText(strView.c_str());
			DownloadTorrentFile(i->get<0>().DownloadUrl);
		}
		OnOK();
	}
	catch (wstring strError)	//发生异常
	{
		AfxMessageBox(strError.c_str(), MB_ICONERROR);
	}
	catch (ULONG)	//用户点了取消,对话框会关闭,所以这里我们不用做任何操作
	{
	}
}

BOOL CDownloadListSelectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME), FALSE);		// Set small icon

	WCHAR strBuffer[MAX_PATH];

	LoadString(NULL, IDS_STRING_RULE_AUTOMATIC, strBuffer, sizeof(strBuffer));
	m_cbRule.InsertString(automatic, strBuffer);
	LoadString(NULL, IDS_STRING_RULE_MANUAL, strBuffer, sizeof(strBuffer));
	m_cbRule.InsertString(manual,    strBuffer);

	m_lcSelectList.InsertColumn(0, L"", 0, 30);
	LoadString(NULL, IDS_STRING_RES_NAME, strBuffer, sizeof(strBuffer));
	m_lcSelectList.InsertColumn(1, strBuffer, 0, 110);
	LoadString(NULL, IDS_STRING_RES_TYPE, strBuffer, sizeof(strBuffer));
	m_lcSelectList.InsertColumn(2, strBuffer, 0, 80);
	LoadString(NULL, IDS_STRING_RES_VER, strBuffer, sizeof(strBuffer));
	m_lcSelectList.InsertColumn(3, strBuffer, 0, 70);
	LoadString(NULL, IDS_STRING_RES_RULE, strBuffer, sizeof(strBuffer));
	m_lcSelectList.InsertColumn(4, strBuffer, 0, 80);
	LoadString(NULL, IDS_STRING_INSTALL_PATH, strBuffer, sizeof(strBuffer));
	m_lcSelectList.InsertColumn(5, strBuffer, 0, 300);

	ListView_SetExtendedListViewStyle(m_lcSelectList.m_hWnd, LVS_EX_FULLROWSELECT|LVS_EX_LABELTIP|LVS_EX_GRIDLINES);

	m_ProgressCtrl.SetRange(0, 100);

	m_ProgressCtrl.SetPos(0);

	UpdateListView();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDownloadListSelectDlg::OnLvnItemchangedListDownloadlistSelect(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	m_Index = pNMLV->iItem;
	tuple<ResInfoNode, path> & tupleNode = m_GameList[pNMLV->iItem];
	CString strPath;
	strPath = tupleNode.get<1>().branch_path().native_directory_string().c_str();
	m_edtPath.SetWindowText(strPath);
	m_cbRule.SetCurSel(tupleNode.get<0>().UpdateRule);
	*pResult = 0;
}

void CDownloadListSelectDlg::DownloadTorrentFile(string strHttpUrl)
{
	m_ProgressCtrl.SetPos(0);
	try
	{
		HINTERNET hSession = InternetOpen(L"Microsoft Internet Explorer", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
		if (hSession == NULL)
		{
			throw(wstring(L"无法连接服务器"));
		}
		wstring Url = mbstowcs(strHttpUrl);
		HINTERNET hHttpFile = InternetOpenUrl(hSession, Url.c_str(), NULL, 0, INTERNET_FLAG_NO_AUTO_REDIRECT, 0);
		if(hHttpFile == NULL)
		{
			throw(wstring(L"无法连接服务器"));
		}
		WCHAR szBuffer[512];
		DWORD dwRead = sizeof(szBuffer);
		if(!HttpQueryInfo(hHttpFile, HTTP_QUERY_STATUS_CODE, szBuffer, &dwRead, NULL))
		{
			throw(wstring(L"无法连接服务器"));
		}
		if(wcscmp(szBuffer, L"200") != 0 && wcscmp(szBuffer, L"206") != 0)
		{
			throw(wstring(L"无法连接服务器"));
		}
		dwRead = sizeof(szBuffer);
		if(!HttpQueryInfo(hHttpFile, HTTP_QUERY_CONTENT_LENGTH, szBuffer, &dwRead,NULL))
		{
			throw(wstring(L"无法连接服务器"));
		}

		double iBlockLen = boost::lexical_cast<double>(szBuffer) / 100.0f;
		int iDownloadedLen   = 0;

		path FilePath = GetTorrentPath();
		FilePath /= path(strHttpUrl).leaf();

		std::locale prev_loc = std::locale::global(std::locale(""));
		ofstream fs(FilePath, ofstream::binary);
		std::locale::global(prev_loc);

		if(!fs.is_open()) //判断文件创建成功
		{
			throw(wstring(L"无法创建文件"));
		}

		while (InternetReadFile(hHttpFile, szBuffer, sizeof(szBuffer), &dwRead) && dwRead != 0)
		{
			fs.write((const char*)szBuffer, dwRead);
			iDownloadedLen += dwRead;
			m_ProgressCtrl.SetPos(iDownloadedLen/iBlockLen);
	
			MSG msg = {0};
			while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT || m_bQuitExecute)
				{
					throw(ULONG(0));
				}
				TranslateMessage(&msg);
				DispatchMessage(&msg);     
			}
		}

		m_ProgressCtrl.SetPos(100);
		fs.close();
		InternetCloseHandle(hHttpFile);
		InternetCloseHandle(hSession);
	}
	catch (wstring)
	{
		throw;
	}
	catch (ULONG)
	{
		throw;
	}
}

void CDownloadListSelectDlg::OnBnClickedButtonBrowse()
{
	// TODO: 在此添加控件通知处理程序代码
	CPathSelectDlg PathSelectDlg(this, L"请选择一个目录");
	if(PathSelectDlg.DoModal())
	{
		tuple<ResInfoNode, path> & tupleNode = m_GameList[m_Index];
		tupleNode.get<1>() = path(wcstombs(wstring(PathSelectDlg.m_strPath.GetBuffer())))/tupleNode.get<0>().Name;
		UpdateListView();
	}
}

void CDownloadListSelectDlg::UpdateListView(void)
{
	m_lcSelectList.DeleteAllItems();
	for (vector< tuple<ResInfoNode, path> >::size_type i = 0; i < m_GameList.size(); i++)
	{
		tuple<ResInfoNode, path> & tupleNode = m_GameList[i];
		wstring strwIndex = mbstowcs(boost::lexical_cast<string>(i+1));
		m_lcSelectList.InsertItem(i, strwIndex.c_str());
		m_lcSelectList.SetItemText(i, 1, tupleNode.get<0>().cols[0].c_str());
		m_lcSelectList.SetItemText(i, 2, tupleNode.get<0>().cols[1].c_str());
		m_lcSelectList.SetItemText(i, 3, tupleNode.get<0>().cols[2].c_str());
		m_lcSelectList.SetItemText(i, 4, tupleNode.get<0>().cols[5].c_str());
		m_lcSelectList.SetItemText(i, 5, CString(mbstowcs(tupleNode.get<1>().native_directory_string()).c_str()));
	}
	m_lcSelectList.SetItemState(m_Index, LVNI_SELECTED, LVNI_SELECTED);
	m_lcSelectList.SetHotItem(m_Index);
}

void CDownloadListSelectDlg::OnCbnSelchangeComboUpdaterule()
{
	// TODO: 在此添加控件通知处理程序代码
	int i = m_cbRule.GetCurSel();
	if (i != CB_ERR)
	{
		UPDATE_RULE Rule = static_cast<UPDATE_RULE>(i);
		tuple<ResInfoNode, path> & tupleNode = m_GameList[m_Index];
		tupleNode.get<0>().UpdateRule = Rule;
		if (Rule == automatic)
		{
			tupleNode.get<0>().cols[5] = L"自动更新";
		}
		else if (Rule == manual)
		{
			tupleNode.get<0>().cols[5] = L"手动更新";
		}
		UpdateListView();
	}
}
