#include "stdafx.h"
#include "loggerThread.h"

loggerThread::loggerThread(PrinterManager* p)
{
	pMgr = p;
	endThread = false;
	autoDelete = false;
	deleteTimeout = -60;
	start = clock();
}

loggerThread::~loggerThread()
{
	loggerList.clear();
}

void loggerThread::initialize()
{
	loggerList.clear();
	
	USES_CONVERSION;
	
	time_t now = time(0);
	tm *ltm = localtime(&now);

	std::ostringstream stream;

	stream << ltm->tm_mday <<'-'<< 1 + ltm->tm_mon <<'-'<< 1900 + ltm->tm_year;
	string date = stream.str();

	currentDate = date;

	int numOfPrinters = pMgr->getNumOfPrinters();

	for (int i=0; i<numOfPrinters; i++) {
			//to use printNotification thread, set the handle of the printer. Printer name is just incase
			HANDLE h = pMgr->getPrinterHandle(pMgr->getPrinter(i).pPrinterName);

			if(h!=NULL)
			{
				Logger newLog;
				string pName = W2A(pMgr->getPrinter(i).pPrinterName);
				
				newLog.init(pName, date);
				pair<string,Logger> p;

				p.first = pName;
				p.second = newLog;
				loggerList.push_back(p);

			}
			
	}
	
}

UINT LoggerThreadFunc( LPVOID pParam )
{
    loggerThread *lThread = (loggerThread *) pParam;
    
    return lThread->ThreadFunc();
}

void loggerThread::beginThread()
{
	AfxBeginThread(::LoggerThreadFunc, this);
}

void loggerThread::stopThread()
{
	endThread = true;
}

int loggerThread::getDeleteInterval()
{
	return deleteTimeout;
}

void loggerThread::setDeleteInterval(int interval)
{
	if(interval>0)
	{
		deleteTimeout = interval;
		start = clock();
		autoDelete= true;
	}
	else
	{
		deleteTimeout = interval;
		autoDelete = false;
	}
	
}
UINT loggerThread::ThreadFunc( void )
{
	
    while (!endThread)
    {
		Sleep(2000);
		//cout<<"house keeping..."<<endl;

		dateCheck();

		for(unsigned int i=0;i<loggerList.size();i++)
		{
			vector<PrintJob> jobList;

			JOB_INFO_2* pJobInfo = new JOB_INFO_2;
			pMgr->getPrinterJobs(pMgr->getPrinter(loggerList.at(i).first).pPrinterName, pJobInfo,jobList);
			
			for(unsigned int j=0;j<jobList.size();j++)
			{
				if(jobList.at(j).status==128 || jobList.at(j).status==129)
				{
					//cout<<"Job Printed! ID: "<<jobList.at(j).jobID<<endl;
					addRecord(loggerList.at(i).second,jobList.at(j));
				}
			}

			loggerList.at(i).second.save();


			if(autoDelete)
			{
				diff = clock() - start;
				int msec = diff / CLOCKS_PER_SEC;
				//cout<<msec<<loggerList.at(i).first<<endl;

				if(msec>=deleteTimeout)
				{
					pMgr->updateJobCache(loggerList.at(i).first,jobList);
					pMgr->deletePrintedJobs(pMgr->getPrinterHandle(pMgr->getPrinter(loggerList.at(i).first).pPrinterName), 
						loggerList.at(i).first);

					if(i>=loggerList.size()-1)
					{
						start = clock();
					}
				}
			}

			if(pJobInfo!=NULL)
			{
				delete pJobInfo;
				pJobInfo = NULL;
					
			}
		}
		
		
		
    }
	
    // signal the event to let the primary thread know that this thread is done
    SetEvent(m_ThreadInfo.GetThreadDoneEvent());
	endThread = false;

    return 0;
}


void loggerThread::dateCheck()
{
	time_t now = time(0);
	tm *ltm = localtime(&now);

	std::ostringstream stream;

	stream << ltm->tm_mday <<'-'<< 1 + ltm->tm_mon <<'-'<< 1900 + ltm->tm_year;
	string date = stream.str();

	if((int)currentDate.compare(date)!=0)
	{
		cout<<endl<<"Notice: Generating new logs for "<<date<<"..."<<endl;
		for(unsigned int i=0;i<loggerList.size();i++)
		{
			loggerList.at(i).second.init(loggerList.at(i).first,date);
		}
		currentDate = date;
	}
}
void loggerThread::addRecord(Logger& fileLogger,PrintJob job)
{
	try
	{
		USES_CONVERSION;
			
		ostringstream stream;
		string username = job.user;
		string document = job.document;
		SYSTEMTIME Submitted = job.submitted;
						
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
		stream.str("");
		stream<<job.jobID<<"_"<<time;

		if(!fileLogger.recordExists(stream.str()))
		{
			//cout<<"Adding to record ID:"<<job.jobID<<endl;
			fileLogger.update(stream.str());
			stream.str("");

			string record ="<Row ss:AutoFitHeight=\"0\">\n";
			stream << "<Cell><Data ss:Type=\"Number\">"<<job.jobID<<"</Data></Cell>\n";
			record+=stream.str();
			stream.str("");
			stream << "<Cell><Data ss:Type=\"String\">"<<username<<"</Data></Cell>\n";
			record+=stream.str();
			stream.str("");
			stream << "<Cell><Data ss:Type=\"Number\">"<<job.getTotalPages()<<"</Data></Cell>\n";
			record+=stream.str();
			stream.str("");
			stream << "<Cell><Data ss:Type=\"Number\">"<<job.copies<<"</Data></Cell>\n";
			record+=stream.str();
			stream.str("");
			stream << "<Cell><Data ss:Type=\"String\">"<<time<<"</Data></Cell>\n";
			record+=stream.str();
			stream.str("");
			stream << "<Cell><Data ss:Type=\"String\">"<<document<<"</Data></Cell>\n";
			record+=stream.str();
			stream.str("");
			record+="</Row>\n";

			fileLogger.record(record);
				
		}
									
		
	}
	catch(...)
	{
		cout<<"add record crash caught"<<endl;
	}

}



