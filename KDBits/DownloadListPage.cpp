// DownloadListPage.cpp : 实现文件
//

#include "stdafx.h"
#include "KDBits.h"
#include "DownloadListPage.h"
#include "DownloadList.h"
#include "PreDeleteDownNodeDlg.h"
#include "configuration.h"


#define DOWNLOADLIST_TIMEEVENT_ID 0x222

// CDownloadListPage 对话框

IMPLEMENT_DYNAMIC(CDownloadListPage, CPropertyPage)

CDownloadListPage::CDownloadListPage()
	: CPropertyPage(CDownloadListPage::IDD)
{
	static WCHAR strTitle[MAX_PATH];
	LoadString(NULL, IDS_STRING_TITLE_DOWNLOADLIST, strTitle, sizeof(strTitle));
	m_psp.hIcon    = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_psp.pszTitle = strTitle;
	m_psp.dwFlags &= ~PSP_USEICONID;   
	m_psp.dwFlags |= PSP_USEHICON;
	m_psp.dwFlags |= PSP_USETITLE;
}

CDownloadListPage::~CDownloadListPage()
{
}

void CDownloadListPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DOWNLOADLIST, m_DownloadList);
}


BEGIN_MESSAGE_MAP(CDownloadListPage, CPropertyPage)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_NOTIFY(LVN_GETDISPINFO, IDC_DOWNLOADLIST, &CDownloadListPage::OnLvnGetdispinfoDownloadlist)
	ON_NOTIFY(NM_RCLICK, IDC_DOWNLOADLIST, &CDownloadListPage::OnNMRclickDownloadlist)
	ON_COMMAND(ID_MENU_DOWNLIST_START_TASK, &CDownloadListPage::OnMenuDownlistStartTask)
	ON_COMMAND(ID_MENU_DOWNLIST_STOP_TASK, &CDownloadListPage::OnMenuDownlistStopTask)
	ON_COMMAND(ID_MENU_DOWNLIST_DELETE_TASK, &CDownloadListPage::OnMenuDownlistDeleteTask)
	ON_COMMAND(ID_MENU_DOWNLIST_ANNOUNCE_TASK, &CDownloadListPage::OnMenuDownlistAnnounceTask)
	ON_COMMAND(ID_MENU_DOWNLIST_TORRENT_FILE_SAVE, &CDownloadListPage::OnMenuDownlistTorrentFileSave)
	ON_COMMAND(ID_MENU_DOWNLIST_TASK_PROPERTY, &CDownloadListPage::OnMenuDownlistTaskProperty)
	ON_COMMAND(ID_MENU_DOWNLIST_OPEN_DIR, &CDownloadListPage::OnMenuDownlistOpenDir)
	ON_NOTIFY(NM_DBLCLK, IDC_DOWNLOADLIST, &CDownloadListPage::OnNMDblclkDownloadlist)
END_MESSAGE_MAP()


// CDownloadListPage 消息处理程序

