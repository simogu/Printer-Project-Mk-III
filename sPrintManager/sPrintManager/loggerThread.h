#include "StdAfx.h"
#include "Logger.h"
#include "ThreadInfo.h"
#include "PrinterManager.h"
#include <time.h>

using namespace std;

class loggerThread
{
		private:
				vector<pair<string,Logger>> loggerList;
				PrinterManager* pMgr;
				CThreadInfo m_ThreadInfo; 
				CWinThread *m_pWinThread;
				
				LPWSTR curPrinterName;
				bool endThread;
				HANDLE curPrinter;
				
				bool autoDelete;
				int deleteTimeout;
				string currentDate;
				clock_t start, diff;

		public:
				

				loggerThread(PrinterManager* p);
				~loggerThread();

				void initialize();
				void beginThread();
				void stopThread();
				int getDeleteInterval();
				void setDeleteInterval(int interval);
				
				void dateCheck();
				void addRecord(Logger& fileLogger, PrintJob job);
				
				
				UINT ThreadFunc( void );
				
};
