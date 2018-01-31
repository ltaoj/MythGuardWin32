/***********************************************************************
 * Module:  WinLogger.cpp
 * Author:  lenovo
 * Modified: 2018年1月30日 20:20:09
 * Purpose: Implementation of the class WinLogger
 ***********************************************************************/

#include "stdafx.h"
#include "WinLogger.h"

WinLogger* WinLogger::m_winLog = NULL;
////////////////////////////////////////////////////////////////////////
// Name:       WinLogger::WinLogger()
// Purpose:    Implementation of WinLogger::WinLogger()
// Return:     
////////////////////////////////////////////////////////////////////////

WinLogger::WinLogger() :
	m_bTimeStamp(false), m_bWriteLogfile(true),
	m_wsEmpty(), m_wsLine(), m_dLength(0),
	iBegin(m_vsLog.begin()), iEnd(m_vsLog.end()),
	rBegin(m_vsLog.rbegin()), rEnd(m_vsLog.rend()),
	m_wsLogfile(), m_hLogfile(NULL), m_hInstance(NULL),
	m_hWnd(NULL)
{
	InitializeCriticalSection(&m_crit);
}

////////////////////////////////////////////////////////////////////////
// Name:       WinLogger::~WinLogger()
// Purpose:    Implementation of WinLogger::~WinLogger()
// Return:     
////////////////////////////////////////////////////////////////////////

WinLogger::~WinLogger()
{
	CloseHandle(m_hLogfile);
	DeleteCriticalSection(&m_crit);
}

////////////////////////////////////////////////////////////////////////
// Name:       WinLogger::Init(const TCHAR pOutputFilename, HINSTANCE hInstance)
// Purpose:    Implementation of WinLogger::Init()
// Parameters:
// - pOutputFilename
// - hInstance
// Return:     bool
////////////////////////////////////////////////////////////////////////

bool WinLogger::Init(const TCHAR *pOutputFilename, HINSTANCE hInstance)
{
	TCHAR *pPath = NULL;
	// save the global instance
	m_hInstance = hInstance;

	// do .. while (false) loop to allow easy break for error paths
	do
	{
		// if user provided pOutputFilename is valid, use that and return
		m_hLogfile = CreateFile(pOutputFilename,        // filename
			GENERIC_WRITE,                              // open access
			FILE_SHARE_READ,                            // share access
			NULL,                                       // security descriptor
			CREATE_ALWAYS,                              // creation disposition
			FILE_ATTRIBUTE_NORMAL,                      // flags
			NULL);                                      // template
		if (INVALID_HANDLE_VALUE != m_hLogfile)
		{
			// The filename is valid
			m_wsLogfile = pOutputFilename;
			return true;
		}

		// Initialize a string for the path
		pPath = new TCHAR[MAX_PATH];
		if (NULL == pPath)
		{
			break;
		}
		memset(pPath, 0, MAX_PATH * sizeof(pPath[0]));

		// Get the base current directory
		DWORD dwRet = GetModuleFileName(hInstance, pPath, MAX_PATH);
		if ((0 == dwRet) || (ERROR_INSUFFICIENT_BUFFER == dwRet))
		{
			break;
		}

		// find the last period in the path
		size_t nDex = 0;
		StringCchLength(pPath, MAX_PATH, &nDex);
		if (0 == nDex)
		{
			break;
		}

		while ('.' != pPath[nDex - 1])
		{
			--nDex;
		}
		// Zero-term the path right after that period
		pPath[nDex] = 0;

		// Append the txt to module name
		StringCchCat(pPath, MAX_PATH, _T("txt"));

		// Create the log file
		m_hLogfile = CreateFile(pPath,					// filename
			GENERIC_WRITE,                              // open access
			FILE_SHARE_READ | FILE_SHARE_WRITE,                            // share access
			NULL,                                       // security descriptor
			CREATE_ALWAYS,                              // creation disposition
			FILE_ATTRIBUTE_NORMAL,                      // flags
			NULL);                                      // template
		if (INVALID_HANDLE_VALUE != m_hLogfile)
		{
			// We have a logfile
			m_wsLogfile = pPath;
			delete[] pPath;
			// This is the function exit
			return true;
		}
	} while (false);

	// Not a fatal error, but we won't have a logfile
	delete[] pPath;
	return false;
}

