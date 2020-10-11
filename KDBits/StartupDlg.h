#pragma once
#include "afxcmn.h"
#include "TextProgressCtrl\TextProgressCtrl.h"
#include "afxwin.h"

// CStartupDlg 对话框

class CStartupDlg : public CDialog
{
	DECLARE_DYNAMIC(CStartupDlg)

public:
	CStartupDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CStartupDlg();

// 对话框数据
	enum { IDD = IDD_STARTUPDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	bool DownloadXmlFile(void);
public:
	CTextProgressCtrl m_ProgressCtrl;
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	bool m_bDownloadOK;
protected:
	bool LoadGameListFromXmlDoc(void);
	bool IsToday(const string strNow);
public:
	CStatic m_TitleView;
protected:
	virtual void OnCancel();
	bool m_bQuitExecute;
};
