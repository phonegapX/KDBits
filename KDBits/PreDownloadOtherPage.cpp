// PreDownloadOtherPage.cpp : 实现文件
//

#include "stdafx.h"
#include "KDBits.h"
#include "PreDownloadOtherPage.h"


// CPreDownloadOtherPage 对话框

IMPLEMENT_DYNAMIC(CPreDownloadOtherPage, CPropertyPage)

CPreDownloadOtherPage::CPreDownloadOtherPage(torrent_info & t)
	: CPropertyPage(CPreDownloadOtherPage::IDD), m_TorrentInfo(t)
{

}

CPreDownloadOtherPage::~CPreDownloadOtherPage()
{
}

void CPreDownloadOtherPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OTHER_COMMENT, m_edtComment);
	DDX_Control(pDX, IDC_OTHER_CREATOR, m_edtCreator);
	DDX_Control(pDX, IDC_OTHER_URLLIST, m_edtUrlList);
}


BEGIN_MESSAGE_MAP(CPreDownloadOtherPage, CPropertyPage)
END_MESSAGE_MAP()


// CPreDownloadOtherPage 消息处理程序

BOOL CPreDownloadOtherPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_edtCreator.SetWindowText(mbstowcs(m_TorrentInfo.creator()).c_str());
	m_edtComment.SetWindowText(mbstowcs(m_TorrentInfo.comment()).c_str());
	string strUrlList;
	for (std::vector<announce_entry>::const_iterator i = m_TorrentInfo.trackers().begin(); i != m_TorrentInfo.trackers().end(); ++i)
	{
		strUrlList += i->url;
		strUrlList += "\r\n";
	}
	m_edtUrlList.SetWindowText(mbstowcs(strUrlList).c_str());

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