////////////////////////////////////////////////////////////////////////
// Name:       WinLogger::writelog(const TCHAR * szFormat)
// Purpose:    Implementation of WinLogger::writelog()
// Parameters:
// - szFormat
// Return:     void
////////////////////////////////////////////////////////////////////////

void WinLogger::writelog(const TCHAR * szFormat, ...)
{
	{
		// Lock the write
		EnterCriticalSection(&m_crit);

		// Clear input buffer and line
		memset(m_szLine, 0, (MAX_PATH) * sizeof(m_szLine[0]));
		m_wsLine.clear();

		// Preface time stamp, if requested
		if (m_bTimeStamp)
		{
			SYSTEMTIME stLocal;
			TCHAR szTime[MAX_PATH] = { 0 };
			DWORD dwSize = MAX_PATH;
			GetLocalTime(&stLocal);
			DWORD dwRet = StringCchPrintf(szTime, dwSize,
				TEXT("%02d/%02d/%d  %02d:%02d:%02d    "),
				stLocal.wMonth, stLocal.wDay, stLocal.wYear,
				stLocal.wHour, stLocal.wMinute, stLocal.wSecond);
			if (S_OK == dwRet)
			{
				m_wsLine.append(szTime);
			}
		}

		// parse the string format if it's non-null
		if (szFormat)
		{
			try
			{
				va_list vargp;
				va_start(vargp, szFormat);
				_vsnwprintf_s(m_szLine, (MAX_PATH), (MAX_PATH)-1, szFormat, vargp);
				va_end(vargp);
			}
			catch (...)
			{
				// Could not parse line, so just print a blank line
				m_szLine[0] = 0;
			}
		}

		// Now write the formated line to the log input line
		m_wsLine.append(m_szLine);
		AddLine();

		// Done
		LeaveCriticalSection(&m_crit);
		return;
	}
}

////////////////////////////////////////////////////////////////////////
// Name:       WinLogger::readlog(size_t lineNum)
// Purpose:    Implementation of WinLogger::readlog()
// Parameters:
// - lineNum
// Return:     LPCWSTR
////////////////////////////////////////////////////////////////////////

LPCWSTR WinLogger::readlog(size_t lineNum)
{
	// return empty string on out of range request
	if (lineNum >= m_vsLog.size())
	{
		return m_wsEmpty.c_str();
	}
	return m_vsLog[lineNum].c_str();
}

////////////////////////////////////////////////////////////////////////
// Name:       WinLogger::lineLength(size_t lineNum)
// Purpose:    Implementation of WinLogger::lineLength()
// Parameters:
// - lineNum
// Return:     size_t
////////////////////////////////////////////////////////////////////////

size_t WinLogger::lineLength(size_t lineNum)
{
	// return empty string on out of range request
	if (lineNum >= m_vsLog.size())
	{
		return m_wsEmpty.length();
	}
	return m_vsLog[lineNum].length();
}

////////////////////////////////////////////////////////////////////////
// Name:       WinLogger::logSize()
// Purpose:    Implementation of WinLogger::logSize()
// Return:     size_t
////////////////////////////////////////////////////////////////////////

size_t WinLogger::logSize(void)
{
	return m_vsLog.size();
}

////////////////////////////////////////////////////////////////////////
// Name:       WinLogger::TimeStamp(bool bStamp)
// Purpose:    Implementation of WinLogger::TimeStamp()
// Parameters:
// - bStamp
// Return:     bool
////////////////////////////////////////////////////////////////////////

bool WinLogger::TimeStamp(bool bStamp)
{
	return (m_bTimeStamp = bStamp);
}

