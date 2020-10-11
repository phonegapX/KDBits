// MakeTorrentFileDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "KDBits.h"
#include "MakeTorrentFileDlg.h"
#include "pathselectdlg.h"

#define STRALLOWDHT    _T("添加 Tracker服务器地址 . HTTP Seeds地址 . DHT网络节点地址")
#define STRNOTALLOWDHT _T("添加 Tracker服务器地址 . HTTP Seeds地址                                                   ")
#define STRDIRFILETYPE _T("目录（多文件）")
// CMakeTorrentFileDlg 对话框

IMPLEMENT_DYNAMIC(CMakeTorrentFileDlg, CDialog)

CMakeTorrentFileDlg::CMakeTorrentFileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMakeTorrentFileDlg::IDD, pParent)
	, m_strDirFileType(STRDIRFILETYPE)
	, m_strPath(_T(""))
	, m_strPieceSize(_T("256   KB"))
	, m_strTitle(STRALLOWDHT)
	, m_strUrlList(_T(""))
	, m_strCreator(_T(""))
	, m_strComment(_T(""))
	, m_bNotAllowDHT(FALSE)
{
}

CMakeTorrentFileDlg::~CMakeTorrentFileDlg()
{
}

void CMakeTorrentFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_CBString(pDX, IDC_MKT_DIRORFILE_COMBO, m_strDirFileType);
	DDX_Text(pDX, IDC_MKT_PATH_EDIT, m_strPath);
	DDV_MaxChars(pDX, m_strPath, 512);
	DDX_CBString(pDX, IDC_MKT_PIECESIZE_COMBO, m_strPieceSize);
	DDX_Text(pDX, IDC_MKT_TITLE_STATIC, m_strTitle);
	DDX_Text(pDX, IDC_MKT_URL_LIST_EDIT, m_strUrlList);
	DDV_MaxChars(pDX, m_strUrlList, 1024);
	DDX_Text(pDX, IDC_MKT_CREATOR_EDIT, m_strCreator);
	DDV_MaxChars(pDX, m_strCreator, 128);
	DDX_Text(pDX, IDC_MKT_COMMENT_EDIT, m_strComment);
	DDX_Radio(pDX, IDC_MKT_ALLOW_DHT_RADIO, m_bNotAllowDHT);
}


BEGIN_MESSAGE_MAP(CMakeTorrentFileDlg, CDialog)
	ON_BN_CLICKED(IDC_MKT_BRW_BUTTON, &CMakeTorrentFileDlg::OnBnClickedMktBrwButton)
	ON_BN_CLICKED(IDC_MKT_ALLOW_DHT_RADIO, &CMakeTorrentFileDlg::OnBnClickedMktAllowDhtRadio)
	ON_BN_CLICKED(IDC_MKT_UNALLOW_DHT_RADIO, &CMakeTorrentFileDlg::OnBnClickedMktUnallowDhtRadio)
	ON_BN_CLICKED(IDOK, &CMakeTorrentFileDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CMakeTorrentFileDlg 消息处理程序

BOOL CMakeTorrentFileDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  在此添加额外的初始化

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CMakeTorrentFileDlg::OnBnClickedMktBrwButton()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_strDirFileType == STRDIRFILETYPE)
	{
		CPathSelectDlg PathSelectDlg(this, L"请选择一个目录");
		if(PathSelectDlg.DoModal())
		{
			m_strPath = PathSelectDlg.m_strPath;
			UpdateData(FALSE);
		}
	}
	else
	{
		CFileDialog FileDlg(TRUE, NULL, NULL, NULL, L"所有文件 (*.*)|*.*||", NULL);
		FileDlg.m_ofn.lpstrTitle = L"请选择一个文件";
		if(FileDlg.DoModal() == IDOK)
		{
			m_strPath = FileDlg.GetPathName();
			UpdateData(FALSE);
		}
	}
}

void CMakeTorrentFileDlg::OnBnClickedMktAllowDhtRadio()
{
	// TODO: 在此添加控件通知处理程序代码
	m_strTitle = STRALLOWDHT;
	m_bNotAllowDHT = FALSE;
	UpdateData(FALSE);
}

void CMakeTorrentFileDlg::OnBnClickedMktUnallowDhtRadio()
{
	// TODO: 在此添加控件通知处理程序代码
	m_strTitle = STRNOTALLOWDHT;
	m_bNotAllowDHT = TRUE;
	UpdateData(FALSE);
}



