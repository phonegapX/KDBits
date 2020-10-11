#pragma once
#include "afxcmn.h"
#include "SortListCtrl\SortListCtrl.h"

// CResListPage 对话框

class CResListPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CResListPage)

public:
	CResListPage();
	virtual ~CResListPage();

// 对话框数据
	enum { IDD = IDD_RESLISTPAGE };

public:
	enum LIST_TYPE { TYPE_NETGAME, TYPE_LOCALGAME, TYPE_OTHER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	CSortListCtrl m_ResListCtrl;
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnLvnGetdispinfoNetgamelist(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnNMRclickNetgamelist(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnResDownload();
public:
	afx_msg void OnResProperty();
public:
	afx_msg void OnNMDblclkNetgamelist(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg LRESULT OnSortList(WPARAM wParam, LPARAM lParam);
protected:
	LIST_TYPE m_ListType;
	RES_LIST  m_ResInfoList;
public:
	void SetTypeClass(LIST_TYPE ListType);
protected:
	CImageList m_Image;
public:
	void UpdateList(void);
};
