#pragma once
#include "afxdlgs.h"
#include "predownloadgeneralpage.h"
#include "predownloadotherpage.h"
#include "predownloadsetuppage.h"


// CPreDownloadDlg 对话框

class CPreDownloadDlg : public CDialog
{
	DECLARE_DYNAMIC(CPreDownloadDlg)

public:
	CPreDownloadDlg(CWnd* pParent, torrent_info & t, PreDownloadProperty & PreDownProperty);   // 标准构造函数
	virtual ~CPreDownloadDlg();

// 对话框数据
	enum { IDD = IDD_PREDOWNLOADDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	CPropertySheet m_Sheet;
public:
	CPreDownloadGeneralPage m_GeneralPage;
public:
	virtual BOOL OnInitDialog();
public:
	CPreDownloadOtherPage m_OtherPage;
public:
	CPreDownloadSetupPage m_SetupPage;
protected:
	virtual void OnOK();
};
