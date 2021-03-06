#include "stdafx.h"
#include "listenerThread.h"

listenerThread::listenerThread(PrinterManager* p)
{
	pMgr = p;
	endThread = false;
	
}

listenerThread::~listenerThread()
{

}

void listenerThread::setPrinter(HANDLE printerHandle,  LPWSTR printerName)
{
	m_ThreadInfo.SetPrinter(printerHandle);
	curPrinterName = printerName;
	curPrinter = printerHandle;
}

UINT ThreadFunc( LPVOID pParam )
{
    listenerThread *lThread = (listenerThread *) pParam;
    
    return lThread->ThreadFunc();
}

void listenerThread::beginThread()
{
	
	AfxBeginThread(::ThreadFunc, this);
}

void listenerThread::stopThread()
{
	endThread = true;
}

UINT listenerThread::ThreadFunc( void )
{
	PPRINTER_NOTIFY_INFO pNotification = NULL;

	WORD JobFields[] = 
    {
        JOB_NOTIFY_FIELD_PRINTER_NAME,
        JOB_NOTIFY_FIELD_MACHINE_NAME,
        JOB_NOTIFY_FIELD_PORT_NAME,
        JOB_NOTIFY_FIELD_USER_NAME,
        JOB_NOTIFY_FIELD_NOTIFY_NAME,
        JOB_NOTIFY_FIELD_DATATYPE,
        JOB_NOTIFY_FIELD_PRINT_PROCESSOR,
        JOB_NOTIFY_FIELD_PARAMETERS,
        JOB_NOTIFY_FIELD_DRIVER_NAME,
        JOB_NOTIFY_FIELD_DEVMODE,
        JOB_NOTIFY_FIELD_STATUS,
        JOB_NOTIFY_FIELD_STATUS_STRING,
        JOB_NOTIFY_FIELD_DOCUMENT,
        JOB_NOTIFY_FIELD_PRIORITY,
        JOB_NOTIFY_FIELD_POSITION,
        JOB_NOTIFY_FIELD_SUBMITTED,
        JOB_NOTIFY_FIELD_START_TIME,
        JOB_NOTIFY_FIELD_UNTIL_TIME,
        JOB_NOTIFY_FIELD_TIME,
        JOB_NOTIFY_FIELD_TOTAL_PAGES,
        JOB_NOTIFY_FIELD_PAGES_PRINTED,
        JOB_NOTIFY_FIELD_TOTAL_BYTES,
        JOB_NOTIFY_FIELD_BYTES_PRINTED
    };
	PRINTER_NOTIFY_OPTIONS_TYPE	Notifications[1] =					
    {
        {
            JOB_NOTIFY_TYPE,
            0,
            0,
            0,
            sizeof(JobFields) / sizeof(JobFields[0]),
            JobFields
        },
    };
	PRINTER_NOTIFY_OPTIONS NotificationOptions = 
    {
        2,
        PRINTER_NOTIFY_OPTIONS_REFRESH,
        sizeof(Notifications) / sizeof(Notifications[0]),
        Notifications
    };

    // get a handle to a printer change notification object.
    HANDLE hChange = FindFirstPrinterChangeNotification(m_ThreadInfo.GetPrinter(),
                                                        PRINTER_CHANGE_JOB,
                                                        0, 
                                                        &NotificationOptions);

    DWORD dwChange;
    HANDLE aHandles[2];
    aHandles[0] = hChange;
    aHandles[1] = m_ThreadInfo.GetStopRequestedEvent();

	USES_CONVERSION;
	string printerName = W2A(curPrinterName);
	//cout<<printerName<<": Listener Thread started"<<endl;
	
    while (hChange != INVALID_HANDLE_VALUE)
    {
		Sleep(10);
		
        // sleep until a printer change notification wakes this thread or the
        // event becomes set indicating it's time for the thread to end.
		
        WaitForMultipleObjects(2, aHandles, FALSE, INFINITE);

        if (WaitForSingleObject(hChange, 0U) == WAIT_OBJECT_0)
        {
			
            FindNextPrinterChangeNotification(hChange, &dwChange, &NotificationOptions, (LPVOID *) &pNotification);
		
			if(pNotification == NULL)
			{
				cout<<endl<<"Error:notification is null"<<endl;
			}
			if (pNotification != NULL)
            {
				// iterate through each notification
				for (DWORD x = 0; x < pNotification->Count; x++)
				{	
					/*
						JOB_STATUS_PAUSED = 0x00000001,
						JOB_STATUS_ERROR = 0x00000002,
						JOB_STATUS_DELETING = 0x00000004,
						JOB_STATUS_SPOOLING = 0x00000008,
						JOB_STATUS_PRINTING = 0x00000010,
						JOB_STATUS_OFFLINE = 0x00000020,
						JOB_STATUS_PAPEROUT = 0x00000040,
						JOB_STATUS_PRINTED = 0x00000080,
						JOB_STATUS_DELETED = 0x00000100,
						JOB_STATUS_BLOCKED_DEVQ = 0x00000200,
						JOB_STATUS_USER_INTERVENTION = 0x00000400,
						JOB_STATUS_RESTART = 0x00000800,
						JOB_STATUS_COMPLETE = 0x00001000,
						JOB_STATUS_RETAINED = 0x00002000,
						JOB_STATUS_RENDERING_LOCALLY = 0x00004000,
					*/

					if(pNotification->aData[x].Type == JOB_NOTIFY_TYPE)
					{
						if(pNotification->aData[x].NotifyData.Data.cbBuf == JOB_STATUS_SPOOLING)
						{
							PrintJob job;

							bool result = pMgr->fetchJobFromCache(printerName,pNotification->aData[x].Id,job);
							if(!result)
							{
								SetJob(m_ThreadInfo.GetPrinter(), pNotification->aData[x].Id, 0, NULL, 1);
								//cout<<endl<<"Listener Thread: Job notification received! Id:"<<pNotification->aData[x].Id<<endl;	
							}
							else{
								if(job.status == JOB_STATUS_SPOOLING)
								{
									SetJob(m_ThreadInfo.GetPrinter(), pNotification->aData[x].Id, 0, NULL, 1);
								}
							}
							//	job received notification
						}
						/*
						if(pNotification->aData[x].NotifyData.Data.cbBuf == JOB_STATUS_PRINTED)
						{
							
						}*/
					}
				}
				
				//notification cleanup

				DWORD dwOldFlags = NotificationOptions.Flags;
				    
				// we must refresh to continue
				NotificationOptions.Flags = PRINTER_NOTIFY_OPTIONS_REFRESH;
				
				FreePrinterNotifyInfo(pNotification);
				pNotification = NULL;
				
				FindNextPrinterChangeNotification(hChange, &dwChange, &NotificationOptions, (LPVOID *) &pNotification);
				NotificationOptions.Flags = dwOldFlags;
				
				FreePrinterNotifyInfo(pNotification);
				pNotification = NULL;
				
			}
			
        }
        else if (endThread)
        {
			FindClosePrinterChangeNotification(hChange);
			//cout<<"Listener Thread closed"<<endl;
		    hChange = INVALID_HANDLE_VALUE;
        }

		
    }
	
    // signal the event to let the primary thread know that this thread is done
    SetEvent(m_ThreadInfo.GetThreadDoneEvent());
	endThread = false;

    return 0;
}


