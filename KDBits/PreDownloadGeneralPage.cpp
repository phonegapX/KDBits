// PreDownloadGeneralPage.cpp : 实现文件
//

#include "stdafx.h"
#include "KDBits.h"
#include "PreDownloadGeneralPage.h"
#include "configuration.h"
#include "pathselectdlg.h"

// CPreDownloadGeneralPage 对话框

IMPLEMENT_DYNAMIC(CPreDownloadGeneralPage, CPropertyPage)

CPreDownloadGeneralPage::CPreDownloadGeneralPage(torrent_info & t, PreDownloadProperty & PreDownProperty)
	: CPropertyPage(CPreDownloadGeneralPage::IDD), m_TorrentInfo(t), m_PreDownProperty(PreDownProperty)
{

}

CPreDownloadGeneralPage::~CPreDownloadGeneralPage()
{
}

void CPreDownloadGeneralPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GENERAL_SAVEPATH, m_editSavePath);
	DDX_Control(pDX, IDC_GENERAL_DISKSIZE, m_stDiskSize);
	DDX_Control(pDX, IDC_GENERAL_NAME, m_stName);
	DDX_Control(pDX, IDC_GENERAL_SIZE, m_stSize);
	DDX_Control(pDX, IDC_GENERAL_FILELIST, m_lcFileList);
}


BEGIN_MESSAGE_MAP(CPreDownloadGeneralPage, CPropertyPage)
	ON_BN_CLICKED(IDC_GENERAL_BROWER, &CPreDownloadGeneralPage::OnBnClickedGeneralBrower)
END_MESSAGE_MAP()

namespace
{
	__inline int InsertItem(HWND hWnd, const LVITEMA* pItem)
	{ 
		ASSERT(::IsWindow(hWnd)); return (int) ::SendMessageA(hWnd, LVM_INSERTITEMA, 0, (LPARAM)pItem); 
	}

	__inline int InsertItem(HWND hWnd, UINT nMask, int nItem, PCHAR lpszItem, UINT nState, UINT nStateMask, int nImage, LPARAM lParam)
	{
		ASSERT(::IsWindow(hWnd));
		LVITEMA item;
		item.mask = nMask;
		item.iItem = nItem;
		item.iSubItem = 0;
		item.pszText = (PCHAR)lpszItem;
		item.state = nState;
		item.stateMask = nStateMask;
		item.iImage = nImage;
		item.lParam = lParam;
		return InsertItem(hWnd, &item);
	}

	__inline int InsertItemAscii(HWND hWnd, int nItem, PCHAR lpszItem, int nImage)
	{ 
		ASSERT(::IsWindow(hWnd)); return InsertItem(hWnd, LVIF_TEXT|LVIF_IMAGE, nItem, lpszItem, 0, 0, nImage, 0); 
	}
}

// CPreDownloadGeneralPage 消息处理程序

BOOL CPreDownloadGeneralPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_Image.Create(16, 16, TRUE|ILC_COLOR24, 2, 1);
	m_lcFileList.SetImageList(&m_Image, LVSIL_SMALL);
	m_Image.Add(AfxGetApp()-> LoadIcon(IDR_MAINFRAME));
	m_Image.Add(AfxGetApp()-> LoadIcon(IDR_KDBitsTYPE));

	WCHAR strBuffer[MAX_PATH];
	LoadString(NULL, IDS_STRING_FILE_NAME, strBuffer, sizeof(strBuffer));
	m_lcFileList.InsertColumn(0, strBuffer, 0, 250);
	LoadString(NULL, IDS_STRING_FILE_SIZE, strBuffer, sizeof(strBuffer));
	m_lcFileList.InsertColumn(1, strBuffer, 0, 80);

	ListView_SetExtendedListViewStyle(m_lcFileList.m_hWnd, LVS_EX_FULLROWSELECT|LVS_EX_LABELTIP|LVS_EX_GRIDLINES);

	m_stName.SetWindowText(mbstowcs(m_TorrentInfo.name()).c_str());
	m_stSize.SetWindowText(formatData(m_TorrentInfo.total_size()));

	int iCount = 0;
	for (torrent_info::file_iterator i = m_TorrentInfo.begin_files(); i != m_TorrentInfo.end_files(); ++i, iCount++)
	{
		::InsertItemAscii(m_lcFileList.m_hWnd, iCount, (PCHAR)libtorrent::ConvertUtf8ToLocal(i->path.string()).c_str(), 0);
		m_lcFileList.SetItemText(iCount, 1, formatData(i->size));
	}

	wstring wPath = mbstowcs(m_PreDownProperty.SavePath.native_directory_string());
	m_editSavePath.SetWindowText(wPath.c_str());

	WCHAR szVolume[4] = L" :\\";
	szVolume[0] = wPath[0];

	ULARGE_INTEGER uFreeBytesAvailableToCaller;
	if(GetDiskFreeSpaceEx(szVolume, &uFreeBytesAvailableToCaller, NULL, NULL))
		m_stDiskSize.SetWindowText(formatData(uFreeBytesAvailableToCaller.QuadPart));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CPreDownloadGeneralPage::OnBnClickedGeneralBrower()
{
	// TODO: 在此添加控件通知处理程序代码
	CPathSelectDlg PathSelectDlg(this, L"请选择一个目录");
	if(PathSelectDlg.DoModal())
	{
		m_editSavePath.SetWindowText(PathSelectDlg.m_strPath);
		WCHAR szVolume[4] = L" :\\";
		szVolume[0] = PathSelectDlg.m_strPath[0];
		ULARGE_INTEGER uFreeBytesAvailableToCaller;
		if(GetDiskFreeSpaceEx(szVolume, &uFreeBytesAvailableToCaller, NULL, NULL))
			m_stDiskSize.SetWindowText(formatData(uFreeBytesAvailableToCaller.QuadPart));
	}
}

void CPreDownloadGeneralPage::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	CString strPath;
	m_editSavePath.GetWindowText(strPath);
	m_PreDownProperty.SavePath = wcstombs(wstring(strPath.GetBuffer()));
	CPropertyPage::OnOK();
}
