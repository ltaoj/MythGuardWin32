/***********************************************************************
 * Module:  CamaraContext.h
 * Author:  lenovo
 * Modified: 2018��1��30�� 16:02:01
 * Purpose: Declaration of the class CamaraContext
 ***********************************************************************/

#ifndef _CAMARACONTEXT_
#define _CAMARACONTEXT_

class CameraManager;

#include "CameraManager.h"

class CameraContext
{
public:
   CameraContext();
   ~CameraContext();
   void Cls_OnVideoCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify);
   void setCameraManager(CameraManager* cameraManager);
   CameraManager* getCameraManager(void);

protected:
private:
   CameraManager* cameraManager;


};

#endif