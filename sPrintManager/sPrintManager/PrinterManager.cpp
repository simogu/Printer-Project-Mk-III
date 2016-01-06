#include "stdafx.h"
#include "PrinterManager.h"

PrinterManager::PrinterManager() 
{
		sz = 0;
		Level = 2;
		numOfPrinters = 0;

		PrinterHandles.clear();
		PrinterData.clear();

		refreshList();
}

PrinterManager::~PrinterManager() 
{
}



void PrinterManager::refreshList() 
{
		//free(list);
		/*boolean add =  false;

		if(PrinterData.size()==0)
		{
			add = true;
		}*/
		try{
			EnumPrinters( PRINTER_ENUM_LOCAL|PRINTER_ENUM_CONNECTIONS , NULL, Level, NULL, 0, &sz, &numOfPrinters );

			if ( (list = (PRINTER_INFO_2*) malloc( sz )) == 0 )    return;
   
			if (!EnumPrinters(PRINTER_ENUM_LOCAL|PRINTER_ENUM_CONNECTIONS , NULL, Level, (LPBYTE)list, sz, &sz, &numOfPrinters)) 
			{
						free( list );
						return;
			}
			
			
			//closing any printer handles left opened
			//closePrinters();
		
			//creating HANDLES for each printer and opening them
			//if(add)
			//{
				std::map<LPTSTR, HANDLE>::iterator it = PrinterHandles.begin();
				std::map<LPWSTR, PRINTER_INFO_2>::iterator it2 = PrinterData.begin();
				for (unsigned int i=0; i<numOfPrinters; i++) 
				{
					HANDLE hPrinter;
					// You need a printer handle, open the printer

					PRINTER_DEFAULTS pd;
					ZeroMemory(&pd, sizeof(pd));
					pd.DesiredAccess = PRINTER_ACCESS_USE; //PRINTER_ACCESS_USE, PRINTER_ACCESS_ADMINISTER;
					pd.DesiredAccess = PRINTER_ACCESS_ADMINISTER;
					pd.pDatatype=NULL;
					pd.pDevMode = NULL;

					

					if(list[i].Attributes & PRINTER_ATTRIBUTE_SHARED && list[i].Attributes & PRINTER_ATTRIBUTE_LOCAL)
					{
						if( !OpenPrinter( list[i].pPrinterName, &hPrinter, &pd) )
							continue;
					
						PrinterHandles.insert( it, std::pair<LPTSTR, HANDLE>(list[i].pPrinterName, hPrinter) );
						PrinterData.insert( it2, std::pair<LPWSTR, PRINTER_INFO_2>(list[i].pPrinterName, list[i]) );
					}			
				}
			//}
			
			
		}
		catch(...)
		{
			cout<<"refreshlist exception caught"<<endl;
		}

		
}

void PrinterManager::destroy() 
{
		closePrinters();
		PrinterHandles.clear();
		PrinterData.clear();
		delete list;
}

void PrinterManager::closePrinters() 
{
		for(std::map<LPTSTR, HANDLE>::iterator it=PrinterHandles.begin(); it!=PrinterHandles.end(); ++it)
		{
				ClosePrinter( (*it).second );
		}
		
		PrinterHandles.clear();
		PrinterData.clear();
}

int PrinterManager::getNumOfPrinters() 
{
		return numOfPrinters;		
}

PRINTER_INFO_2* PrinterManager::getPrinterList() 
{
		return list;
}

bool PrinterManager::findPrinter(string printerName)
{
		for(unsigned int i=0;i<numOfPrinters;i++)
		{
			USES_CONVERSION;
			char* pBuffer = T2A(list[i].pPrinterName);
			if(printerName.compare(pBuffer)==0){
				return true;
			}
		}
		
		return false;
}
PRINTER_INFO_2 PrinterManager::getPrinter(int printerNumber) 
{
		return list[printerNumber];
}

PRINTER_INFO_2 PrinterManager::getPrinter(string printerName) 
{
	
		for(unsigned int i=0;i<numOfPrinters;i++)
		{
			USES_CONVERSION;
			char* pBuffer = T2A(list[i].pPrinterName);
			char* pBuffer2 ="";// T2A(list[i].pShareName);
			if(list[i].pShareName!=NULL)
			{
				pBuffer2 = T2A(list[i].pShareName);
			}
			
			if(printerName.compare(pBuffer)==0 || printerName.compare(pBuffer2)==0){
				
				EnumPrinters(PRINTER_ENUM_LOCAL|PRINTER_ENUM_CONNECTIONS , NULL, Level, (LPBYTE)list, sz, &sz, &numOfPrinters);
				return list[i];
			}
		}

	
	
}

