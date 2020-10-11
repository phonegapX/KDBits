// GeneralOutputPage.cpp : 实现文件
//

#include "stdafx.h"
#include "KDBits.h"
#include "GeneralOutputPage.h"


// CGeneralOutputPage 对话框

IMPLEMENT_DYNAMIC(CGeneralOutputPage, CPropertyPage)

CGeneralOutputPage::CGeneralOutputPage()
	: CPropertyPage(CGeneralOutputPage::IDD)
{
	m_psp.dwFlags &= ~PSP_USEICONID;   
	m_psp.dwFlags |= PSP_USEHICON;
	m_psp.dwFlags |= PSP_USETITLE;
	m_psp.hIcon    = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_psp.pszTitle = L"输出";
}

CGeneralOutputPage::~CGeneralOutputPage()
{
}

void CGeneralOutputPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGeneralOutputPage, CPropertyPage)
END_MESSAGE_MAP()


// CGeneralOutputPage 消息处理程序
