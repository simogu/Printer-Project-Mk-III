#include "stdafx.h"
#include "PrintJob.h"

PrintJob::PrintJob()
{	
	calculatedTotalPages = 0;
	calculatedPrintedPages = 0;
	printFactor = 1;
}

PrintJob::~PrintJob()
{
}

void PrintJob::setJobInfo(JOB_INFO_2 job)
{
	USES_CONVERSION;

	jobID = job.JobId;
	totalPages = job.TotalPages;
	copies = job.pDevMode->dmCopies;
	printedPages = job.PagesPrinted;
	document = W2A(job.pDocument);
	user = W2A(job.pUserName);
	status = job.Status;
	submitted = job.Submitted;
	size = job.Size;
	calculatedTotalPages = job.TotalPages;
}

void PrintJob::setPrinterInfo(PRINTER_INFO_2 printer) 
{
		printerInfo = printer;
}

PRINTER_INFO_2 PrintJob::getPrinterInfo() {
		return printerInfo;
}

void PrintJob::setPrintFactor(double factor) {
	
		printFactor = factor;
		double TotalPages = totalPages;
		calculatedTotalPages = ceil(TotalPages * printFactor);
}


int PrintJob::getTotalPages() {
		return calculatedTotalPages;
}

int PrintJob::getPagesPrinted() {
		double PrintedPages = printedPages;
		calculatedPrintedPages = ceil(PrintedPages * printFactor);

		return calculatedPrintedPages;
}