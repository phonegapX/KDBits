#pragma once


// CPeerListPage 对话框

class CPeerListPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CPeerListPage)

public:
	CPeerListPage();
	virtual ~CPeerListPage();

// 对话框数据
	enum { IDD = IDD_PEERLISTPAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
