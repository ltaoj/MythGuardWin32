/***********************************************************************
 * Module:  CamaraManager.h
 * Author:  lenovo
 * Modified: 2018��1��30�� 16:03:27
 * Purpose: Declaration of the class CamaraManager
 ***********************************************************************/

#if !defined(__guard_CamaraManager_h)
#define __guard_CamaraManager_h

// class WinLogger;

#include "WinLogger.h"
#include "stdafx.h"

class CameraManager
{
public:
   virtual void Cls_OnVideoCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify)=0;
   void setWinLog(WinLogger &winLog);
   WinLogger* getWinLog(void);

protected:
	WinLogger* winLog;

private:


};

#endif