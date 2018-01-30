/***********************************************************************
 * Module:  CvManager.h
 * Author:  lenovo
 * Modified: 2018Äê1ÔÂ30ÈÕ 16:03:00
 * Purpose: Declaration of the class CvManager
 ***********************************************************************/

#if !defined(__guard_CvManager_h)
#define __guard_CvManager_h

#include "CameraManager.h"

class CvManager : public CameraManager
{
public:
   void Cls_OnVideoCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify);

protected:
private:

};

#endif