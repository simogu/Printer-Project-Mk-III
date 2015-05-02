#include "stdafx.h"
#include "Logger.h"

Logger::Logger()
{
}

Logger::~Logger()
{
}

bool fileExists(const std::string& filename)
{
    struct stat buf;
    if (stat(filename.c_str(), &buf) != -1)
    {
        return true;
    }
    return false;
}


void Logger::init(string printer, string file)
{
	printerName = printer;
	date = file;
	ostringstream stream;
	_mkdir("Printing Logs");
	stream << "Printing Logs/" << printer.c_str();
	_mkdir(stream.str().c_str());
	stream.str("");
	stream << "Printing Logs/" << printer.c_str() << "/Buffer(Do Not Open)";
	string directory1 = stream.str();
	stream.str("");

	_mkdir(directory1.c_str());

	stream << "Printing Logs/" << printer.c_str() << "/Release";
	string directory2 = stream.str();
	stream.str("");

	_mkdir(directory2.c_str());

	stream << "Printing Logs/" << printer.c_str() << "/Buffer(Do Not Open)/" << printer << "_" << file << ".xls";
	fileName = stream.str();
	stream.str("");

	stream << "Printing Logs/" << printer.c_str() << "/Release/" << printer << "_" << file << ".xls";
	releaseFileName = stream.str();
	stream.str("");

	//cout<<"File Name :"<<fileName<<endl;

	if(!fileExists(fileName))
	{
		prepareExcel(fileName);
	}
	else
	{
		readRecord(fileName);
	}

}

void Logger::record(string content)
{
	contentBuffer.push_back(content);
}
void Logger::update(string key)
{
	entryMap[key]=1;
}

bool Logger::recordExists(string key)
{
	if(entryMap[key]==1)
	{
		return true;
	}
	else
	{
		return false;
	}
}
void Logger::readRecord(string fileName)
{
	//cout<<"reading record..."<<endl;
	ifstream readFile;

	if(!fileExists(fileName))
	{
		init(printerName,date);
	}
	readFile.open(fileName);

	string entry="";
	int cell = 0;
	bool start = false;

	
	while (!readFile.eof())
	{
		
		string line;
		getline(readFile, line);
		//cout<<line<<endl;
		if(line.compare("<Row ss:AutoFitHeight=\"0\">")==0)
		{
			start = true;
			cell = 0;
		}
		if(line.compare("</Table>")==0)
		{
			cell = 0;
			start = false;
		}
		
		if(cell == 1)
		{
			line = line.substr(29);
			entry+=line.substr(0,line.find("</Data></Cell>"));
			entry+="_";	
		}
		else if(cell == 5)
		{
			line = line.substr(29);
			entry+=line.substr(0,line.find("</Data></Cell>"));
			entryMap[entry]=1;
			entry="";
		}
		if(start)
		{
			cell++;
		}
	}

	readFile.close();


}

bool copyFile(const char *SRC, const char* DEST)
{
    std::ifstream src(SRC, std::ios::binary);
    std::ofstream dest(DEST, std::ios::binary);
    dest << src.rdbuf();
    return src && dest;
}

void Logger::save()
{

	if(contentBuffer.size()<=0)
	{
		copyFile(fileName.c_str(),releaseFileName.c_str());
		return;
	}
	//cout<<"saving buffer: "<<contentBuffer.size()<<endl;
	ifstream readFile;

	if(!fileExists(fileName))
	{
		init(printerName,date);
	}

	readFile.open(fileName);

	vector<string> buffer;
	
	
	
	while (!readFile.eof())
	{
		string line;
		getline(readFile, line);
		buffer.push_back(line);

	}

	readFile.close();

	ofstream writeFile;
	writeFile.open(fileName);

	for(unsigned int i=0;i<buffer.size();i++)
	{
		
		if(buffer.at(i).compare("</Table>")==0)
		{
			for(unsigned int j=0;j<contentBuffer.size();j++)
			{
				writeFile<<contentBuffer.at(j);
			}
		}

		writeFile<<buffer.at(i)<<endl;
		

	}

	writeFile.close();
	buffer.clear();
	contentBuffer.clear();
	
	copyFile(fileName.c_str(),releaseFileName.c_str());

}

