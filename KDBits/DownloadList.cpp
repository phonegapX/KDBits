#include "StdAfx.h"
#include "DownloadList.h"

//要做到与界面完全分离 与配置对象也完全分离

CDownloadList g_DownloadList;


CDownloadList::CDownloadList(void)
{
}

CDownloadList::~CDownloadList(void)
{
}

bool CDownloadList::SetTransferSpeed(int iUploadSpeed, int iDownloadSpeed)
{
	m_Session.set_upload_rate_limit(iUploadSpeed>=1?iUploadSpeed*dwBytesOfKilo:iUploadSpeed);
	m_Session.set_download_rate_limit(iDownloadSpeed>=1?iDownloadSpeed*dwBytesOfKilo:iDownloadSpeed);
	return true;
}

bool CDownloadList::BindPort(int iFristPort, int iLastPort)
{
	m_Session.listen_on(std::make_pair(iFristPort, iLastPort));
	return true;
}

void CDownloadList::Initialize(void)
{
	Load();



}

void CDownloadList::Exit(void)
{
	try
	{
		for (node_iterator i = m_List.begin(); i != m_List.end(); ++i)
		{
			torrent_handle & h = const_cast<torrent_handle &>(i->Handle);
			if (!h.is_valid() || !h.has_metadata())
				continue;
			h.pause();
			const entry ResumeData = h.write_resume_data();
			std::stringstream s;
			s << i->Name << ".FastResume";
			bencode(GetFastResumePath()/s.str(), ResumeData);
//			m_Session.remove_torrent(h);
		}
	}
	catch (...){}
	Save();
}

bool CDownloadList::Load(void)
{
	try
	{
		ifstream ifs(GetModulePath()/"KDBits.dl", std::ios::binary);
		boost::archive::binary_iarchive ia(ifs);
		// read class state from archive
		ia >> *this;
	}
	catch (...)
	{
		return false;
	}
	return true;
}

bool CDownloadList::Save(void)
{
	try
	{
		ofstream ofs(GetModulePath()/"KDBits.dl", std::ios::binary);
		boost::archive::binary_oarchive oa(ofs);
		// write class instance to archive
		oa << *this;
	}
	catch (...)
	{
		return false;
	}
	return true;
}

static inline bool operator==(const DownloadNode & Node, const string & Name) //用于查找函数（std::find）
{
	return (Node.Name == Name);
}

void CDownloadList::ParseTorrentFile(path TorrentFile, torrent_info * TorrentInfo)
{
	entry metadata = bdecode(TorrentFile); //读取种子文件并解码
	*TorrentInfo    = metadata;
}

