#ifndef _PRINTJOB_
#define _PRINTJOB_


#include "stdafx.h"
#include <wincon.h>
#include <winspool.h>
#include <math.h>



class PrintJob
{
		private:

				PRINTER_INFO_2	printerInfo;	

		public:

				int jobID;
				int totalPages;
				int printedPages;
				string document;
				string user;
				SYSTEMTIME submitted;
				int status;
				int copies;
				double size;
				double printFactor;
				int calculatedTotalPages;
				int calculatedPrintedPages;


				PrintJob();
				~PrintJob();

				void setJobInfo(JOB_INFO_2 job);
				void setPrinterInfo(PRINTER_INFO_2 printer);

				
				PRINTER_INFO_2 getPrinterInfo();

				void setPrintFactor(double factor);

				int getTotalPages();
				int getPagesPrinted();
};


#endif