BOOL CDownloadListPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_Image.Create(16, 16, TRUE|ILC_COLOR24, 3, 1);	//下载 停止 完成 3种图标
	m_DownloadList.SetImageList(&m_Image, LVSIL_SMALL);
	m_Image.Add(AfxGetApp()-> LoadIcon(IDR_MAINFRAME));
	m_Image.Add(AfxGetApp()-> LoadIcon(IDR_KDBitsTYPE));
	m_Image.Add(AfxGetApp()-> LoadIcon(IDR_KDBitsTYPE));

	// TODO:  在此添加额外的初始化
	WCHAR strBuffer[MAX_PATH];

	LoadString(NULL, IDS_STRING_DOWNLOADLIST_NAME, strBuffer, sizeof(strBuffer));
	m_DownloadList.InsertColumn(0, strBuffer, 0, 120);

	LoadString(NULL, IDS_STRING_DOWNLOADLIST_TYPE, strBuffer, sizeof(strBuffer));
	m_DownloadList.InsertColumn(1, strBuffer, 0, 70);

	LoadString(NULL, IDS_STRING_DOWNLOADLIST_STATE, strBuffer, sizeof(strBuffer));
	m_DownloadList.InsertColumn(2, strBuffer, 0, 70);

	LoadString(NULL, IDS_STRING_DOWNLOADLIST_PROGRESS, strBuffer, sizeof(strBuffer));
	m_DownloadList.InsertColumn(3, strBuffer, 0, 70);

	LoadString(NULL, IDS_STRING_DOWNLOADLIST_DOWNSPEED, strBuffer, sizeof(strBuffer));
	m_DownloadList.InsertColumn(4, strBuffer, 0, 70);

	LoadString(NULL, IDS_STRING_DOWNLOADLIST_UPSPEED, strBuffer, sizeof(strBuffer));
	m_DownloadList.InsertColumn(5, strBuffer, 0, 70);

	LoadString(NULL, IDS_STRING_DOWNLOADLIST_SIZE, strBuffer, sizeof(strBuffer));
	m_DownloadList.InsertColumn(6, strBuffer, 0, 70);

	LoadString(NULL, IDS_STRING_DOWNLOADLIST_DOWNEDSIZE, strBuffer, sizeof(strBuffer));
	m_DownloadList.InsertColumn(7, strBuffer, 0, 70);

	LoadString(NULL, IDS_STRING_DOWNLOADLIST_UPEDSIZE, strBuffer, sizeof(strBuffer));
	m_DownloadList.InsertColumn(8, strBuffer, 0, 70);

	LoadString(NULL, IDS_STRING_DOWNLOADLIST_HEALTH, strBuffer, sizeof(strBuffer));
	m_DownloadList.InsertColumn(9, strBuffer, 0, 70);

	LoadString(NULL, IDS_STRING_DOWNLOADLIST_SEEDS, strBuffer, sizeof(strBuffer));
	m_DownloadList.InsertColumn(10, strBuffer, 0, 70);

	LoadString(NULL, IDS_STRING_DOWNLOADLIST_PEERS, strBuffer, sizeof(strBuffer));
	m_DownloadList.InsertColumn(11, strBuffer, 0, 70);

	LoadString(NULL, IDS_STRING_DOWNLOADLIST_PATH, strBuffer, sizeof(strBuffer));
	m_DownloadList.InsertColumn(12, strBuffer, 0, 300);

	ListView_SetExtendedListViewStyle(m_DownloadList.m_hWnd, LVS_EX_FULLROWSELECT|LVS_EX_LABELTIP|LVS_EX_GRIDLINES);

	SetTimer(DOWNLOADLIST_TIMEEVENT_ID, 1000, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDownloadListPage::OnSize(UINT nType, int cx, int cy)
{
//	CPropertyPage::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	ShowWindow(SW_MAXIMIZE);

	CWnd * pchild = GetWindow(GW_CHILD);//只有一个顶层子窗口
	if(pchild != NULL)
	{
		CRect rect;
		GetWindowRect(&rect);
		pchild->ScreenToClient(&rect);
		pchild->SetWindowPos(NULL, 0, 0, rect.Width(), rect.Height(), SWP_NOZORDER|SWP_NOACTIVATE);
	}
}

void CDownloadListPage::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == DOWNLOADLIST_TIMEEVENT_ID)
	{
		g_DownloadList.UpdateState();
		ListView_SetItemCount(m_DownloadList.m_hWnd, g_DownloadList.Count());//下载列表有多少项 那么用于显示的列表控件也要有多少项
		::InvalidateRect(m_DownloadList.m_hWnd, NULL, FALSE);//更新列表
	}
	CPropertyPage::OnTimer(nIDEvent);
}

void CDownloadListPage::OnLvnGetdispinfoDownloadlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *di = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	if(di->item.iItem < (int)g_DownloadList.Count() && di->item.iSubItem < (int)DOWNLOADLIST_COLUMN_NUMBER)
	{
/*
		if (m_ResInfoList[di->item.iItem].bNew)
		{
			di->item.iImage = 0;
		}
		else
		{
			di->item.iImage = 1;
		}
*/
di->item.iImage = 0;
		di->item.pszText = (LPTSTR)g_DownloadList[di->item.iItem].cols[di->item.iSubItem].c_str();
	}

	*pResult = 0;
}