void Logger::prepareExcel(string filename) {
		
		ExcelXML excelXML;

		//Font Settings
		Font bodyContent("Calibri", "Swiss");
		Font headerFont("Arial", "Swiss");
		headerFont.setBold(true);
		headerFont.setSize(12);
		headerFont.setUnderline("None");

		//Border Properties
		Border top("Top", "Continuous", 2);
		Border left("Left", "Continuous", 2);
		Border right("Right", "Continuous", 2);
		Border bottom("Bottom", "Continuous", 2);

		//Border Settings
		vector<Border> NONE;	//No borders;
		vector<Border> TLB;		//Top, Left & Bottom
		TLB.push_back(top); 
		TLB.push_back(left); 
		TLB.push_back(bottom);

		vector<Border> TRB;		//Top, Right & Bottom
		TRB.push_back(top); 
		TRB.push_back(right); 
		TRB.push_back(bottom);

		vector<Border> TB;		//Top & Bottom
		TB.push_back(top);
		TB.push_back(bottom);

		vector<Border> LB;		//Left & Bottom
		LB.push_back(left);
		LB.push_back(bottom);

		vector<Border> RB;		//Right & Bottom
		RB.push_back(right);
		RB.push_back(bottom);

		vector<Border> L; L.push_back(left);		//left
		vector<Border> R; R.push_back(right);		//right
		vector<Border> B; B.push_back(bottom);	//bottom
		

		//add styles to excel
		Style headerTLB("headerTLB", headerFont, TLB, "#cccccc", "Center", "Center");	excelXML.addStyle(headerTLB);
		Style headerTRB("headerTRB", headerFont, TRB, "#cccccc", "Center", "Center");	excelXML.addStyle(headerTRB);
		Style headerTB("headerTB", headerFont, TB, "#cccccc", "Center", "Center");		excelXML.addStyle(headerTB);
		Style headerB("headerB", headerFont, B, "#dddddd", "Center", "Center");				excelXML.addStyle(headerB);
		Style bodyL("bodyL", bodyContent, L, "#ffffff", "Center", "Left");					excelXML.addStyle(bodyL);
		Style bodyR("bodyR", bodyContent, R, "#ffffff", "Center", "Left");					excelXML.addStyle(bodyR);
		Style bodyLB("bodyLB", bodyContent, LB, "#ffffff", "Center", "Left");				excelXML.addStyle(bodyLB);
		Style bodyRB("bodyRB", bodyContent, RB, "#ffffff", "Center", "Left");				excelXML.addStyle(bodyRB);
		Style bodyB("bodyB", bodyContent, B, "#ffffff", "Center", "Left");					excelXML.addStyle(bodyB);
		Style body("body", headerFont, NONE, "#ffffff", "Center", "Left");					excelXML.addStyle(body);

		//start document
		//"Job ID, Username/Matric Number, Document, Pages Printed, Date, Time"
		//cout<<"> Creating "<<filename<<endl;
		excelXML.setDocumentProperties("CBLC", "15.00");
		Worksheet wksheet("Printing Log", 15);
		
		Row title;
		vector<double> width;//0,80,120,0,60,0,0

		title.addCell(excelXML.getStyleID("headerB"), "String", "Date Here");	width.push_back(80);
		wksheet.addRow(title);

		Row header;
		header.addCell(excelXML.getStyleID("headerTB"), "String", "Job ID");				width.push_back(120);
		header.addCell(excelXML.getStyleID("headerTB"), "String", "Username");			width.push_back(120);
		header.addCell(excelXML.getStyleID("headerTB"), "String", "Pages Printed");	width.push_back(60);
		header.addCell(excelXML.getStyleID("headerTB"), "String", "Copies");	width.push_back(60);
		header.addCell(excelXML.getStyleID("headerTB"), "String", "Time");					width.push_back(140);
		header.addCell(excelXML.getStyleID("headerTB"), "String", "Document");			width.push_back(200);
		wksheet.addRow(header);
		wksheet.setColumnWidth(width);


		excelXML.addWorksheet(wksheet);
		//cout<<"> Excel XML updated."<<endl;
		excelXML.saveFile(filename);
}