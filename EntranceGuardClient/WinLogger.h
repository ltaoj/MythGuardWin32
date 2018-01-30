// WinLogger.h : include file for the WinLogger object
//

#ifndef _WINLOGGER_
#define _WINLOGGER_

using std::vector;
using std::wstring;

// WinLogger records diagnostic/information messages.
// The log is a vector of wstring, with one input TCHAR buffer.
// 
// The critical section is for atomic I/O
//   
class WinLogger
{
public:
	// c'tor
	WinLogger();
	// d'tor
	~WinLogger();
	// Initialize the logfile with user provided name
	// Default is module name with .txt extension
	//
	bool Init(const TCHAR *pOutputFilename, HINSTANCE hInstance);

	// write to log using a resource string
	// $$ Need to write this so that we don't repeat a lot of the TCHAR * writeLog
	//   void writelog(const UINT uID, ...)
	//{
	//	LPTSTR lpBuffer = NULL;
	//	int cchBufferMax = 0;

	//	LoadString(m_hInstance, uID, lpBuffer, cchBufferMax);
	//}

	// write to log using a string or formatted straing
	void writelog(const TCHAR *szFormat, ...);

	// read from log
	LPCWSTR readlog(size_t lineNum);

	// return length of specified line
	size_t lineLength(size_t lineNum);

	// return number of lines in log
	size_t logSize();

	// Turn timestamping on/off
	bool TimeStamp(bool bStamp);
	// Turn logfile write on/off
	bool WriteLogfile(bool bWriteLogfile);
	// Set the window to be written to
	void SetWindow(HWND hWnd);
	// Get longest line length
	size_t GetLongestLineLength();

protected:
	// Add completed line to log, update iters and write to file if flag set
	void AddLine();

private:
	vector<wstring> m_vsLog;            // vector of strings to hold log
	wstring m_wsLine;                   // log input line
	wstring m_wsEmpty;                  // empty string for out of range returns

										// Use preallocated arrays to optimize for speed
	TCHAR m_szLine[MAX_PATH];       // the log input buffer
	char m_szFileWrite[MAX_PATH];   // output print buffer for ANSI writes
	size_t m_dLength;                   // longest log line length

	bool m_bTimeStamp;                  // Timestamp each line?
	bool m_bWriteLogfile;               // Write log to a file?

	HWND m_hWnd;                        // handle to window to write on
	HANDLE m_hLogfile;                  // handle to logfile
	HINSTANCE m_hInstance;				// handle to application instance
	wstring m_wsLogfile;                // Complete logfile name (including path)

	CRITICAL_SECTION m_crit;            // Critical section for multithread safety
	// Forward and reverse iterators
	vector<wstring>::iterator iBegin;           // FIRST element in arry 
	vector<wstring>::iterator iEnd;             // LAST element in array
	vector<wstring>::reverse_iterator rBegin;   // LAST element in array
	vector<wstring>::reverse_iterator rEnd;     // FIRST element in array
};

#endif
