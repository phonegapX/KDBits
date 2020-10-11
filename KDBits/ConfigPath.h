#pragma once

// CConfigPath 对话框

class CConfigPath : public CPropertyPage
{
	DECLARE_DYNAMIC(CConfigPath)

public:
	CConfigPath();
	virtual ~CConfigPath();

// 对话框数据
	enum { IDD = IDD_CONFIGPATH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedNetgameBrw();
	afx_msg void OnBnClickedLocalgameBrw();
	afx_msg void OnBnClickedOtherBrw();
	CString m_NetGamePath;
	CString m_LocalGamePath;
	CString m_OtherPath;
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};
