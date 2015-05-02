#include "StdAfx.h"
#include "Logger.h"
#include "ThreadInfo.h"
#include "PrinterManager.h"
#include <time.h>

using namespace std;

class listenerThread
{
		private:
				PrinterManager* pMgr;
				CThreadInfo m_ThreadInfo; 
				CWinThread *m_pWinThread;
				LPWSTR curPrinterName;
				bool endThread;
				HANDLE curPrinter;
				

		public:
			
				listenerThread(PrinterManager* p);
				~listenerThread();

				void setPrinter(HANDLE printerHandle,  LPWSTR printerName);
				void beginThread();
				void stopThread();

				
				UINT ThreadFunc( void );
				
};
