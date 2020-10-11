// KDPanelSheet.cpp : 实现文件
//

#include "stdafx.h"
#include "KDBits.h"
#include "KDPanelSheet.h"


// CKDPanelSheet

IMPLEMENT_DYNAMIC(CKDPanelSheet, CPropertySheet)

CKDPanelSheet::CKDPanelSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{

}

CKDPanelSheet::CKDPanelSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_GeneralOutputPage);
	AddPage(&m_PeerListPage);
}

CKDPanelSheet::~CKDPanelSheet()
{
}


BEGIN_MESSAGE_MAP(CKDPanelSheet, CPropertySheet)
END_MESSAGE_MAP()


// CKDPanelSheet 消息处理程序
