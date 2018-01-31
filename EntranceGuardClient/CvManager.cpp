/***********************************************************************
 * Module:  CvManager.cpp
 * Author:  lenovo
 * Modified: 2018年1月30日 16:03:00
 * Purpose: Implementation of the class CvManager
 ***********************************************************************/

#include "stdafx.h"
#include "CvManager.h"
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;
////////////////////////////////////////////////////////////////////////
// Name:       CvManager::Cls_OnVideoCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify)
// Purpose:    Implementation of CvManager::Cls_OnVideoCommand()
// Parameters:
// - hWnd
// - id
// - hwndCtl
// - codeNotify
// Return:     void
////////////////////////////////////////////////////////////////////////

void CvManager::Cls_OnVideoCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify)
{
	BOOL				bSuccess = FALSE;
	HINSTANCE			hInst = NULL;
	TCHAR				*szStringOne = NULL;		// Two scratch strings
	TCHAR				*szStringTwo = NULL;
	VideoCapture		cap;
	Mat					frame;						// 记录直接捕获的每一帧
	Mat					result;						// 人脸识别、追踪处理过的画面帧
	String				capWndName;					// 视频捕获窗口名称
   // TODO : implement
	switch (id)
	{
	case ID_CAP_PREVIEW:
		winLog->writelog(_T("Capture start - Video for Windows version 0x%x %x"), 
			HIWORD(VideoForWindowsVersion()), LOWORD(VideoForWindowsVersion()));

		// Get the title and class for our Capture window
		szStringOne = szStringTwo = NULL;
		szStringOne = new TCHAR[MAX_PATH + 1];
		szStringTwo = new TCHAR[MAX_PATH + 1];
		if ((NULL == szStringOne) || (NULL == szStringTwo))
		{
			DWORD dwError = GetLastError();
			winLog->writelog(_T("ERROR: memory allocation - Last Error 0x%.4x"), dwError);
			break;
		}
		memset(szStringOne, 0, MAX_PATH * sizeof(szStringOne[0]));
		memset(szStringTwo, 0, MAX_PATH * sizeof(szStringTwo[0]));
		hInst = reinterpret_cast <HINSTANCE> (GetWindowLongPtr(hWnd, GWLP_HINSTANCE));
		LoadString(hInst, IDC_VFW4_CAPWINCLASS, szStringOne, MAX_PATH);
		LoadString(hInst, IDS_CAP_WINDOW_TITLE, szStringTwo, MAX_PATH);

		// 列举可用的设备驱动
		// If there are other Vfw drivers, you'll see them, but modern Windows typically only has the MSVfw mapper
		for (int nDex = 0; nDex < 10; ++nDex)
		{
			if (capGetDriverDescription(nDex,
				szStringOne, MAX_PATH,
				szStringTwo, MAX_PATH))
			{
				winLog->writelog(_T("Vfw device : %s"), szStringOne);
				winLog->writelog(_T("Version    : %s"), szStringTwo);
				break;
			}
		}

		// 连接驱动
		// 如果有多个驱动那么只开启第一个连接成功的或者达到VFW限制的9个
		// 可能没有连接成功
		for (int capNum = 0; (FALSE == bSuccess) && (capNum < 9); ++capNum)
		{
			bSuccess = cap.open(capNum);
		}

		if (FALSE == bSuccess)
		{
			winLog->writelog(_T("ERROR! Unable to open camera"));
			winLog->writelog(_T("capDriverConnect in Windows 7 Vfw will fail to load if there are multiple capture devices"));
			winLog->writelog(_T(""));
			break;
		}
		winLog->writelog(_T("Video Camera - Width: %x, Height: %x"),
			cap.get(CAP_PROP_FRAME_WIDTH), cap.get(CAP_PROP_FRAME_HEIGHT));

		winLog->writelog(_T("开始捕获摄像头"));
		winLog->writelog(_T("点击菜单Release选项终止捕获"));
		
		capWndName = "门禁摄像头捕获画面";
		while (1)
		{
			// 读取帧
			cap.read(frame);
			if (frame.empty())
				continue;

			imshow(capWndName, frame);
			char c = waitKey(10) & 0xFF;
			if (c == ' ')
				
			if (c == 'q' || c == 'Q' || c == 27)
				break;
		}
	}
}