HANDLE PrinterManager::getPrinterHandle(LPTSTR szPrinterName)
{
	return PrinterHandles.find(szPrinterName)->second;
}

bool PrinterManager::FetchJobsFromPrinter(LPTSTR szPrinterName, std::vector<PrintJob> &userJob)
{
		std::map<LPTSTR, HANDLE>::iterator it = PrinterHandles.find(szPrinterName);
		if (it == PrinterHandles.end()) {
			return false;
		}
		HANDLE         hPrinter = it->second;
		DWORD          dwNeeded, dwReturned;
		JOB_INFO_2     *pJobInfo;

		if (it->first==NULL) {
				std::cout<<"Error:No rights!\n";
		}

		// First you call EnumJobs() to find out how much memory you need
		if( ! EnumJobs( hPrinter, 0, 0xFFFFFFFF, 2, NULL, 0, &dwNeeded, &dwReturned ) )
		{
				// It should have failed, but if it failed for any reason other
				// than "not enough memory", you should bail out
				if( GetLastError() != ERROR_INSUFFICIENT_BUFFER )
				{
						return FALSE;
				}

		}

		// Allocate enough memory for the JOB_INFO_2 structures plus
		// the extra data - dwNeeded from the previous call tells you
		// the total size needed
		if( (pJobInfo = (JOB_INFO_2 *)malloc( dwNeeded )) == NULL )
		{
				return FALSE;
		}

		// Call EnumJobs() again and let it fill out our structures
		if( ! EnumJobs( hPrinter, 0, 0xFFFFFFFF, 2, (LPBYTE)pJobInfo,
										dwNeeded, &dwNeeded, &dwReturned ) )
		{
				free( pJobInfo );
				return FALSE;
		}

		if (dwReturned > 0) 
		{
				for(unsigned int i=0; i<dwReturned; i++)
				{
				
							PRINTER_INFO_2	printerInfo;
							printerInfo.pPrinterName = szPrinterName;
							PrintJob p;
							p.setJobInfo(pJobInfo[i]);
							p.setPrinterInfo(printerInfo);
							userJob.push_back(p);		
				}

		}
		
		// Clean up
		free( pJobInfo );
		return true;

}


bool PrinterManager::getPrinterJobs(LPTSTR PrinterName, JOB_INFO_2 *pJobInfo, std::vector<PrintJob> &v) {
		
		
		DWORD          dwNeeded=0, dwReturned=0;
		std::map<LPTSTR, HANDLE>::iterator it = PrinterHandles.find(PrinterName);
		std::map<LPWSTR, PRINTER_INFO_2>::iterator it2 = PrinterData.find(PrinterName);
		if (it == PrinterHandles.end() || it2 == PrinterData.end()) {
			return false;
		}
		HANDLE         hPrinter = it->second;
		PRINTER_INFO_2 printerInfo = it2->second;


		// First you call EnumJobs() to find out how much memory you need
		if( !EnumJobs(hPrinter, 0, 0xFFFFFFFF, 2, NULL, 0, &dwNeeded, &dwReturned) )		{
				// It should have failed, but if it failed for any reason other
				// than "not enough memory", you should bail out
				if( GetLastError() != ERROR_INSUFFICIENT_BUFFER )
				{	
					return false;
				}
		}
		
		// Allocate memory size of dwNeeded for JOB_INFO_2 structures
		if( (pJobInfo = (JOB_INFO_2 *)malloc( dwNeeded )) == NULL )
				return false;

		// Call EnumJobs() to fill out our structures; dwReturned tells how many jobs there are
		if( !EnumJobs(hPrinter , 0, 0xFFFFFFFF, 2, (LPBYTE)pJobInfo,
										dwNeeded, &dwNeeded, &dwReturned) )	{
				free( pJobInfo );
				return false;
		}

		if (dwReturned > 0) {
				USES_CONVERSION;
				string pName = W2A(PrinterName);
				
				
				for(unsigned int i=0; i<dwReturned; i++)	{
						
						
						PrintJob p;
						p.setJobInfo(pJobInfo[i]);
						p.setPrinterInfo(printerInfo);
					//	cout<<"id: "<<pJobInfo[i].JobId<<" copies: "<<pJobInfo[i].pDevMode->dmCopies<<endl;
					//	p.setPrintFactor(getPrintFactor(PrinterName, pJobInfo[i]));
						
						v.push_back(p);
						
						
				}
				
				//if(v.size()!=PrinterJobCache[pName].size())
				//{
					PrinterJobCache[pName].clear();
					PrinterJobCache[pName] = v;
					
			//	}
				
		}
		
		
				
		free( pJobInfo );

		return true;
}

