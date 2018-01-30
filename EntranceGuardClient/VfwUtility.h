/***********************************************************************
 * Module:  VfwUtility.h
 * Author:  lenovo
 * Modified: 2018Äê1ÔÂ30ÈÕ 16:03:00
 * Purpose: Declaration of the class VfwUtility
 ***********************************************************************/

#if !defined(__guard_VfwUtility_h)
#define __guard_VfwUtility_h

#include "CameraManager.h"

class VfwUtility : public CameraManager
{
public:
   TCHAR * TrueOrFalse(BOOL bTF);
   BOOL dumpCAPDRIVERCAPS(PCAPDRIVERCAPS pCaps);
   BOOL dumpCAPSTATUS(PCAPSTATUS pCaps);
   BOOL dumpCAPTUREPARMS(PCAPTUREPARMS pCaps);
   BOOL SetCaptureFilename(HWND hWnd, HWND hWndCap, TCHAR * szCapFilename, int * nCapFileIndex);
   BOOL SetCapDIBFilename(HWND hWnd, TCHAR * szCapFilename, int * nCapFileIndex);
   // unsigned __stdcall ShowCaptureStatusThread(void* args);
   // LRESULT CALLBACK CapWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
   void ResizeCaptureWindow(HWND hWndChild, HWND hWndCap);
   // LRESULT CALLBACK VideoStreamCallback(HWND hwndC, LPVIDEOHDR lpVHdr);
   // LRESULT PASCAL FrameCallbackProc(HWND hWnd, LPVIDEOHDR lpVHdr);
   void Cls_OnVideoCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify);
   void Cls_OnAudioCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify);

protected:
private:

};

#endif