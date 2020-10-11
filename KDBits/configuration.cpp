
#include "stdafx.h"
#include "resource.h"
#include "configuration.h"


Configuration g_Configuration;


Configuration::Configuration() :
	UpLimit(-1), DownLimit(-1), ListenPort(19825), OpenUPnP(false), JoinDHT(false), //初始化成员变量
	PreAlloc(false), DelFileThenDelTask(false), DefaultClient(true), AutoRun(false), AutoUpdateNewVersion(true), 
	ReCheckDataCalcSum(false), RunSetup(down_now), MaxTaskNum(5), ProtectDiskSize(0), DiskCacheSize(0)
{
	boost::filesystem::path::default_name_check(boost::filesystem::native);
	NetGamePath   = LoadStringA(IDS_STRING_PATH_NETGAME);   //设置网络游戏下载路径
	LocalGamePath = LoadStringA(IDS_STRING_PATH_LOCALGAME); //设置单机游戏下载路径
	OtherPath     = LoadStringA(IDS_STRING_PATH_OTHER);     //设置其他资源下载路径
}

bool Configuration::Load()
{
	try 
	{
		entry config = bdecode(GetModulePath()/"KDBits.conf");	//读取配置文件并解码
		entry::dictionary_type &config_d = config.dict();

		for(entry::dictionary_type::const_iterator iter = config_d.begin(); iter != config_d.end(); iter++)
		{
			if (iter->first == "Basic")
			{
				const entry::dictionary_type & Basic = iter->second.dict();
				for(entry::dictionary_type::const_iterator niter = Basic.begin(); niter != Basic.end(); niter++)
				{
					if(niter->first == "PreAlloc")
						PreAlloc = niter->second.integer();
					else if(niter->first == "DelFileThenDelTask")
						DelFileThenDelTask = niter->second.integer();
					else if(niter->first == "DefaultClient")
						DefaultClient = (USHORT)niter->second.integer();
					else if(niter->first == "AutoRun")
						AutoRun = (bool)niter->second.integer();
					else if(niter->first == "AutoUpdateNewVersion")
						AutoUpdateNewVersion = (bool)niter->second.integer();
					else if(niter->first == "ReCheckDataCalcSum")
						ReCheckDataCalcSum = niter->second.integer();
					else if(niter->first == "RunSetup")
						RunSetup = static_cast<DOWN_SETUP>(niter->second.integer());
					else if(niter->first == "MaxTaskNum")
						MaxTaskNum = niter->second.integer();
					else if(niter->first == "ProtectDiskSize")
						ProtectDiskSize = niter->second.integer();
					else if(niter->first == "DiskCacheSize")
						DiskCacheSize = niter->second.integer();
				}
			}
			else if(iter->first == "Network")
			{
				const entry::dictionary_type & Network = iter->second.dict();
				for(entry::dictionary_type::const_iterator niter = Network.begin(); niter != Network.end(); niter++) 
				{
					if(niter->first == "UpLimit") 
						UpLimit = niter->second.integer();
					else if(niter->first == "DownLimit") 
						DownLimit = niter->second.integer();
					else if(niter->first == "ListenPort")
						ListenPort = (USHORT)niter->second.integer();
					else if(niter->first == "OpenUPnP")
						OpenUPnP = (bool)niter->second.integer();
					else if(niter->first == "JoinDHT")
						JoinDHT = (bool)niter->second.integer();
					else if(niter->first == "ProxyIP")
						ProxyIP = niter->second.string();
					else if(niter->first == "ProxyPort")
						ProxyPort = niter->second.string();
					else if(niter->first == "ProxyUserName")
						ProxyUserName = niter->second.string();
					else if(niter->first == "ProxyPassword")
						ProxyPassword = niter->second.string();
				}
			}
			else if(iter->first == "FileSystem")
			{
				const entry::dictionary_type &FileSystem = iter->second.dict();
				for(entry::dictionary_type::const_iterator fiter = FileSystem.begin(); fiter != FileSystem.end(); fiter++) 
				{
					if(fiter->first == "NetGamePath")
						NetGamePath = fiter->second.string();
					if(fiter->first == "LocalGamePath")
						LocalGamePath = fiter->second.string();
					if(fiter->first == "OtherPath")
						OtherPath = fiter->second.string();
				}
			}
		}

		if(UpLimit > -1 && UpLimit < 12)
			DownLimit = UpLimit * 5;
	}
	catch(...)
	{
		return false;
	}

	return true;
}

bool Configuration::Save() 
{
	entry::dictionary_type Config;

	{
		entry::dictionary_type Basic;
		Basic.insert(pair<string, entry>("PreAlloc",             PreAlloc));
		Basic.insert(pair<string, entry>("DelFileThenDelTask",   DelFileThenDelTask));
		Basic.insert(pair<string, entry>("DefaultClient",        DefaultClient));
		Basic.insert(pair<string, entry>("AutoRun",              AutoRun));
		Basic.insert(pair<string, entry>("AutoUpdateNewVersion", AutoUpdateNewVersion));
		Basic.insert(pair<string, entry>("ReCheckDataCalcSum",   ReCheckDataCalcSum));
		Basic.insert(pair<string, entry>("RunSetup",             RunSetup));
		Basic.insert(pair<string, entry>("MaxTaskNum",           MaxTaskNum));
		Basic.insert(pair<string, entry>("ProtectDiskSize",      ProtectDiskSize));
		Basic.insert(pair<string, entry>("DiskCacheSize",        DiskCacheSize));

		Config.insert(pair<string,entry>("Basic", Basic));
	}

	{
		entry::dictionary_type Network;
		Network.insert(pair<string, entry>("UpLimit",    UpLimit));
		Network.insert(pair<string, entry>("DownLimit",  DownLimit));
		Network.insert(pair<string, entry>("ListenPort", ListenPort));
		Network.insert(pair<string, entry>("OpenUPnP",   OpenUPnP));
		Network.insert(pair<string, entry>("JoinDHT",    JoinDHT));

		Network.insert(pair<string, entry>("ProxyIP",       ProxyIP));
		Network.insert(pair<string, entry>("ProxyPort",     ProxyPort));
		Network.insert(pair<string, entry>("ProxyUserName", ProxyUserName));
		Network.insert(pair<string, entry>("ProxyPassword", ProxyPassword));

		Config.insert(pair<string,entry>("Network", Network));
	}

	{
		entry::dictionary_type FileSystem;
		FileSystem.insert(pair<string, entry>("NetGamePath",   NetGamePath.native_directory_string()));
		FileSystem.insert(pair<string, entry>("LocalGamePath", LocalGamePath.native_directory_string()));
		FileSystem.insert(pair<string, entry>("OtherPath",     OtherPath.native_directory_string()));

		Config.insert(pair<string,entry>("FileSystem", FileSystem));
	}

	return bencode(GetModulePath()/"KDBits.conf", Config); //编码并写入配置文件
}
