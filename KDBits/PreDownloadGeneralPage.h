#pragma once
#include "afxwin.h"
#include "afxcmn.h"

extern struct PreDownloadProperty;
// CPreDownloadGeneralPage 对话框

class CPreDownloadGeneralPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CPreDownloadGeneralPage)

public:
	CPreDownloadGeneralPage(torrent_info & t, PreDownloadProperty & PreDownProperty);
	virtual ~CPreDownloadGeneralPage();

// 对话框数据
	enum { IDD = IDD_PREDOWNLOADGENERALPAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedGeneralBrower();
public:
	CEdit m_editSavePath;
public:
	CStatic m_stDiskSize;
public:
	CStatic m_stName;
public:
	CStatic m_stSize;
public:
	CListCtrl m_lcFileList;
public:
	CImageList m_Image;
public:
	torrent_info & m_TorrentInfo;
	PreDownloadProperty & m_PreDownProperty;
	virtual void OnOK();
};
