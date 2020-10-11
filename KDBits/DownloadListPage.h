#pragma once
#include "afxcmn.h"
#include "SortListCtrl\SortListCtrl.h"

// CDownloadListPage 对话框

class CDownloadListPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CDownloadListPage)

public:
	CDownloadListPage();
	virtual ~CDownloadListPage();

// 对话框数据
	enum { IDD = IDD_DOWNLOADLISTPAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CSortListCtrl m_DownloadList;
public:
	virtual BOOL OnInitDialog();
protected:
	CImageList m_Image;
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	afx_msg void OnLvnGetdispinfoDownloadlist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRclickDownloadlist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMenuDownlistStartTask();
	afx_msg void OnMenuDownlistStopTask();
	afx_msg void OnMenuDownlistDeleteTask();
	afx_msg void OnMenuDownlistAnnounceTask();
	void EventStartAllTask(void);
	void EventStopAllTask(void);
	void EventDeleteAllTask(void);
private:
	void DeleteTask(string Name, path FullPath, bool bDelPathFile);
public:
	afx_msg void OnMenuDownlistTorrentFileSave();
	afx_msg void OnMenuDownlistTaskProperty();
	afx_msg void OnMenuDownlistOpenDir();
	afx_msg void OnNMDblclkDownloadlist(NMHDR *pNMHDR, LRESULT *pResult);
};