void PrinterManager::updateJobCache(string PrinterName, vector<PrintJob> v)
{
	PrinterJobCache[PrinterName].clear();
	PrinterJobCache[PrinterName] = v;
}

bool PrinterManager::fetchJobCache(string PrinterName, std::vector<PrintJob> &v)
{
	
	//cout<<"previous cache size: "<<PrinterName<<" "<<PrinterJobCache[PrinterName].size()<<endl;
	v = PrinterJobCache[PrinterName];

	return true;
}

bool PrinterManager::fetchJobFromCache(std::string PrinterName, int id, PrintJob& job)
{
	for(unsigned int i=0;i<PrinterJobCache[PrinterName].size();i++)
	{
		if(PrinterJobCache[PrinterName].at(i).jobID == id)
		{
			job = PrinterJobCache[PrinterName].at(i);
			return true;
		}
	}
	return false;
}
//default 1 logical page per sheet of paper
double PrinterManager::getPrintFactor(LPTSTR PrinterName, JOB_INFO_2 jobInfo) {
	/*
		double printFactor = 1;
		DWORD          dwNeeded=0;
		std::map<LPTSTR, HANDLE>::iterator it = PrinterHandles.find(PrinterName);
		std::map<LPWSTR, PRINTER_INFO_2>::iterator it2 = PrinterData.find(PrinterName);
		if (it == PrinterHandles.end() || it2 == PrinterData.end()) {
			return false;
		}
		HANDLE         hPrinter = it->second;
		PRINTER_INFO_2 printerInfo = it2->second;

		dwNeeded = DocumentProperties(NULL, hPrinter, jobInfo.pPrinterName, NULL, NULL, 0);


		LPDEVMODE jobDevMode;
		if ( (jobDevMode = (LPDEVMODE)malloc(dwNeeded)) == NULL ) {
				return printFactor;
		}
		
		DocumentProperties(NULL, hPrinter, jobInfo.pPrinterName, jobDevMode, NULL, DM_OUT_BUFFER);
		jobDevMode = jobInfo.pDevMode;
		
		LPDEVMODE pDM = (LPDEVMODE)GlobalLock(jobInfo.pDevMode);
		PBYTE pbDriverData= (PBYTE)( ((BYTE *)pDM)+(jobInfo.pDevMode->dmSize) );
		
		


		lua_State* L;
		L = luaL_newstate();		//initialize Lua
		luaL_openlibs(L);				//load Lua base libraries
		luaL_dofile(L, "printerNUP.lua");		//load the script

		cout<<"fetch printfactor"<<endl;
		USES_CONVERSION;
		string temp = W2A(printerInfo.pShareName);		//convert string to LPWSTR

		lua_getglobal(L, "getPrintFactor");
		lua_pushstring(L, temp.c_str());							//pass as arg 1 to LUA function
		lua_pushnumber(L, jobInfo.TotalPages);				//pass as arg 2 to LUA function
		lua_newtable(L);
		int top = lua_gettop(L);
		for (int i=0; i<jobDevMode->dmSize; i++) {		//pushing the byte array as table elements to LUA
				lua_pushnumber(L, i);											//push index
				lua_pushnumber(L, (int)pbDriverData[i]);	//push value
				lua_rawset(L, -3);												//set as table element
		}
		lua_setglobal(L, "pbDriverData");				//table name referenced in script
		lua_call(L,2,1);	// 2 args, 1 returns
		printFactor = lua_tonumber(L, -1);					lua_pop(L, 1);

		lua_close(L);						//cleanup Lua

		
		cout<<"printfactor ok"<<endl;


		GlobalUnlock( jobInfo.pDevMode );


		return printFactor;

	*/
	return 0;
}





