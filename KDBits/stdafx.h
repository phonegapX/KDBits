// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// 从 Windows 头中排除极少使用的资料
#endif

// 如果您必须使用下列所指定的平台之前的平台，则修改下面的定义。
// 有关不同平台的相应值的最新信息，请参考 MSDN。
#ifndef WINVER				// 允许使用特定于 Windows XP 或更高版本的功能。
#define WINVER 0x0501		// 将此值更改为相应的值，以适用于 Windows 的其他版本。
#endif

#ifndef _WIN32_WINNT		// 允许使用特定于 Windows XP 或更高版本的功能。
#define _WIN32_WINNT 0x0501	// 将此值更改为相应的值，以适用于 Windows 的其他版本。
#endif						

#ifndef _WIN32_WINDOWS		// 允许使用特定于 Windows 98 或更高版本的功能。
#define _WIN32_WINDOWS 0x0410 // 将它更改为适合 Windows Me 或更高版本的相应值。
#endif

#ifndef _WIN32_IE			// 允许使用特定于 IE 6.0 或更高版本的功能。
#define _WIN32_IE 0x0600	// 将此值更改为相应的值，以适用于 IE 的其他版本。值。
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展


#include <afxdisp.h>        // MFC 自动化类



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>		// MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxdlgs.h>

#include <Wininet.h>

#include <string>
#include <vector>

#ifdef _MSC_VER
#pragma warning(push, 1)
#endif

#include <boost/scoped_array.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#include "libtorrent/torrent_info.hpp"
#include "libtorrent/entry.hpp"
#include "libtorrent/bencode.hpp"
#include "libtorrent/session.hpp"
#include "libtorrent/file.hpp"
#include "libtorrent/storage.hpp"
#include "libtorrent/hasher.hpp"
#include "libtorrent/file_pool.hpp"

using std::string;
using std::wstring;
using std::vector;
using std::pair;
using std::runtime_error;
using boost::filesystem::path;
using boost::filesystem::wpath;
using boost::filesystem::ofstream;
using boost::filesystem::ifstream;
using boost::bind;
using boost::tuple;
using boost::posix_time::ptime;

using libtorrent::entry;
using libtorrent::torrent_handle;
using libtorrent::session;
using libtorrent::torrent_info;
using libtorrent::torrent_status;
using libtorrent::announce_entry;
using libtorrent::file_pool;
//using libtorrent::storage;
using libtorrent::hasher;

enum UPDATE_RULE
{
	automatic = 0,
	manual    = 1
};

enum UPDATE_STATE
{
	state_install   = 0,
	state_uninstall = 1
};

#define COLUMN_NUMBER 7
#define DOWNLOADLIST_COLUMN_NUMBER 13

struct ResInfoNode
{
	string  DownloadUrl;
	string  Name;
	ptime   Time;
	double  Size;
	string  Type;
	string  Ver;
	UPDATE_RULE  UpdateRule;
	UPDATE_STATE UpdateState;
	bool    bNew;
	wstring cols[COLUMN_NUMBER]; //显示字符（这个会绑定到列表控件上，每列绑定一个元素）
};

typedef vector<ResInfoNode> RES_LIST;

extern RES_LIST g_ResInfoList;

wstring mbstowcs(const string &str);

string wcstombs(const wstring &str);

path GetTorrentPath();

path GetFastResumePath();

path GetModulePath();

wstring LoadStringW(UINT id);

string LoadStringA(UINT id);

entry bdecode(const path &file);

bool bencode(const path &file, const entry &e);

wchar_t * formatData(double s);

#define dwBytesOfKilo					1024
#define dwBytesOfMega					(dwBytesOfKilo*dwBytesOfKilo)
#define dwBytesOfGiga					(dwBytesOfMega*dwBytesOfKilo)

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