namespace
{
	void add_files(torrent_info & t, path const & p, path const & l)
	{
		if (l.leaf()[0] == '.')
			return;
		path f(p / l);
		if (is_directory(f))
		{
			for (boost::filesystem::directory_iterator i(f), end; i != end; ++i)
				add_files(t, p, l / i->leaf());
		}
		else
		{
			t.add_file(l, file_size(f));
		}
	}

	void MakeTorrentFile(path & FullPath, path & FileSavePath, int iPieceSize, bool bNotAllowDHT, string & strUrlList, string & strCreator, string & strComment)
	{
		/*
		path::default_name_check(boost::filesystem::no_check);
		try
		{
			torrent_info t;

			std::locale prev_loc = std::locale::global(std::locale(""));
			ofstream out(complete(FileSavePath), std::ios_base::binary);
			std::locale::global(prev_loc);

			t.set_piece_size(iPieceSize * dwBytesOfKilo);

			add_files(t, FullPath.branch_path(), FullPath.leaf());

			file_pool fp;
			storage st(t, FullPath.branch_path(), fp);

			// calculate the hash for all pieces
			int num = t.num_pieces();
			vector<char> buf(iPieceSize);
			for (int i = 0; i < num; ++i)
			{
				st.read(&buf[0], i, 0, t.piece_size(i));
				hasher h(&buf[0], t.piece_size(i));
				t.set_hash(i, h.final());
			}

			//		t.add_tracker(argv[2]);
			//		if (argc == 5)
			//			t.add_url_seed(argv[4]);
			//		add_node()
			//			void add_node(std::pair<std::string, int> const& node);

			t.set_priv(bNotAllowDHT);

			t.set_creator(strCreator.c_str());
			t.set_comment(strComment.c_str());

			// create the torrent and print it to out
			entry e = t.create_torrent();
			libtorrent::bencode(std::ostream_iterator<char>(out), e);
		}
		catch (std::exception& e)
		{
			std::cerr << e.what() << "\n";
		}
		*/
	}
}





void CMakeTorrentFileDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE);

	m_strPath.Trim();
	if (m_strPath.GetLength() == 0)
	{
		AfxMessageBox(L"目标路径不能为空", MB_ICONWARNING);
		return;
	}

	wstring strInitialDir       = wpath(m_strPath.GetBuffer()).branch_path().native_directory_string();
	WCHAR wInitialDir[MAX_PATH] = {0};
	copy(strInitialDir.begin(), strInitialDir.end(), wInitialDir);

	wstring strFile       = wpath(m_strPath.GetBuffer()).leaf();strFile += L".torrent";
	WCHAR wFile[MAX_PATH] = {0};
	copy(strFile.begin(), strFile.end(), wFile);

	CFileDialog FileDlg(FALSE, NULL, NULL, NULL, L"种子文件 (*.torrent)|*.torrent|所有文件 (*.*)|*.*||", NULL);

	FileDlg.m_ofn.lpstrTitle      = L"生成种子文件为";
	FileDlg.m_ofn.lpstrFile       = wFile;
	FileDlg.m_ofn.lpstrInitialDir = wInitialDir;

	if(FileDlg.DoModal() != IDOK)
	{
		return;
	}

	wpath OrgPath  = m_strPath.GetBuffer();
	wpath DescPath = FileDlg.GetPathName().GetBuffer();
	if (OrgPath == DescPath)
	{
		return;
	}
	if (exists(DescPath))
	{
		if (AfxMessageBox(L"目标文件已经存在，是否替换？", MB_ICONWARNING|MB_YESNO) != IDYES)
		{
			return;
		}
	}
	try
	{
		boost::filesystem::remove(DescPath);
	}
	catch (...){}

	wstring strPieceSize = m_strPieceSize.GetBuffer();

	strPieceSize.erase(
		std::remove_if(
			strPieceSize.begin(), 
			strPieceSize.end(), 
			bind(
				std::logical_or<bool>(),
				bind(std::less<int>(), _1, 0x30),
				bind(std::greater<int>(), _1, 0x39)
			)
		),
		strPieceSize.end()
	);

	path FullPath     = wcstombs(OrgPath.string());
	path SaveFilePath = wcstombs(DescPath.string());
	int iPieceSize    = boost::lexical_cast<int>(strPieceSize);
	bool bNotAllowDHT = m_bNotAllowDHT;
	string strUrlList = wcstombs(m_strUrlList.GetBuffer());
	string strCreator = wcstombs(m_strCreator.GetBuffer());
	string strComment = wcstombs(m_strComment.GetBuffer());

	try
	{
		MakeTorrentFile(FullPath, SaveFilePath, iPieceSize, bNotAllowDHT, strUrlList, strCreator, strComment);
	}
	catch (...)
	{
		
	}


	OnOK();
}
