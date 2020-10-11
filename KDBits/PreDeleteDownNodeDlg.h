#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CPreDeleteDownNodeDlg 对话框

class CPreDeleteDownNodeDlg : public CDialog
{
	DECLARE_DYNAMIC(CPreDeleteDownNodeDlg)

public:
	CPreDeleteDownNodeDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPreDeleteDownNodeDlg();

// 对话框数据
	enum { IDD = IDD_PREDELETEDOWNNODEDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bDelFileThenDelTask;
	vector< pair<string, path> > m_NameList;
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
public:
	CListCtrl m_lcNodeList;
};