void CDownloadList::AddTask(path TorrentFile, PreDownloadProperty & PreDownProperty, boost::function<bool (torrent_info &, PreDownloadProperty &)> ShowPreDownloadDialog, boost::function<void (boost::function<void (void)>, path)> AsynWaitHandler)
{
	if (std::greater_equal<int>()(static_cast<int>(m_List.size()), PreDownProperty.MaxTaskNum))
	{
		throw(wstring(L"超过了最大任务数限制"));
		return;
	}

	if (TorrentFile.leaf() == TorrentFile)
	{
		TorrentFile = GetTorrentPath()/TorrentFile;
	}

	if(!boost::filesystem::exists(TorrentFile))
	{
		throw(wstring(L"种子文件不存在"));
		return;
	}

	torrent_info t;

	if (AsynWaitHandler == NULL) //同步处理
	{
		ParseTorrentFile(TorrentFile, &t);
	}
	else						 //异步处理
	{
		AsynWaitHandler(bind(&CDownloadList::ParseTorrentFile, this, TorrentFile, &t), TorrentFile);
	}

	if(std::find(m_List.begin(), m_List.end(), t.name()) != m_List.end())
	{
		throw(wstring(L"已经存在相同的下载项目"));
		return;
	}

	entry resume_data;

	if (ShowPreDownloadDialog != NULL)	//点 添加新任务
	{
		if(!ShowPreDownloadDialog(t, PreDownProperty))	//显示预下载对话框
		{
			return;
		}
	}
	else								// 从资源列表中选择下载或是启动的时候继续下载未完成的项目
	{
		if (!PreDownProperty.bReCheckDataCalcSum)
		{
			try
			{
				std::stringstream s;
				s << t.name() << ".FastResume";
				path FastResumeFilePath = GetFastResumePath() / s.str();
				if(boost::filesystem::exists(FastResumeFilePath))
				{
					resume_data = bdecode(FastResumeFilePath);
				}
			}
			catch (libtorrent::invalid_encoding&) {}
			catch (boost::filesystem::filesystem_error&) {}
			catch (...) {}
		}
	}

	string strDescFileName = t.name() + "_External" + boost::filesystem::extension(TorrentFile);
	path   pathDescFile    = GetTorrentPath()/path(strDescFileName);
	string l = pathDescFile.branch_path().string();boost::to_lower(l);
	string r = TorrentFile.branch_path().string();boost::to_lower(r);
	if (l != r)
	{
		try
		{
			boost::filesystem::remove(pathDescFile);
			boost::filesystem::copy_file(TorrentFile, pathDescFile); //将种子文件拷贝到指定目录
		}
		catch (...){}
	}
	else
	{
		pathDescFile = TorrentFile;
	}

	DownloadNode NodeInfo;

	NodeInfo.FileName   = pathDescFile.leaf();
	//NodeInfo.Handle     = m_Session.add_torrent(t, PreDownProperty.SavePath, resume_data, !PreDownProperty.bPreAlloc);
	
	libtorrent::storage_mode_t storage_mode = libtorrent::storage_mode_allocate;
	if (PreDownProperty.bPreAlloc)
	{
		storage_mode = libtorrent::storage_mode_allocate;
	}
	else
	{
		storage_mode = libtorrent::storage_mode_compact;
	}
	NodeInfo.Handle     = m_Session.add_torrent(t, PreDownProperty.SavePath, resume_data, storage_mode);
	
	NodeInfo.Name       = t.name();

	NodeInfo.cols[0]    = mbstowcs(t.name());
	NodeInfo.cols[1]    = (PreDownProperty.UpdateRule == manual ? L"手动更新" : L"自动更新");
	NodeInfo.cols[6]    = boost::lexical_cast<wstring>(formatData(t.total_size()));
	NodeInfo.cols[12]   = mbstowcs((PreDownProperty.SavePath/t.name()).native_directory_string());

	NodeInfo.Handle.set_upload_limit  (PreDownProperty.UploadLimit>=1?PreDownProperty.UploadLimit*dwBytesOfKilo:PreDownProperty.UploadLimit);
	NodeInfo.Handle.set_download_limit(PreDownProperty.DownloadLimit>=1?PreDownProperty.DownloadLimit*dwBytesOfKilo:PreDownProperty.DownloadLimit);

	NodeInfo.Handle.set_max_connections(-1);
	NodeInfo.Handle.set_max_uploads(-1);
	NodeInfo.Handle.set_ratio(0);
	NodeInfo.Handle.set_sequenced_download_threshold(15);
	NodeInfo.Handle.resolve_countries(true);	//peer_info 中包含 ISO 3166 国家识别码

	if(PreDownProperty.RunSetup == down_late)
	{
		NodeInfo.Handle.pause();
	}

	NodeInfo.DownNodeProperty = PreDownProperty;

	m_List.push_back(NodeInfo);

	Save();
}

bool CDownloadList::DeleteTask(string Name)
{
	vector<DownloadNode>::iterator it = std::find(m_List.begin(), m_List.end(), Name);
	if (it == m_List.end())
	{
		return false;
	}
	DownloadNode & Node = *it;
	if (!Node.Handle.is_paused())
	{
		Node.Handle.pause();
	}
	m_Session.remove_torrent(Node.Handle);
	try
	{
		boost::filesystem::remove(GetTorrentPath()/Node.FileName);
		std::stringstream s;
		s << Node.Name << ".FastResume";
		path FastResumeFilePath = GetFastResumePath() / s.str();
		boost::filesystem::remove(FastResumeFilePath);
	}
	catch(...){}
	//从下载项列表中删除
	m_List.erase(it);
	Save();
	return true;
}

