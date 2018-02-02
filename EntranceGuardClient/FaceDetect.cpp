/***********************************************************************
 * Module:  FaceDetect.cpp
 * Author:  lenovo
 * Modified: 2018年1月31日 16:30:02
 * Purpose: Implementation of the class FaceDetect
 ***********************************************************************/

#include "stdafx.h"
#include "FaceDetect.h"

////////////////////////////////////////////////////////////////////////
// Name:       FaceDetect::FaceDetect()
// Purpose:    Implementation of FaceDetect::FaceDetect()
// Return:     
////////////////////////////////////////////////////////////////////////

FaceDetect::FaceDetect()
{
   nRet = MERR_UNKNOWN;
   hEngine = NULL;
   offInput = { 0 };
}

////////////////////////////////////////////////////////////////////////
// Name:       FaceDetect::~FaceDetect()
// Purpose:    Implementation of FaceDetect::~FaceDetect()
// Return:     
////////////////////////////////////////////////////////////////////////

FaceDetect::~FaceDetect()
{
	destroy();
}

////////////////////////////////////////////////////////////////////////
// Name:       FaceDetect::detect(const Mat & imgData, RECT & rcFace)
// Purpose:    Implementation of FaceDetect::detect()
// Parameters:
// - imgData
// - rcFace
// Return:     void
////////////////////////////////////////////////////////////////////////

void FaceDetect::detect(Mat & imgData, VRECT & rcFaces)
{
	// 将Mat图像数据转换为ASVLOFFSCREEN类型
	if (FALSE == convertFromMat(imgData, &offInput))
	{
		return;
	}
	offInput.pi32Pitch[0] = offInput.i32Width * 3;
	// 人脸检测 
	LPAFD_FSDK_FACERES	FaceRes = NULL;
	nRet = AFD_FSDK_StillImageFaceDetection(hEngine, &offInput, &FaceRes);
	if (nRet != MOK)
	{
		WinLogger::getInstance()->writelog(_T("face detect no result，return code:%x"), nRet);
		return;
	}
	else
	{
		WinLogger::getInstance()->writelog(_T("face detect success!"));
		for (int i = 0; i < FaceRes->nFace;i++)
		{
			RECT rcFace = { 0 };
			rcFace.left = FaceRes->rcFace[i].left;
			rcFace.top = FaceRes->rcFace[i].top;
			rcFace.right = FaceRes->rcFace[i].right;
			rcFace.bottom = FaceRes->rcFace[i].bottom;
			rcFaces.push_back(rcFace);
			WinLogger::getInstance()->writelog(_T("face index:%x, left:%d, top:%d, right:%d, bottom:%d"),
				i, rcFace.left, rcFace.top, rcFace.right, rcFace.bottom);
		}
	}
}

////////////////////////////////////////////////////////////////////////
// Name:       FaceDetect::convertFromMat(Mat & matData, LPASVLOFFSCREEN offInput)
// Purpose:    Implementation of FaceDetect::convertFromMat()
// Parameters:
// - matData
// - offInput
// Return:     bool
////////////////////////////////////////////////////////////////////////

bool FaceDetect::convertFromMat(Mat & matData, LPASVLOFFSCREEN offInput)
{
	if (offInput == NULL)
	{
		return FALSE;
	}
	offInput->i32Width = matData.cols;
	offInput->i32Height = matData.rows;

	int biBitCount, channels;

	switch (matData.type())
	{
	case CV_8UC1:
	case CV_8UC2:
	case CV_8UC3:
	case CV_8UC4:
		biBitCount = 8;
		break;
	default:
		biBitCount = 8;
	}
	channels = matData.channels();
	if (8 == biBitCount && channels == 3)
	{
		offInput->u32PixelArrayFormat = ASVL_PAF_RGB24_B8G8R8;
		offInput->ppu8Plane[0] = matData.data;
		/*int lineByte = ((offInput->i32Width) * 3 * channels);

		*(uint8_t** )&offInput->ppu8Plane[0] = (uint8_t *)malloc(lineByte * (offInput->i32Height));
		for (int i = 0; i < offInput->i32Height; i++)
		{
			for (int j = 0; j < offInput->i32Width; j++)
			{
				memcpy((*(uint8_t**)&offInput->ppu8Plane[0]) + i * (offInput->i32Width) * 3 + j * 3,
					matData.data + i * (offInput->i32Width) * 3 + j * 3, 3);
			}
		}*/
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

////////////////////////////////////////////////////////////////////////
// Name:       FaceDetect::init()
// Purpose:    Implementation of FaceDetect::init()
// Return:     bool
////////////////////////////////////////////////////////////////////////

bool FaceDetect::init(MInt32 nScale, MInt32 nMaxFace)
{
	// 为引擎分配内存
	pWorkMem = (MByte *)malloc(WORKBUF_SIZE);
	if (NULL == pWorkMem)
	{
		WinLogger::getInstance()->writelog(_T("ERROR! Memory allocated failed"));
		return FALSE;
	}
	// 初始化引擎
	nRet = AFD_FSDK_InitialFaceEngine(APPID, FDSDKKey, pWorkMem, WORKBUF_SIZE, &hEngine, AFD_FSDK_OPF_0_HIGHER_EXT, nScale, nMaxFace);
	if (nRet != MOK)
	{
		WinLogger::getInstance()->writelog(_T("Engine init failed"));
		return FALSE;
	}
	WinLogger::getInstance()->writelog(_T("Engine init success"));
	return TRUE;
}

////////////////////////////////////////////////////////////////////////
// Name:       FaceDetect::destroy()
// Purpose:    Implementation of FaceDetect::destroy()
// Return:     bool
////////////////////////////////////////////////////////////////////////

bool FaceDetect::destroy(void)
{
	// 只声明对象，而没有调用init()进行初始化的实例不用释放内存和引擎
	if (NULL != hEngine)
	{
		// 释放内存和引擎
		nRet = AFD_FSDK_UninitialFaceEngine(hEngine);
		if (nRet != MOK)
		{
			WinLogger::getInstance()->writelog(_T("ERROR!Engine release failed"));
			return FALSE;
		}
		// 当成员变量是指针类型时，需要显示释放内存，bug date：2018年2月1日23:30:59
		free(pWorkMem);
		//free(offInput.ppu8Plane[0]);
		WinLogger::getInstance()->writelog(_T("Engine release success"));
	}
	return TRUE;
}