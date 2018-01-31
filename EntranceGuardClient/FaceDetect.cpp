/***********************************************************************
 * Module:  FaceDetect.cpp
 * Author:  lenovo
 * Modified: 2018��1��31�� 16:30:02
 * Purpose: Implementation of the class FaceDetect
 ***********************************************************************/

#include "stdafx.h"
#include "FaceDetect.h"
#include <opencv2/core/core.hpp>

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

void FaceDetect::detect(Mat & imgData, vector<RECT> & rcFaces)
{
	// ��Matͼ������ת��ΪASVLOFFSCREEN����
	if (FALSE == convertFromMat(imgData, &offInput))
	{
		return;
	}
	// ������� 
	LPAFD_FSDK_FACERES	FaceRes = NULL;
	nRet = AFD_FSDK_StillImageFaceDetection(hEngine, &offInput, &FaceRes);
	if (nRet != MOK)
	{
		return;
	}
	else
	{
		for (int i = 0; i < FaceRes->nFace;i++)
		{
			RECT rcFace = { 0 };
			rcFace.left = FaceRes->rcFace[i].left;
			rcFace.top = FaceRes->rcFace[i].top;
			rcFace.right = FaceRes->rcFace[i].right;
			rcFace.bottom = FaceRes->rcFace[i].bottom;
			rcFaces.push_back(rcFace);
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
   // TODO : implement
	return TRUE;
}

////////////////////////////////////////////////////////////////////////
// Name:       FaceDetect::init()
// Purpose:    Implementation of FaceDetect::init()
// Return:     bool
////////////////////////////////////////////////////////////////////////

bool FaceDetect::init(MInt32 nScale, MInt32 nMaxFace)
{
	// Ϊ��������ڴ�
	pWorkMem = (MByte *)malloc(WORKBUF_SIZE);
	if (NULL == pWorkMem)
	{
		return FALSE;
	}
	// ��ʼ������
	nRet = AFD_FSDK_InitialFaceEngine(APPID, FDSDKKey, pWorkMem, WORKBUF_SIZE, &hEngine, AFD_FSDK_OPF_0_HIGHER_EXT, nScale, nMaxFace);
	if (nRet != MOK)
	{
		return FALSE;
	}
	return TRUE;
}

////////////////////////////////////////////////////////////////////////
// Name:       FaceDetect::destroy()
// Purpose:    Implementation of FaceDetect::destroy()
// Return:     bool
////////////////////////////////////////////////////////////////////////

bool FaceDetect::destroy(void)
{
	// �ͷ��ڴ������
	nRet = AFD_FSDK_UninitialFaceEngine(hEngine);
	if (nRet != MOK)
	{
		return FALSE;
	}
	free(pWorkMem);
	free(offInput.ppu8Plane[0]);
	return TRUE;
}