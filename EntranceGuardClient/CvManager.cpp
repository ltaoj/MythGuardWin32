/***********************************************************************
 * Module:  CvManager.cpp
 * Author:  lenovo
 * Modified: 2018��1��30�� 16:03:00
 * Purpose: Implementation of the class CvManager
 ***********************************************************************/

#include "stdafx.h"
#include "CvManager.h"
#include "FaceDetect.h"
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

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
	Mat					frame;						// ��¼ֱ�Ӳ����ÿһ֡
	Mat					result;						// ����ʶ��׷�ٴ�����Ļ���֡
	String				capWndName;					// ��Ƶ���񴰿�����
	FaceDetect			mDetect;					// ����ʶ����
	vector<RECT>		rcFaces;					// ��������
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

		// �оٿ��õ��豸����
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

		// ��������
		// ����ж��������ôֻ������һ�����ӳɹ��Ļ��ߴﵽVFW���Ƶ�9��
		// ����û�����ӳɹ�
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

		winLog->writelog(_T("Start Grab"));
		winLog->writelog(_T("Click MenuItem \"Release\" to Stop Grab"));
		
		capWndName = "�Ž�����ͷ������";
		mDetect.init(16, 10);
		rcFaces.reserve(10);
		while (1)
		{
			// ��ȡ֡
			cap.read(frame);
			if (frame.empty())
				continue;

			// ÿ�����¶�ȡ֡�󿽱�ԭʼͼ�����
			// ������ʾ�Ķ���resultʵ������
			result = frame.clone();
			rcFaces.clear();
			// �������
			mDetect.detect(frame, rcFaces);

			// ����������μ��ϴ�СΪ0�����ʾû��ʶ������
			// �����⵽��������ô��GUI�Ͻ����λ��Ƴ���
			if (rcFaces.size() <= 0)
			{
				// Todo
			}
			else
			{
				// ʶ��������
				// 1. ���������ͼ������Ļ�Ϸֱ����
				// 2. ������̬��ѡ����������Ƕȣ�
				// 3. 
				for (int i = 0; i < rcFaces.size(); i++)
				{
					Point center((rcFaces[i].left + rcFaces[i].right)*0.5, 
						(rcFaces[i].top + rcFaces[i].bottom)*0.5);
					ellipse(result, center, Size((rcFaces[i].right  - rcFaces[i].left)*0.5, 
						(rcFaces[i].bottom - rcFaces[i].top)*0.5), 0, 0, 360, Scalar(0, 255, 0), 4, 8, 0);
				}
			}
			imshow(capWndName, result);

			char c = waitKey(10) & 0xFF;
			if (c == ' ')
			{

			}
			else if (c == 'q' || c == 'Q' || c == 27)
			{
				destroyWindow(capWndName);
				break;
			}
		}
	}
}