#pragma once
#include "ResListPage.h"
#include "DownloadListPage.h"

// CKDContentSheet

class CKDContentSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CKDContentSheet)

public:
	CKDContentSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CKDContentSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CKDContentSheet();

protected:
	DECLARE_MESSAGE_MAP()
public:
	CResListPage m_NetGamePage;
public:
	CResListPage m_LocalGamePage;
public:
	CResListPage m_OtherResPage;
public:
	CDownloadListPage m_DownloadListPage;
public:
	void ReloadGameList(void);
};