void CDownloadListPage::OnNMRclickDownloadlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码

	LPNMITEMACTIVATE pNmi = (LPNMITEMACTIVATE)pNMHDR;
	CMenu clMenu;
	clMenu.LoadMenu(IDR_MENU_DOWNLIST_POP);
	CMenu* ctPopMenu = clMenu.GetSubMenu(0);
	if (pNmi->iItem != -1)		//选中条目
	{
		POINT stPt;
		GetCursorPos(&stPt);
		ctPopMenu->TrackPopupMenu(TPM_LEFTALIGN,stPt.x,stPt.y,this);
	}
	*pResult = 0;
}

void CDownloadListPage::OnMenuDownlistStartTask()
{
	// TODO: 在此添加命令处理程序代码
	if (m_DownloadList.GetSelectedCount() > 0)
	{
		POSITION pos = m_DownloadList.GetFirstSelectedItemPosition();
		if (pos != NULL)
		{
			while (pos)
			{
				int nItem = m_DownloadList.GetNextSelectedItem(pos);
				TRACE1("Item %d was selected!\n", nItem);
				if (g_DownloadList[nItem].Handle.is_paused())
				{
					g_DownloadList[nItem].Handle.resume();
				}
			}
		}
	}
}

void CDownloadListPage::OnMenuDownlistStopTask()
{
	// TODO: 在此添加命令处理程序代码
	if (m_DownloadList.GetSelectedCount() > 0)
	{
		POSITION pos = m_DownloadList.GetFirstSelectedItemPosition();
		if (pos != NULL)
		{
			while (pos)
			{
				int nItem = m_DownloadList.GetNextSelectedItem(pos);
				TRACE1("Item %d was selected!\n", nItem);
				if (!g_DownloadList[nItem].Handle.is_paused())
				{
					g_DownloadList[nItem].Handle.pause();
				}
			}
		}
	}
}

void CDownloadListPage::DeleteTask(string Name, path FullPath, bool bDelPathFile)	//删除下载任务和文件(目录)
{
	if(g_DownloadList.DeleteTask(Name) && bDelPathFile)
	{	//删除目录或文件
		CString strPath;
		strPath = FullPath.native_directory_string().c_str();
		WCHAR wPath[MAX_PATH] = {0};
		wcscpy(wPath, strPath.GetBuffer());
		SHFILEOPSTRUCT FileOp = {0};//定义SHFILEOPSTRUCT结构对象;
		FileOp.hwnd   = *this;
		FileOp.wFunc  = FO_DELETE; //执行文件删除操作;
		FileOp.pFrom  = wPath;
		FileOp.fFlags = FOF_ALLOWUNDO | FOF_NOERRORUI | FOF_SILENT | FOF_NOCONFIRMATION;//删除文件到Windows回收站等等
		//开始删除文件
		SHFileOperation(&FileOp);
	}
}

void CDownloadListPage::OnMenuDownlistDeleteTask()
{
	// TODO: 在此添加命令处理程序代码
	if (m_DownloadList.GetSelectedCount() > 0)
	{
		POSITION pos = m_DownloadList.GetFirstSelectedItemPosition();
		if (pos != NULL)
		{
			CPreDeleteDownNodeDlg DelNodeDlg;
			DelNodeDlg.m_bDelFileThenDelTask = g_Configuration.DelFileThenDelTask;
			while (pos)
			{
				int nItem = m_DownloadList.GetNextSelectedItem(pos);
				DelNodeDlg.m_NameList.push_back(std::make_pair(g_DownloadList[nItem].Name, g_DownloadList[nItem].DownNodeProperty.SavePath/path(g_DownloadList[nItem].Name)));
			}
			if (DelNodeDlg.DoModal() == IDOK)
			{
				std::for_each(DelNodeDlg.m_NameList.begin(), DelNodeDlg.m_NameList.end(), bind(&CDownloadListPage::DeleteTask, this, bind(&pair<string, path>::first, _1), bind(&pair<string, path>::second, _1), DelNodeDlg.m_bDelFileThenDelTask));
			}
		}
	}
}

void CDownloadListPage::OnMenuDownlistAnnounceTask()
{
	// TODO: 在此添加命令处理程序代码
	if (m_DownloadList.GetSelectedCount() > 0)
	{
		POSITION pos = m_DownloadList.GetFirstSelectedItemPosition();
		if (pos != NULL)
		{
			while (pos)
			{
				int nItem = m_DownloadList.GetNextSelectedItem(pos);
				TRACE1("Item %d was selected!\n", nItem);
				g_DownloadList[nItem].Handle.force_reannounce();
			}
		}
	}
}

