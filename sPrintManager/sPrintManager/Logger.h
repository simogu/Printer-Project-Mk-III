#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include "ExcelXML.h"

using namespace std;
class Logger{


private:

	map<string,int> entryMap;
	string printerName;
	string date;
	string fileName;
	string releaseFileName;
	vector<string> contentBuffer;
	void prepareExcel(string filename);


public:

	Logger();
	~Logger();

	void init(string printer, string file);
	void record(string content);
	void save();
	void readRecord(string filename);
	bool recordExists(string key);
	void update(string key);
};