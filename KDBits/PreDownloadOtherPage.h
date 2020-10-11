#pragma once
#include "afxwin.h"


// CPreDownloadOtherPage 对话框

class CPreDownloadOtherPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CPreDownloadOtherPage)

public:
	CPreDownloadOtherPage(torrent_info & t);
	virtual ~CPreDownloadOtherPage();

// 对话框数据
	enum { IDD = IDD_PREDOWNLOADOTHERPAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
	torrent_info &m_TorrentInfo;
public:
	CEdit m_edtComment;
public:
	CEdit m_edtCreator;
public:
	CEdit m_edtUrlList;
};
