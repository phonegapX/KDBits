
#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#include "DownloadList.h"

class Configuration 
{
public:

	int    UpLimit, DownLimit;					// 上传和下载的速度
	USHORT ListenPort;							// 监听端口
	bool   OpenUPnP, JoinDHT;					// 是否启用UPnP,是否加入DHT

	bool   PreAlloc, DelFileThenDelTask;		// 下载前预先分配磁盘空间, 删除下载任务时同时删除文件
	bool   DefaultClient, AutoRun;				// 关联为默认BT客户端, 系统启动时自动运行
	bool   AutoUpdateNewVersion, ReCheckDataCalcSum; // 启动时自动检查版本更新, 断点续传时重新检查数据完整性
	DOWN_SETUP RunSetup;						// 是立即下载还是稍候下载
	int    MaxTaskNum;							// 最大同时下载的任务数
	int    ProtectDiskSize, DiskCacheSize;		// 当磁盘空间低于指定值就停止下载(单位:MB), 磁盘缓存大小(单位KB)

	string ProxyIP, ProxyPort, ProxyUserName, ProxyPassword;	//代理服务器相关

	boost::filesystem::path NetGamePath;	// 网络游戏保存路径
	boost::filesystem::path LocalGamePath;	// 单机游戏保存路径
	boost::filesystem::path OtherPath;		// 其他资源保存路径

	Configuration();

	bool Load();
	bool Save();
};

extern Configuration g_Configuration;

#endif