void CDownloadListPage::EventStartAllTask(void)
{
	std::for_each(g_DownloadList.begin(), g_DownloadList.end(), bind(&torrent_handle::resume, bind(&DownloadNode::Handle, _1)));
}

void CDownloadListPage::EventStopAllTask(void)
{
	std::for_each(g_DownloadList.begin(), g_DownloadList.end(), bind(&torrent_handle::pause, bind(&DownloadNode::Handle, _1)));
}

void CDownloadListPage::EventDeleteAllTask(void)
{
	CPreDeleteDownNodeDlg DelNodeDlg;

	DelNodeDlg.m_bDelFileThenDelTask = g_Configuration.DelFileThenDelTask;

	std::for_each
		(
		g_DownloadList.begin(), 
		g_DownloadList.end(), 
		bind(
			&vector< pair<string, path> >::push_back,
			boost::ref(DelNodeDlg.m_NameList),
			bind(&std::make_pair<string, path>, bind(&DownloadNode::Name, _1), bind(std::divides<path>(), bind(&DownloadNode::DownNodeProperty, _1), bind(&DownloadNode::Name, _1)))
			)
		);

	if (DelNodeDlg.DoModal() == IDOK)
	{
		std::for_each(DelNodeDlg.m_NameList.begin(), DelNodeDlg.m_NameList.end(), bind(&CDownloadListPage::DeleteTask, this, bind(&pair<string, path>::first, _1), bind(&pair<string, path>::second, _1), DelNodeDlg.m_bDelFileThenDelTask));
	}
}

void CDownloadListPage::OnMenuDownlistTorrentFileSave()
{
	// TODO: 在此添加命令处理程序代码
	if (m_DownloadList.GetSelectedCount() > 0)
	{
		POSITION pos = m_DownloadList.GetFirstSelectedItemPosition();
		if (pos != NULL)
		{
			while (pos)
			{
				int nItem = m_DownloadList.GetNextSelectedItem(pos);
				const DownloadNode & Node = g_DownloadList[nItem];
				CFileDialog FileDlg(FALSE, NULL, NULL, NULL, L"种子文件 (*.torrent)|*.torrent|所有文件 (*.*)|*.*||", NULL);
				FileDlg.m_ofn.lpstrTitle = L"种子文件另存为";
				WCHAR wFile[MAX_PATH] = {0};
				wstring strFile = mbstowcs(Node.FileName);
				copy(strFile.begin(), strFile.end(), wFile);
				FileDlg.m_ofn.lpstrFile = wFile;
				if(FileDlg.DoModal() == IDOK)
				{
					path OrgPath  = GetTorrentPath()/Node.FileName;
					path DescPath = wcstombs(wstring(FileDlg.GetPathName().GetBuffer()));
					if (exists(DescPath))
					{
						if (AfxMessageBox(L"目标文件已经存在，是否替换？", MB_ICONWARNING|MB_YESNO) == IDNO)
						{
							break;
						}
					}
					if (OrgPath == DescPath)
					{
						break;
					}
					try
					{
						boost::filesystem::remove(DescPath);
						boost::filesystem::copy_file(OrgPath, DescPath); //将种子文件拷贝到指定目录
					}
					catch (...){}
				}
			}
		}
	}
}

__inline bool ShowPreDownloadDialog(torrent_info & t, PreDownloadProperty & PreDownProperty);

