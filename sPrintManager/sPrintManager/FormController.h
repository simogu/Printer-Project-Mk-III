#include "PrinterManager.h"
#include "listenerThread.h"
#include <vector>
#include <queue>

//reminder: void/mark out obsolete functions in printMgr class after refactor and completion.


class FormController
{

private:
	
	PrinterManager* pMgr;
	bool printerSelected; //determines that there's a printer selected, begin jobFetch etc
	LPWSTR curPrinter; //current printer name in long pointer form
	JOB_INFO_2 *pJobInfo; //placeholder for _printMgr use
	vector<listenerThread*> threadList;

	//for job code generation
	int jobStatusList[15];
	string statusStringList[15];

	
public:

	FormController();
	~FormController();

	void init();
	void runBlockerThreads();
	vector<string> getPrinterListEvent();
	void setCurrentPrinterEvent(string pName);
	vector<vector<string>> getPrinterJobEvent();
	string getPrinterStateEvent();
	void refreshSelectedPrinterEvent(string currentPrinter);
	void setPausePrinterEvent();
	void setUnpausePrinterEvent();
	void setControlJobEvent(vector<int> jobList, int action);

	string fetchPrinterStatus(int status);
	string fetchJobStatus(int status);

	template <typename T>
	string toString ( T Number );


};