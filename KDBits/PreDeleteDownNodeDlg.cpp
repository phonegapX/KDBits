// PreDeleteDownNodeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "KDBits.h"
#include "PreDeleteDownNodeDlg.h"


// CPreDeleteDownNodeDlg 对话框

IMPLEMENT_DYNAMIC(CPreDeleteDownNodeDlg, CDialog)

CPreDeleteDownNodeDlg::CPreDeleteDownNodeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPreDeleteDownNodeDlg::IDD, pParent)
	, m_bDelFileThenDelTask(FALSE)
{

}

CPreDeleteDownNodeDlg::~CPreDeleteDownNodeDlg()
{
}

void CPreDeleteDownNodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_DELETEDOWNNODE_DELEFILE_CHECK, m_bDelFileThenDelTask);
	DDX_Control(pDX, IDC_DELETEDOWNNODE_NODELIST_LIST, m_lcNodeList);
}


BEGIN_MESSAGE_MAP(CPreDeleteDownNodeDlg, CDialog)
END_MESSAGE_MAP()


// CPreDeleteDownNodeDlg 消息处理程序

BOOL CPreDeleteDownNodeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME), FALSE);		// Set small icon

	// TODO:  在此添加额外的初始化

	m_lcNodeList.InsertColumn(0, L"项目", 0, 120);

	m_lcNodeList.InsertColumn(1, L"文件(目录)", 0, 300);

	ListView_SetExtendedListViewStyle(m_lcNodeList, LVS_EX_FULLROWSELECT|LVS_EX_LABELTIP|LVS_EX_GRIDLINES);

	for (vector< pair<string, path> >::size_type i = 0; i < m_NameList.size(); i++)
	{
		CString strText;
		strText = m_NameList[i].first.c_str();
		m_lcNodeList.InsertItem(i, strText);
		strText = m_NameList[i].second.native_directory_string().c_str();
		m_lcNodeList.SetItemText(i, 1, strText);
	}

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CPreDeleteDownNodeDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	UpdateData(TRUE);
	CDialog::OnOK();
}