void CDownloadListPage::OnMenuDownlistTaskProperty()
{
	// TODO: 在此添加命令处理程序代码
	if (m_DownloadList.GetSelectedCount() > 0)
	{
		POSITION pos = m_DownloadList.GetFirstSelectedItemPosition();
		if (pos != NULL)
		{
			while (pos)
			{
				int nItem = m_DownloadList.GetNextSelectedItem(pos);
				DownloadNode & Node = const_cast<DownloadNode &>(g_DownloadList[nItem]);
				PreDownloadProperty & PreDownProperty = Node.DownNodeProperty;
				PreDownloadProperty OrgProperty = Node.DownNodeProperty; //保存原属性
				if(ShowPreDownloadDialog(const_cast<torrent_info &>(Node.Handle.get_torrent_info()), PreDownProperty))
				{
					if (OrgProperty.SavePath != PreDownProperty.SavePath)	//是否修改了保存目录
					{
						if(AfxMessageBox(L"您修改了存放目录， 所有下载数据都会转移至新目录中，数据量大的话将会是个费时又漫长的操作，是否确定修改？", MB_ICONWARNING|MB_YESNO) != IDYES)
						{
							PreDownProperty = OrgProperty;	//还原设置
							break;
						}
						Node.Handle.move_storage(PreDownProperty.SavePath);//0.13
						/*
						if (!Node.Handle.move_storage(PreDownProperty.SavePath))
						{
							PreDownProperty = OrgProperty;	//还原设置
							AfxMessageBox(L"移动存放目录失败", MB_ICONERROR);
							break;
						}
						*/
						Node.cols[12] = mbstowcs((PreDownProperty.SavePath/Node.Name).native_directory_string());
					}
					Node.Handle.set_upload_limit  (PreDownProperty.UploadLimit>=1?PreDownProperty.UploadLimit*dwBytesOfKilo:PreDownProperty.UploadLimit);
					Node.Handle.set_download_limit(PreDownProperty.DownloadLimit>=1?PreDownProperty.DownloadLimit*dwBytesOfKilo:PreDownProperty.DownloadLimit);
				}
			}
		}
	}
}

void CDownloadListPage::OnNMDblclkDownloadlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	OnMenuDownlistTaskProperty();
	*pResult = 0;
}

namespace
{

	__inline HRESULT GetShellFolderViewDual(ITEMIDLIST* pidl, IShellFolderViewDual** ppIShellFolderViewDual)
	{
		IWebBrowserApp* pIWebBrowserApp;
		IDispatch* pDoc;
		HWND hWnd;
		HRESULT hr;
		HINSTANCE ghSHDOCVW;

		HRESULT (WINAPI*gpfSHGetIDispatchForFolder)(ITEMIDLIST* pidl, IWebBrowserApp** ppIWebBrowserApp);

		*ppIShellFolderViewDual = NULL;

		ghSHDOCVW = LoadLibrary(_T("SHDOCVW.DLL"));
		if (ghSHDOCVW == NULL)
			return FALSE;

		pIWebBrowserApp=NULL;
		gpfSHGetIDispatchForFolder = (HRESULT (WINAPI*)(ITEMIDLIST*, IWebBrowserApp**)) GetProcAddress(ghSHDOCVW, "SHGetIDispatchForFolder");
		if (gpfSHGetIDispatchForFolder == NULL)
			return FALSE;

		/// 调用未公开的API函数 SHGetIDispatchForFolder
		if (SUCCEEDED(gpfSHGetIDispatchForFolder(pidl, &pIWebBrowserApp))) 
		{
			if (SUCCEEDED(pIWebBrowserApp->get_HWND((long*)&hWnd))) 
			{
				SetForegroundWindow(hWnd);
				ShowWindow(hWnd, SW_SHOWNORMAL);
			}

			if (SUCCEEDED(hr = pIWebBrowserApp->get_Document(&pDoc)))
			{
				pDoc->QueryInterface(IID_IShellFolderViewDual, (void**) ppIShellFolderViewDual);
				pDoc->Release();
			}

			pIWebBrowserApp->Release();
		}
		FreeLibrary(ghSHDOCVW);

		return TRUE;
	}

