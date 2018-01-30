/***********************************************************************
 * Module:  CamaraContext.cpp
 * Author:  lenovo
 * Modified: 2018Äê1ÔÂ30ÈÕ 16:02:01
 * Purpose: Implementation of the class CamaraContext
 ***********************************************************************/

#include "stdafx.h"
#include "CameraContext.h"

////////////////////////////////////////////////////////////////////////
// Name:       CamaraContext::CamaraContext()
// Purpose:    Implementation of CamaraContext::CamaraContext()
// Return:     
////////////////////////////////////////////////////////////////////////

CameraContext::CameraContext()
{
   cameraManager = NULL;
}

////////////////////////////////////////////////////////////////////////
// Name:       CamaraContext::~CamaraContext()
// Purpose:    Implementation of CamaraContext::~CamaraContext()
// Return:     
////////////////////////////////////////////////////////////////////////

CameraContext::~CameraContext()
{
   // TODO : implement
	delete cameraManager;
	cameraManager = NULL;
}

////////////////////////////////////////////////////////////////////////
// Name:       CamaraContext::Cls_OnVideoCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify)
// Purpose:    Implementation of CamaraContext::Cls_OnVideoCommand()
// Parameters:
// - hWnd
// - id
// - hwndCtl
// - codeNotify
// Return:     void
////////////////////////////////////////////////////////////////////////

void CameraContext::Cls_OnVideoCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify)
{
	this->cameraManager->Cls_OnVideoCommand(hWnd, id, hwndCtl, codeNotify);
}

////////////////////////////////////////////////////////////////////////
// Name:       CamaraContext::setCameraManager(CameraManager* cameraManager)
// Purpose:    Implementation of CamaraContext::setCameraManager()
// Parameters:
// - cameraManager
// Return:     void
////////////////////////////////////////////////////////////////////////

void CameraContext::setCameraManager(CameraManager* cameraManager)
{
   // TODO : implement
	this->cameraManager = cameraManager;
}

////////////////////////////////////////////////////////////////////////
// Name:       CamaraContext::getCameraManager()
// Purpose:    Implementation of CamaraContext::getCameraManager()
// Return:     CamaraManager*
////////////////////////////////////////////////////////////////////////

CameraManager* CameraContext::getCameraManager(void)
{
   // TODO : implement
	return this->cameraManager;
}