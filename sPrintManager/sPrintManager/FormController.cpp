#include "StdAfx.h"
#include "FormController.h"


FormController::FormController()
{
	init();
}

FormController::~FormController()
{
	pMgr->destroy();
	delete pMgr;
}

void FormController::init()
{
	printerSelected = false;
	curPrinter = NULL;
	pMgr = new PrinterManager();
}

vector<string> FormController::getPrinterListEvent()
{
	vector<string> printerList;

	int numOfPrinters = 0;
	pMgr->refreshList();
	numOfPrinters = pMgr->getNumOfPrinters();

	for ( int i=0; i < numOfPrinters; i++ ) {

		if(pMgr->getPrinter(i).Attributes & PRINTER_ATTRIBUTE_SHARED && pMgr->getPrinter(i).Attributes & PRINTER_ATTRIBUTE_LOCAL)
		{
			USES_CONVERSION;
			string printerName = W2A(pMgr->getPrinter(i).pPrinterName);
			string printerShareName = W2A(pMgr->getPrinter(i).pShareName);

			printerList.push_back(printerShareName);
			
		}
	}

	return printerList;

}

void FormController::setCurrentPrinterEvent(string pName)
{
	curPrinter = pMgr->getPrinter(pName).pPrinterName; //set the curPrinter for ease of use later
	printerSelected = true;
}

template <typename T>
string FormController::toString ( T Number )
{
	ostringstream ss;
	ss << Number;
	return ss.str();
}
vector<vector<string>> FormController::getPrinterJobEvent()
{	
	vector<vector<string>> jobCollection;

	if(printerSelected)
	{
		
		vector<PrintJob> allJobs; //container to capture jobs
		pJobInfo = new JOB_INFO_2;

		pMgr->getPrinterJobs(curPrinter, pJobInfo, allJobs);

		for (vector<PrintJob>::iterator I = allJobs.begin(), E = allJobs.end();
			I != E; ++I) {

				vector<string> jobDetails;
				SYSTEMTIME Submitted = I->submitted;

				USES_CONVERSION;

				string id = toString(I->jobID);
				string username = I->user;
				string document = I->document;
				string jStatus = fetchJobStatus(I->status);
				std::ostringstream stream;

				stream << Submitted.wDay << '/' << Submitted.wMonth << '/' << Submitted.wYear;
				string date = stream.str();
				stream.str("");
				stream << (Submitted.wHour+8)%(12) << ':' << Submitted.wMinute << ':' << Submitted.wSecond;
				string time = stream.str();
				if(Submitted.wHour+8>=12)
				{
					time+=" pm";
				}
				else
				{
					time+=" am";
				}

				string dateTime = date+" "+time;

				stream.str("");
				stream.setf( std::ios::fixed, std:: ios::floatfield ); // floatfield set to fixed
				stream.precision(2);
				stream << double(I->size/1000.0) << "kb";

				string fileSize = stream.str();
				stream.str("");

				jobDetails.push_back(id);
				jobDetails.push_back(document);
				jobDetails.push_back(username);
				jobDetails.push_back(toString(I->getTotalPages()));
				jobDetails.push_back(toString(I->copies));
				jobDetails.push_back(toString(I->getPagesPrinted()));
				jobDetails.push_back(jStatus);		
				jobDetails.push_back(fileSize);
				jobDetails.push_back(dateTime);

				jobCollection.push_back(jobDetails);
		}

		allJobs.clear();
	}
	
	return jobCollection;
}

string FormController::getPrinterStateEvent()
{
	if(curPrinter == NULL)
	{
		return "NOT CONNECTED";
	}
	string currentPrinter;
	USES_CONVERSION;
	currentPrinter = W2A(curPrinter);

	return fetchPrinterStatus(pMgr->getPrinter(currentPrinter).Status);
}

void FormController::refreshSelectedPrinterEvent(string pName)
{
	pMgr->refreshList();
	curPrinter = pMgr->getPrinter(pName).pPrinterName; 
}

void FormController::setPausePrinterEvent()
{
	pMgr->PausePrinter(curPrinter);
}

void FormController::setUnpausePrinterEvent()
{
	pMgr->UnpausePrinter(curPrinter);
}

void FormController::setControlJobEvent(vector<int> jobList, int action)
{
	for(int i = 0; i < jobList.size(); i++)
	{
		pMgr->startJob(curPrinter,jobList.at(i), action);
	}

}
string FormController::fetchPrinterStatus(int status)
{
	if(status == 1)
	{
		return "PAUSED";
	}
	else if(status == 0)
	{
		return "READY";
	}
	else
	{
		return "OTHER/ERROR";
	}
}

string FormController::fetchJobStatus(int status)
{
	if(status == 1)
	{
		return "Paused";
	}
	else if(status == 0)
	{
		return "OK";
	}
	else if(status == 9)
	{
		return "Paused Spooling";
	}
	else if(status == 8208)
	{
		return "Printing";
	}
	else if(status == 8209)
	{
		return "Paused Printing";
	}
	else if(status == 8212)
	{
		return "Deleting";
	}
	else if(status == 8213)
	{
		return "Paused Deleting";
	}
	else if(status == 128 || status == 129)
	{
		return "Printed";
	}
	else if(status == 4097 || status == 4096)
	{
		return "Error";
	}
	else
	{
		std::ostringstream stream;
		stream << status;

		return stream.str();
	}

}