void CDownloadList::UpdateState(void)
{
	////////////////////////////////////////////////////////////
	/// Update status bar

	wchar_t buf[32];
/*
	HWND status = GetDlgItem(hwnd, IDC_STATUS);
	libtorrent::session_status s = session->status(); //取总个状态

	StringCchPrintf(buf, 32, L"D: %s/s", strsize(s.download_rate)); //取下载总速度
	SendMessage(status, SB_SETTEXT, 1|SBT_NOBORDERS, (LPARAM)buf);

	StringCchPrintf(buf, 32, L"U: %s/s", strsize(s.upload_rate)); //取上传总速度
	SendMessage(status, SB_SETTEXT, 2|SBT_NOBORDERS, (LPARAM)buf);
*/

	for(vector<DownloadNode>::size_type i = 0; i < m_List.size(); i++)	//遍历所有下载项目
	{
		try
		{
			torrent_status status = m_List[i].Handle.status(); //取每个下载项目的状态

			if(status.paused)  //如果下载项目的状态是暂停，就设置显示字符串
				m_List[i].cols[2] = L"停止"; //用于显示
			else switch(status.state)//根据不同的状态 设置显示字符串
			{
				case libtorrent::torrent_status::queued_for_checking:
					m_List[i].cols[2] = L"正在排队";
					break;
				case libtorrent::torrent_status::checking_files:
					m_List[i].cols[2] = L"正在检查";
					break;
				case libtorrent::torrent_status::connecting_to_tracker:
					m_List[i].cols[2] = L"正在连接";
					break;
				case libtorrent::torrent_status::downloading:
				case libtorrent::torrent_status::downloading_metadata:
					m_List[i].cols[2] = L"正在下载";
					break;
				case libtorrent::torrent_status::finished:
					m_List[i].cols[2] = L"已完成下载(但数据不完整)";
					break;
				case libtorrent::torrent_status::allocating:
					m_List[i].cols[2] = L"正在分配空间";
					break;
				case libtorrent::torrent_status::seeding:
					m_List[i].cols[2] = L"完成(作种中)";
					break;
				default:
					m_List[i].cols[2] = L"未知状态";
					break;
			}

			swprintf(buf, L"%.2f%%", (double)status.progress*100.0); //进度
			m_List[i].cols[3] = buf;

			swprintf(buf, L"%s/s", formatData(status.download_payload_rate));
			m_List[i].cols[4] = buf;

			swprintf(buf, L"%s/s", formatData(status.upload_payload_rate));
			m_List[i].cols[5] = buf;

			m_List[i].cols[7] = formatData((double)status.total_payload_download); //此下载项目 已经下载了多少

			m_List[i].cols[8] = formatData((double)status.total_payload_upload); //此下载项目 已经上传了多少

			swprintf(buf, L"%d%%", (int)(status.distributed_copies*100.0f));
			m_List[i].cols[9] = buf;

			m_List[i].cols[10]  = _itow(status.num_seeds, buf, 10);

			m_List[i].cols[11] = _itow(status.num_peers, buf, 10);
		}
		catch(std::exception &ex) 
		{
			wstring text    = mbstowcs(ex.what());
			MessageBox(0, text.c_str(), 0, MB_ICONERROR|MB_OK);
			continue;
		}
	}
}

const size_t CDownloadList::Count(void)
{
	return m_List.size();
}

const DownloadNode & CDownloadList::operator[](const size_t index)
{
	//TODO: insert return statement here
	return m_List[index];
}

CDownloadList::node_iterator CDownloadList::begin(void)
{
	return m_List.begin();
}

CDownloadList::node_iterator CDownloadList::end(void)
{
	return m_List.end();
}

void CDownloadList::SetDownloadProperty(PreDownloadProperty & DownloadProperty)
{
	for (vector<DownloadNode>::iterator i = m_List.begin(); i != m_List.end(); ++i)
	{
		DownloadNode & Node = *i;
		PreDownloadProperty & NodeProperty = Node.DownNodeProperty;
		//对于正在下载的项目的保存目录,上传限制,下载限制等属性不受全局配置的影响
		//NodeProperty.SavePath      = DownloadProperty.SavePath;
		//NodeProperty.UploadLimit   = DownloadProperty.UploadLimit;
		//NodeProperty.DownloadLimit = DownloadProperty.DownloadLimit;
		NodeProperty.RunSetup      = DownloadProperty.RunSetup;
		NodeProperty.bPreAlloc     = DownloadProperty.bPreAlloc;
		NodeProperty.bReCheckDataCalcSum = DownloadProperty.bReCheckDataCalcSum;
		//对于正在下载的项目的最大任务数也不能受全局配置的影响,这样可以保证软件退出,然后重新启动时继续下
		//未下完的项目的时候不受影响,比如,本来有5个下载任务,然后将全局配置的最大任务数设置为4,那么软件重新启动继续下载
		//的时候还应该是5个项目继续下载,而不能只剩4个任务了
		//NodeProperty.MaxTaskNum    = DownloadProperty.MaxTaskNum;
		//对于是手动更新,还是自动更新这个属性也不能受全局配置的影响
		//NodeProperty.UpdateRule
	}
	Save();
}