	__inline BOOL XZSHOpenFolderAndSelectItems(ITEMIDLIST *pidlFolder)
	{
		ITEMIDLIST *pidl, *pidl2;
		ITEMIDLIST* pIdlFile;
		USHORT cb;
		IShellFolderViewDual* pIShellFolderViewDual;

		HRESULT (WINAPI *gpfSHOpenFolderAndSelectItems)(LPCITEMIDLIST *pidlFolder, UINT cidl, LPCITEMIDLIST *apidl, DWORD dwFlags);
		HINSTANCE ghShell32;
		/// 只有WinXp及以上及系统才支持SHOpenFolderAndSelectItems() API
		/// 那其它系统该怎么实现这个功能呢？只能采用其它的方法来处理
		/// 首先用XP跟踪到SHOpenFolderAndSelectItems()API中，看它是如何处理的，再用同样的方法去实现
		/// 其它系统的这个功能使用工具 VC6 .net 2003 MSDN Ollydbg v1.10中文版

		ghShell32 = LoadLibrary(_T("Shell32.DLL"));
		if (ghShell32 == NULL)
			return FALSE;

		gpfSHOpenFolderAndSelectItems = (HRESULT (WINAPI*)(LPCITEMIDLIST*, UINT, LPCITEMIDLIST*, DWORD)) GetProcAddress(ghShell32, "SHOpenFolderAndSelectItems");
		if (gpfSHOpenFolderAndSelectItems != NULL)
		{
			/// 可以获得SHOpenFolderAndSelectItems()函数的API地址
			if (SUCCEEDED(gpfSHOpenFolderAndSelectItems((LPCITEMIDLIST*)pidlFolder,0,(LPCITEMIDLIST*)NULL,0))) 
			{
				///直接调用系统的功能
				FreeLibrary(ghShell32);
				return TRUE;
			}
			FreeLibrary(ghShell32);
			return FALSE;
		}
		FreeLibrary(ghShell32);

		/// 当操作系统不支持SHOpenFolderAndSelectItems()函数的API时的处理，
		/// 自已动手写一个与系统功能相同的代码
		pidl = pidlFolder;
		pIdlFile = pidl;
		/// 找出目标文件的偏移量
		while (cb = pIdlFile->mkid.cb) 
		{
			pidl2 = pIdlFile;
			pIdlFile = (ITEMIDLIST*)((BYTE*)pIdlFile + cb);
		}

		cb = pidl2->mkid.cb;
		pidl2->mkid.cb = 0;

		/// 打开目标文件所在的文件夹
		if (SUCCEEDED(GetShellFolderViewDual(pidl, &pIShellFolderViewDual))) 
		{
			pidl2->mkid.cb = cb;
			// 0 Deselect the item.  
			// 1 Select the item.  
			// 3 Put the item in edit mode.  
			// 4 Deselect all but the specified item.  
			// 8 Ensure the item is displayed in the view.  
			// 0x10 Give the item the focus.  
			COleVariant bszFile(pidl2);

			if(pIShellFolderViewDual != NULL)
			{
				/// 选中相应的选项
				pIShellFolderViewDual->SelectItem(bszFile, 0x1d);
				pIShellFolderViewDual->Release();
			}
			return TRUE;
		}

		return FALSE;
	}

	__inline void FindTarget(CString str)
	{
		HRESULT hres; 
		IShellLink *psl; 
		ITEMIDLIST *pidl;
		IPersistFile *ppf; 

		CoInitialize(NULL);

		// Get a pointer to the IShellLink interface. 
		hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&psl); 
		if (SUCCEEDED(hres)) 
		{ 
			// 设置目标文件
			psl->SetPath(str);
			/// 获得目标文件的ITEMIDLIST
			psl->GetIDList(&pidl);

			// Get a pointer to the IPersistFile interface. 
			hres = psl->QueryInterface(IID_IPersistFile, (void**)&ppf); 
			if (SUCCEEDED(hres)) 
			{ 
				WCHAR wsz[MAX_PATH];
	#ifdef _UNICODE
				wcscpy(wsz, str);
	#else
				// Ensure that the string is Unicode. 
				MultiByteToWideChar(CP_ACP, 0, str, -1, wsz, MAX_PATH); 
	#endif 
				// Load the shortcut. 
				hres = ppf->Load(wsz, STGM_READ); 
				if (SUCCEEDED(hres)) 
				{ 
					/// 获得快捷方式的ITEMIDLIST
					psl->GetIDList(&pidl);
				}
				ppf->Release();
			}

			/// 打开文件夹并选中项目
			XZSHOpenFolderAndSelectItems(pidl);

			psl->Release();
		}
		CoUninitialize();
	}

}

void CDownloadListPage::OnMenuDownlistOpenDir()
{
	// TODO: 在此添加命令处理程序代码
	if (m_DownloadList.GetSelectedCount() > 0)
	{
		POSITION pos = m_DownloadList.GetFirstSelectedItemPosition();
		if (pos != NULL)
		{
			while (pos)
			{
				int nItem = m_DownloadList.GetNextSelectedItem(pos);
				const DownloadNode & Node = g_DownloadList[nItem];
				CString strPath;
				strPath = (Node.DownNodeProperty.SavePath/Node.Name).native_directory_string().c_str();
				FindTarget(strPath);
			}
		}
	}
}
