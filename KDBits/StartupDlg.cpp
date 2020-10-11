// StartupDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "KDBits.h"
#include "StartupDlg.h"
#include "..\tinyxml\tinyxml.h"


// CStartupDlg 对话框

IMPLEMENT_DYNAMIC(CStartupDlg, CDialog)

CStartupDlg::CStartupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStartupDlg::IDD, pParent)
	, m_bDownloadOK(false)
	, m_bQuitExecute(false)
{

}

CStartupDlg::~CStartupDlg()
{
}

void CStartupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STARTUPDLG_PROGRESS, m_ProgressCtrl);
	DDX_Control(pDX, IDC_STARTUPDLG_STATIC, m_TitleView);
}


BEGIN_MESSAGE_MAP(CStartupDlg, CDialog)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CStartupDlg 消息处理程序

bool CStartupDlg::DownloadXmlFile(void)
{
	try
	{
		HINTERNET hSession = InternetOpen(L"Microsoft Internet Explorer", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
		if (hSession == NULL)
		{
			throw(GetLastError());
		}
		HINTERNET hHttpFile = InternetOpenUrl(hSession, L"http://www.gamez88.com/NEP/data/Games.xml", NULL, 0, INTERNET_FLAG_NO_AUTO_REDIRECT, 0);
		if(hHttpFile == NULL)
		{
			throw(GetLastError());
		}
		WCHAR szBuffer[512];
		DWORD dwRead = sizeof(szBuffer);
		if(!HttpQueryInfo(hHttpFile, HTTP_QUERY_STATUS_CODE, szBuffer, &dwRead, NULL))
		{
			throw(GetLastError());
		}
		if(wcscmp(szBuffer, L"200") != 0 && wcscmp(szBuffer, L"206") != 0)
		{
			throw(GetLastError());
		}
		dwRead = sizeof(szBuffer);
		if(!HttpQueryInfo(hHttpFile, HTTP_QUERY_CONTENT_LENGTH, szBuffer, &dwRead,NULL))
		{
			throw(GetLastError());
		}

		double iBlockLen = boost::lexical_cast<double>(szBuffer) / 100.0f;
		int iDownloadedLen = 0;

		path FilePath = GetModulePath();
		FilePath /= "Games.xml";

		std::locale prev_loc = std::locale::global(std::locale(""));
		ofstream fs(FilePath, ofstream::binary);	//二进制方式打开配置文件
		std::locale::global(prev_loc);

		if(!fs.is_open()) //打开成功
		{
			throw(GetLastError());
		}

		while (InternetReadFile(hHttpFile, szBuffer, sizeof(szBuffer), &dwRead) && dwRead != 0)
		{
			fs.write((const char*)szBuffer, dwRead);
			iDownloadedLen += dwRead;
			m_ProgressCtrl.SetPos(iDownloadedLen/iBlockLen);

			MSG msg = {0};
			while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT || m_bQuitExecute)
					throw(0);
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		m_ProgressCtrl.SetPos(100);
		fs.close();
		InternetCloseHandle(hHttpFile);
		InternetCloseHandle(hSession);
		return true;
	}
	catch (...)
	{
		return false;
	}
}

BOOL CStartupDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME), FALSE);		// Set small icon

	m_ProgressCtrl.SetRange(0, 100);
	m_ProgressCtrl.SetPos(0);

	SetTimer(0x512, 500, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CStartupDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == 0x512)
	{
		KillTimer(0x512);
		if (DownloadXmlFile())
		{
			m_TitleView.SetWindowText(L"正在加载资源列表：");
			m_ProgressCtrl.SetPos(0);
			if(LoadGameListFromXmlDoc())
				m_bDownloadOK = true;
		}
		OnOK();
	}
	CDialog::OnTimer(nIDEvent);
}

