// KDContentSheet.cpp : 实现文件
//

#include "stdafx.h"
#include "KDBits.h"
#include "KDContentSheet.h"
#include "StartupDlg.h"

// CKDContentSheet

IMPLEMENT_DYNAMIC(CKDContentSheet, CPropertySheet)

CKDContentSheet::CKDContentSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{

}

CKDContentSheet::CKDContentSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	m_NetGamePage.SetTypeClass(CResListPage::TYPE_NETGAME);
	m_LocalGamePage.SetTypeClass(CResListPage::TYPE_LOCALGAME);
	m_OtherResPage.SetTypeClass(CResListPage::TYPE_OTHER);
	AddPage(&m_NetGamePage);
	AddPage(&m_LocalGamePage);
	AddPage(&m_OtherResPage);

	AddPage(&m_DownloadListPage);
}

CKDContentSheet::~CKDContentSheet()
{
}

BEGIN_MESSAGE_MAP(CKDContentSheet, CPropertySheet)
END_MESSAGE_MAP()

void CKDContentSheet::ReloadGameList(void)
{
	CStartupDlg StartupDlg;
	StartupDlg.DoModal();
	if (StartupDlg.m_bDownloadOK)
	{
		m_NetGamePage.SetTypeClass(CResListPage::TYPE_NETGAME);
		m_LocalGamePage.SetTypeClass(CResListPage::TYPE_LOCALGAME);
		m_OtherResPage.SetTypeClass(CResListPage::TYPE_OTHER);
		m_NetGamePage.UpdateList();
		m_LocalGamePage.UpdateList();
		m_OtherResPage.UpdateList();
	}
}