////////////////////////////////////////////////////////////////////////
// Name:       WinLogger::WriteLogfile(bool bWriteLogfile)
// Purpose:    Implementation of WinLogger::WriteLogfile()
// Parameters:
// - bWriteLogfile
// Return:     bool
////////////////////////////////////////////////////////////////////////

bool WinLogger::WriteLogfile(bool bWriteLogfile)
{
	return (m_bWriteLogfile = bWriteLogfile);
}

////////////////////////////////////////////////////////////////////////
// Name:       WinLogger::SetWindow(HWND hWnd)
// Purpose:    Implementation of WinLogger::SetWindow()
// Parameters:
// - hWnd
// Return:     void
////////////////////////////////////////////////////////////////////////

void WinLogger::SetWindow(HWND hWnd)
{
	m_hWnd = hWnd;
	return;
}

////////////////////////////////////////////////////////////////////////
// Name:       WinLogger::GetLongestLineLength()
// Purpose:    Implementation of WinLogger::GetLongestLineLength()
// Return:     size_t
////////////////////////////////////////////////////////////////////////

size_t WinLogger::GetLongestLineLength(void)
{
	return m_dLength;
}

////////////////////////////////////////////////////////////////////////
// Name:       WinLogger::AddLine()
// Purpose:    Implementation of WinLogger::AddLine()
// Return:     void
////////////////////////////////////////////////////////////////////////

void WinLogger::AddLine(void)
{
	// Add the completed wsLine to vsLog
	m_vsLog.push_back(m_wsLine);
	// Update forward and reverse iters
	iBegin = m_vsLog.begin();
	iEnd = m_vsLog.end();
	rBegin = m_vsLog.rbegin();
	rEnd = m_vsLog.rend();

	// Save length of longest line in log for horizontal scrolling
	size_t dLength = m_wsLine.size();
	if (dLength > m_dLength)
	{
		m_dLength = dLength;
	}

	if ((m_bWriteLogfile) && (m_hLogfile))
	{
		// Zero-init file write buffer and copy our wchar string
		memset(m_szFileWrite, 0, (MAX_PATH));
		size_t nCharLen(0);
		size_t sWideLen = m_wsLine.size() + 1;
		wcstombs_s(&nCharLen, m_szFileWrite, sWideLen, m_wsLine.c_str(), (MAX_PATH));

		// Add CR/LF to line, and write to file
		m_szFileWrite[nCharLen - 1] = '\r';
		m_szFileWrite[nCharLen++] = '\n';
		DWORD dwBytesWritten = 0;
		BOOL fSuccess = WriteFile(m_hLogfile, m_szFileWrite, static_cast<DWORD> (nCharLen), &dwBytesWritten, NULL);
	}
	// If we know the window we're writing to, ask it to update
	if (m_hWnd)
	{
		InvalidateRect(m_hWnd, NULL, true);
	}
	return;
}

////////////////////////////////////////////////////////////////////////
// Name:       WinLogger::getInstance(const TCHAR pOutputFilename, HINSTANCE hInstance)
// Purpose:    Implementation of WinLogger::getInstance()
// Parameters:
// - pOutputFilename
// - hInstance
// Return:     WinLogger*
////////////////////////////////////////////////////////////////////////

WinLogger* WinLogger::getInstance(const TCHAR* pOutputFilename, HINSTANCE hInstance)
{
	// TODO : implement
	if (NULL == m_winLog)
	{
		// 加锁
		if (NULL == m_winLog)
		{
			m_winLog = new WinLogger();
			m_winLog->Init(pOutputFilename, hInstance);
		}
		// 释放锁
	}
	return m_winLog;
}

////////////////////////////////////////////////////////////////////////
// Name:       WinLogger::getInstance()
// Purpose:    Implementation of WinLogger::getInstance()
// Return:     WinLogger*
////////////////////////////////////////////////////////////////////////

WinLogger* WinLogger::getInstance(void)
{
	return m_winLog;
}