bool CStartupDlg::IsToday(const string strNow)
{
	bool bResult = false;
	CHAR szBuf[32];
	SYSTEMTIME Now;
	GetSystemTime(&Now);
	sprintf(szBuf, "%u-%02u-%02u", Now.wYear, Now.wMonth, Now.wDay);
	if (strnicmp(szBuf, strNow.c_str(), strlen(szBuf)) == 0)
	{
		bResult = true;
	}
	return bResult;
}

bool CStartupDlg::LoadGameListFromXmlDoc(void)
{
	g_ResInfoList.clear();
	try
	{
		boost::scoped_ptr<TiXmlDocument> myDocument(new TiXmlDocument());
		path filePath = GetModulePath();
		filePath /= "Games.xml";
		if(!myDocument->LoadFile(filePath.string()))
		{
			throw(0);
		}
		TiXmlElement * RootElement = myDocument->RootElement();
		if (RootElement == NULL)
		{
			throw(0);
		}
		TiXmlElement * dataElement = RootElement->FirstChildElement("rs:data");
		if (dataElement == NULL)
		{
			throw(0);
		}
		TiXmlElement * ChildElement = dataElement->FirstChildElement("rs:insert");
		if (ChildElement == NULL)
		{
			throw(0);
		}
		int iBlockCount = 0;
		for (TiXmlElement * nextChildElement = ChildElement->FirstChildElement(); nextChildElement != NULL; nextChildElement = nextChildElement->NextSiblingElement())
		{
			iBlockCount++;
		}
		iBlockCount = (iBlockCount * 10000) / 100;
		int iAddedCount = 0;
		for (TiXmlElement * nextChildElement = ChildElement->FirstChildElement(); nextChildElement != NULL; nextChildElement = nextChildElement->NextSiblingElement())
		{
			ResInfoNode stResNode;
			string strTime;
			string strSize;
			stResNode.DownloadUrl = libtorrent::ConvertUtf8ToLocal(nextChildElement->Attribute("DownloadUrl"));
			stResNode.Name        = libtorrent::ConvertUtf8ToLocal(nextChildElement->Attribute("GameName"));
			stResNode.Type        = libtorrent::ConvertUtf8ToLocal(nextChildElement->Attribute("MainType")); 
			stResNode.Ver         = libtorrent::ConvertUtf8ToLocal(nextChildElement->Attribute("Version")); 
			strSize               = libtorrent::ConvertUtf8ToLocal(nextChildElement->Attribute("GameSize"));			
			strSize.erase(
						std::remove_if(
									strSize.begin(), 
									strSize.end(), 
									bind(
										std::logical_or<bool>(),
										bind(std::less<int>(), _1, 0x30),
										bind(std::greater<int>(), _1, 0x39)
										)
									),
						strSize.end()
						);
			stResNode.Size        = boost::lexical_cast<double>(strSize) * dwBytesOfMega;
			strTime               = libtorrent::ConvertUtf8ToLocal(nextChildElement->Attribute("UpTime"));
			stResNode.Time        = boost::posix_time::time_from_string(strTime);
			stResNode.bNew        = (IsToday(strTime) ? true:false);
			stResNode.UpdateRule  = manual;
			stResNode.UpdateState = state_uninstall;

			stResNode.cols[0] = mbstowcs(stResNode.Name);
			stResNode.cols[1] = mbstowcs(stResNode.Type);
			stResNode.cols[2] = mbstowcs(stResNode.Ver);
			stResNode.cols[3] = mbstowcs(strSize+"MB");
			stResNode.cols[4] = mbstowcs(strTime);
			stResNode.cols[5] = L"手动更新";
			stResNode.cols[6] = L"未下载";
			g_ResInfoList.push_back(stResNode);

			iAddedCount += 10000;
			m_ProgressCtrl.SetPos(iAddedCount/iBlockCount);

			MSG msg = {0};
			while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT || m_bQuitExecute)
					throw(0) ;
				TranslateMessage(&msg);
				DispatchMessage(&msg);     
			}
		}
		return true;
	}
	catch (...)
	{
		return false;
	}
}

void CStartupDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	m_bQuitExecute = true;
	CDialog::OnCancel();
}
