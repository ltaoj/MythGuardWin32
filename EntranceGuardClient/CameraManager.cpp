/***********************************************************************
 * Module:  CamaraManager.cpp
 * Author:  lenovo
 * Modified: 2018Äê1ÔÂ30ÈÕ 16:03:27
 * Purpose: Implementation of the class CamaraManager
 ***********************************************************************/

#include "stdafx.h"
#include "CameraManager.h"

////////////////////////////////////////////////////////////////////////
// Name:       CamaraManager::setWinLog(WinLogger winLog)
// Purpose:    Implementation of CamaraManager::setWinLog()
// Parameters:
// - winLog
// Return:     void
////////////////////////////////////////////////////////////////////////

void CameraManager::setWinLog(WinLogger& winLog)
{
	this->winLog = &winLog;
}

////////////////////////////////////////////////////////////////////////
// Name:       CamaraManager::getWinLog()
// Purpose:    Implementation of CamaraManager::getWinLog()
// Return:     WinLogger
////////////////////////////////////////////////////////////////////////

WinLogger* CameraManager::getWinLog(void)
{
   // TODO : implement
	return this->winLog;
}