bool PrinterManager::CollateJobs(PRINTER_INFO_2 printer, std::string user, std::vector<PrintJob> &userJob) 
{
		DWORD          dwNeeded, dwReturned;
		JOB_INFO_2     *pJobInfo;

		LPTSTR szPrinterName = printer.pPrinterName;
		std::map<LPTSTR, HANDLE>::iterator it = PrinterHandles.find(szPrinterName);
		if (it == PrinterHandles.end()) {
			return false;
		}
		HANDLE         hPrinter = it->second;


		// First you call EnumJobs() to find out how much memory you need
		if( ! EnumJobs(hPrinter, 0, 0xFFFFFFFF, 1, NULL, 0, &dwNeeded, &dwReturned ) )
		{
				// It should have failed, but if it failed for any reason other
				// than "not enough memory", you should bail out
				if( GetLastError() != ERROR_INSUFFICIENT_BUFFER )
						return false;
		}

		// Allocate enough memory for the JOB_INFO_2 structures plus
		// the extra data - dwNeeded from the previous call tells you
		// the total size needed
		if( (pJobInfo = (JOB_INFO_2 *)malloc( dwNeeded )) == NULL )
				return false;

		// Call EnumJobs() again and let it fill out our structures
		if( ! EnumJobs( hPrinter , 0, 0xFFFFFFFF, 1, (LPBYTE)pJobInfo,
										dwNeeded, &dwNeeded, &dwReturned ) )
		{
				free( pJobInfo );
				return false;
		}


		std::transform(user.begin(), user.end(), user.begin(), tolower);		//convert user admin number to lowercase

		if (dwReturned > 0) 
		{
				USES_CONVERSION;
				LPWSTR temp = A2W(user.c_str());		//convert string to LPWSTR
				for(unsigned int i=0; i<dwReturned; i++)
				{
						if (wcscmp(pJobInfo[i].pUserName, temp)==0)	//compare between LPWSTR strings
						{
								PrintJob p;
								p.setJobInfo(pJobInfo[i]);
								p.setPrinterInfo(printer);
								userJob.push_back(p);
						}
				}
		}
		
		// Clean up
		free( pJobInfo );
		return true;
}



//*tested on local printer
bool PrinterManager::UnpausePrinter( LPTSTR szPrinterName )	
{
		std::map<LPTSTR, HANDLE>::iterator it = PrinterHandles.find(szPrinterName);
		HANDLE         hPrinter = it->second;
		
		bool result = SetPrinter(hPrinter, 0, NULL, PRINTER_CONTROL_RESUME);

		if(result == 0 )	{
				std::cout<<"Error: Failed to unpause printer with error code: "<<GetLastError()<<std::endl;
		}

		return result;
}

//*tested on local printer
bool PrinterManager::PausePrinter( LPTSTR szPrinterName )	
{
		std::map<LPTSTR, HANDLE>::iterator it = PrinterHandles.find(szPrinterName);
		HANDLE         hPrinter = it->second;

		//cout<<"handle"<<it->first<<hPrinter<<endl;
		bool result = SetPrinter(hPrinter, 0, NULL, PRINTER_CONTROL_PAUSE);

		if(result == 0 )	{
				std::cout<<"Error: Failed to pause printer with error code: "<<GetLastError()<<std::endl;
		}

	 return result;
}

bool PrinterManager::startJob(LPTSTR szPrinterName, DWORD JobID, int action)
{
		std::map<LPTSTR, HANDLE>::iterator it = PrinterHandles.find(szPrinterName);
		HANDLE         hPrinter = it->second;
		
		bool result = SetJob(hPrinter, JobID, 0, NULL, action);

		if(result == 0 )
		{
				std::cout<<"Error: Failed to start job with error code: "<<GetLastError()<<std::endl;
		}
		
		return result;
}

bool PrinterManager::deletePrintedJobs(HANDLE printerHandle, string printerName)
{

	for(unsigned int i=0;i<PrinterJobCache[printerName].size();i++)
	{
		if(PrinterJobCache[printerName].at(i).status == 128 || PrinterJobCache[printerName].at(i).status== 129)
		{
			//cout<<"deleting job "<< PrinterJobCache[printerName].at(i).jobID<<endl;
			SetJob(printerHandle, PrinterJobCache[printerName].at(i).jobID,0,NULL,5);
		}
	}

	return true;
}