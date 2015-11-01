#include "StdAfx.h"
#include "FormController.h"



FormController::FormController()
{
	init();
}

FormController::~FormController()
{
	//stop all threads
	for (unsigned i=0; i<threadList.size(); i++) {
		//printf("Stopping thread %S\n", _printMgr->getPrinter(i).pPrinterName);
		threadList.at(i)->stopThread();
		delete threadList.at(i);
	}

	pMgr->destroy();
	delete pMgr;

}

void FormController::init()
{
	autoPauseControl = false;
	printerSelected = false;
	curPrinter = NULL;
	pMgr = new PrinterManager();

	//initialize jobCodes
	int jList[] = {(int)JOB_STATUS_RENDERING_LOCALLY, (int)JOB_STATUS_RETAINED, (int)JOB_STATUS_COMPLETE,
		(int)JOB_STATUS_RESTART, (int)JOB_STATUS_USER_INTERVENTION, (int)JOB_STATUS_BLOCKED_DEVQ,
		(int)JOB_STATUS_DELETED, (int)JOB_STATUS_PRINTED, (int)JOB_STATUS_PAPEROUT,
		(int)JOB_STATUS_OFFLINE, (int)JOB_STATUS_PRINTING, (int)JOB_STATUS_SPOOLING,
		(int)JOB_STATUS_DELETING, (int)JOB_STATUS_ERROR, (int)JOB_STATUS_PAUSED};

	std::copy(std::begin(jList), std::end(jList), std::begin(jobStatusList));

	string sList[] = {"RenderingLocally","Retained","Complete","Restarting","UserIntervention",
						"BlockedDeVQ","Deleted","Printed","PaperOut",
						"Offline","Printing","Spooling","Deleting","Error","Paused"};

	std::copy(std::begin(sList), std::end(sList), std::begin(statusStringList));

	currentJob = -1;

}

void FormController::runBlockerThreads()
{

	int numOfPrinters = 0;
	pMgr->refreshList();
	numOfPrinters = pMgr->getNumOfPrinters();

	for ( int i=0; i < numOfPrinters; i++ ) {

		if(pMgr->getPrinter(i).Attributes & PRINTER_ATTRIBUTE_SHARED && pMgr->getPrinter(i).Attributes & PRINTER_ATTRIBUTE_LOCAL)
		{
			
			HANDLE h = pMgr->getPrinterHandle(pMgr->getPrinter(i).pPrinterName);

			if(h!=NULL)
			{
				//3 threads each
				for(int j=0;j<3;j++)
				{
					//printf("Threading %S\n", _printMgr->getPrinter(i).pPrinterName);
					listenerThread *lThread = new listenerThread(pMgr);	//listenerthread for print notifications
					lThread->setPrinter(h,pMgr->getPrinter(i).pPrinterName);
					lThread->beginThread();	//call beginThread to start monitoring the set printer
					threadList.push_back(lThread);
				
				}

			}

		}
	}
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

	if(autoPauseControl)
	{
		if(jobQueue.size()>0)
		{
			if(currentJob!=jobQueue.front())
			{
				currentJob = jobQueue.front();
				cout<<"next to print: "<<currentJob<<endl;
				setUnpausePrinterEvent();
			}
		}	
	}
	bool jobFound = false;

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

				if(autoPauseControl)
				{
					if(I->jobID==currentJob)
					{
						jobFound = true;
						if(jStatus.find("Printing") != std::string::npos) 
						{
							//consider case of paused printing
							if(!jStatus.find("Paused") == std::string::npos)
							{
								//setPausePrinterEvent();
							}
						}
						else if(jStatus.find("Printed") != std::string::npos) 
						{
							jobQueue.pop();
						}
						else if(jStatus.find("Ok") != std::string::npos) 
						{
							setUnpausePrinterEvent();
						}
					}	
				}
		}
		
		allJobs.clear();

		if(autoPauseControl)
		{
			if(!jobFound && jobQueue.size()>0)
			{
				if(currentJob == jobQueue.front())
				{
					//if job is at top of queue, also current job and cannot be found in the list
					//kick it.
					jobQueue.pop();
				}
			}
		}
		
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
		
		if(autoPauseControl)
		{
			if(action==2)
			{
				jobQueue.emplace(jobList.at(i));
			}
		}
	}
}

void FormController::setJobQueueInitialList(vector<int> jobList)
{
	for(int i=0;i<jobList.size();i++)
	{
		jobQueue.emplace(jobList.at(i));
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
	string jobCode = "";
	//cout<<status<<endl;
	if(status == 0)
	{
		jobCode+="Ok";
	}
	else
	{
		while(status!=0)
		{
			for(int i=0;i<15;i++)
			{
				if(status - jobStatusList[i] >= 0)
				{
					jobCode = statusStringList[i] + " " + jobCode;
					status = status - jobStatusList[i];
				}			
			}
		}
	}
	return jobCode;
}

void FormController::toggleAutoPauseControl(bool toggle)
{
	autoPauseControl = toggle;

	if(toggle==false)
	{
		while(jobQueue.size()>0)
		{
			jobQueue.pop();
		}
		currentJob = -1;
	}
	

}

