#pragma once
#include "peerlistpage.h"
#include "generaloutputpage.h"



// CKDPanelSheet

class CKDPanelSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CKDPanelSheet)

public:
	CKDPanelSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CKDPanelSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CKDPanelSheet();

protected:
	DECLARE_MESSAGE_MAP()
public:
	CPeerListPage m_PeerListPage;
	CGeneralOutputPage m_GeneralOutputPage;
};


