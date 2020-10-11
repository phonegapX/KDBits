// AsynWaitDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "KDBits.h"
#include "AsynWaitDlg.h"

// CAsynWaitDlg 对话框

IMPLEMENT_DYNAMIC(CAsynWaitDlg, CDialog)

CAsynWaitDlg::CAsynWaitDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAsynWaitDlg::IDD, pParent)
	, m_MsgText(_T("")), m_ParseError(FALSE)
{
}

CAsynWaitDlg::~CAsynWaitDlg()
{
}

void CAsynWaitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ASYNWAITDLG_STATIC, m_MsgText);
}


BEGIN_MESSAGE_MAP(CAsynWaitDlg, CDialog)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


static void ThreadProc(PVOID lpTuple)
{ 
	tuple<CAsynWaitDlg *, boost::function<void (void)>, path> & Tuple = *(tuple<CAsynWaitDlg *, boost::function<void (void)>, path> *)lpTuple;
	CAsynWaitDlg                 * pDlg         = Tuple.get<0>();
	boost::function<void (void)> & ParseHandler = Tuple.get<1>();
	try
	{
		ParseHandler();
	}
	catch (...)
	{
		pDlg->m_ParseError = TRUE;
	}
	::PostMessage(*pDlg, WM_DESTROY, 0, 0);
	_endthread();
}

// CAsynWaitDlg 消息处理程序

BOOL CAsynWaitDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  在此添加额外的初始化

	string strMsg = "正在解析种子文件：" + m_Tuple.get<2>().leaf();
	strMsg       += "    请稍候……";
	m_MsgText = mbstowcs(strMsg).c_str();
	UpdateData(FALSE);

	m_Tuple.get<0>() = this;
	_beginthread(ThreadProc, 0,(void *)&m_Tuple);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CAsynWaitDlg::OnDestroy()
{
	CDialog::OnDestroy();
	// TODO: 在此处添加消息处理程序代码
	EndDialog(IDCANCEL);
}

void AsynWaitHandler(boost::function<void (void)> ParseHandler, path TorrentFile)
{
	CAsynWaitDlg Dlg;
	Dlg.m_Tuple.get<1>() = ParseHandler;
	Dlg.m_Tuple.get<2>() = TorrentFile;
	Dlg.DoModal();
	if (Dlg.m_ParseError)
	{
		throw libtorrent::invalid_encoding();
	}
}
