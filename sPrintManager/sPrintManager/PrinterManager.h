#ifndef _PRINTERMANAGER_
#define _PRINTERMANAGER_



#include "PrintJob.h"

#include <wincon.h>
#include <winspool.h>
#include <prntvpt.h>

#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
#include "atlstr.h"
#include <string>
#include <vector>
#include <map>



class PrinterManager 
{
		private:
				PRINTER_INFO_2*    list;
				DWORD		numOfPrinters;
				DWORD		sz;
				DWORD		Level;

				std::map<LPTSTR, HANDLE> PrinterHandles;
				std::map<LPWSTR, PRINTER_INFO_2> PrinterData;
				std::map<std::string, std::vector<PrintJob>> PrinterJobCache;

				double getPrintFactor(LPTSTR PrinterName, JOB_INFO_2 pJobInfo);


				std::vector<std::vector<BYTE>> arrlist;


		public:
				PrinterManager();
				~PrinterManager();

				void refreshList();
				void closePrinters();
				
				void destroy();

				PRINTER_INFO_2* getPrinterList();
				PRINTER_INFO_2 getPrinter(int printerNumber);
				PRINTER_INFO_2 getPrinter(std::string printerName);
				bool findPrinter(std::string printerName);

				int getNumOfPrinters();

				bool getPrinterJobs(LPTSTR PrinterName, JOB_INFO_2 *pJobInfo, std::vector<PrintJob> &v);
				bool fetchJobCache(std::string PrinterName, std::vector<PrintJob> &v);
				bool fetchJobFromCache(std::string PrinterName, int id, PrintJob& job);

				HANDLE getPrinterHandle(LPTSTR szPrinterName);

				void updateJobCache(std::string printerName, std::vector<PrintJob> v);
				bool deletePrintedJobs(HANDLE printerHandle, std::string printerName); 
				bool CollateJobs(PRINTER_INFO_2 printer, std::string user, std::vector<PrintJob> &userJob);
				bool UnpausePrinter(LPTSTR szPrinterName);
				bool PausePrinter(LPTSTR szPrinterName);
				bool FetchJobsFromPrinter(LPTSTR szPrinterName, std::vector<PrintJob> &userJob);
				bool startJob(LPTSTR szPrinterName, DWORD JobID, int action);
};



#endif _PRINTERMANAGER_