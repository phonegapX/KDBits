#pragma once


// CAsynWaitDlg 对话框

class CAsynWaitDlg : public CDialog
{
	DECLARE_DYNAMIC(CAsynWaitDlg)

public:
	CAsynWaitDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAsynWaitDlg();

// 对话框数据
	enum { IDD = IDD_ASYNWAITDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	tuple<CAsynWaitDlg *, boost::function<void (void)>, path> m_Tuple;
	afx_msg void OnDestroy();
	CString m_MsgText;
	BOOL    m_ParseError;
};

