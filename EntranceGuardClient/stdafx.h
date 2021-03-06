// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
// Windows 头文件: 
#include <windows.h>
#include <windowsX.h>
#include <ole2.h>
#include <commctrl.h>
#pragma comment( lib, "comctl32.lib" )
#include <shlwapi.h>

// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <process.h>	// for _beginthreadex, _endthreadex


// TODO: 在此处引用程序需要的其他头文件

// Need to include these BEFORE strsafe.h
#include <vfw.h>
#pragma comment(lib, "vfw32.lib")  
#include <string>
#include <vector>

#include <strsafe.h>
#include "WinLogger.h"
#include "resource.h"

// ID for the Preview window hMenu
#define CAP_WINDOW_ID 1297

// 人脸检测、跟踪、识别SDK
#define WORKBUF_SIZE		(40*1024*1024)
#define APPID				"BeQUado7GgZXX7B6tNvRicYsTEhJgmSwFmLRic1zwU71"		//APPID
#define FDSDKKey			"FRaKYgpCgPyPWT2YAqSqNstVPvtSSNSWMvchZTAHfcY3"		//SDKKey