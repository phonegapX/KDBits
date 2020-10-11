// PeerListPage.cpp : 实现文件
//

#include "stdafx.h"
#include "KDBits.h"
#include "PeerListPage.h"


// CPeerListPage 对话框

IMPLEMENT_DYNAMIC(CPeerListPage, CPropertyPage)

CPeerListPage::CPeerListPage()
	: CPropertyPage(CPeerListPage::IDD)
{
	m_psp.dwFlags &= ~PSP_USEICONID;   
	m_psp.dwFlags |= PSP_USEHICON;
	m_psp.dwFlags |= PSP_USETITLE;
	m_psp.hIcon    = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_psp.pszTitle = L"连接列表";
}

CPeerListPage::~CPeerListPage()
{
}

void CPeerListPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPeerListPage, CPropertyPage)
END_MESSAGE_MAP()


// CPeerListPage 消息处理程序
