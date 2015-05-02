
#include "stdafx.h"
#include "ExcelXML.h"


ExcelXML::ExcelXML() {
}

ExcelXML::~ExcelXML() {
}

string ExcelXML::createHeader() {
		string header = "";
		header.append("<?xml version=\"1.0\"?><?mso-application progid=\"Excel.Sheet\"?>\n");
		header.append("<Workbook xmlns=\"urn:schemas-microsoft-com:office:spreadsheet\"\n");
		header.append("xmlns:o=\"urn:schemas-microsoft-com:office:office\"\n");
		header.append("xmlns:x=\"urn:schemas-microsoft-com:office:excel\"\n");
		header.append("xmlns:ss=\"urn:schemas-microsoft-com:office:spreadsheet\"\n");
		header.append("xmlns:html=\"http://www.w3.org/TR/REC-html40\">\n\n");
		return header;
}

void ExcelXML::setDocumentProperties(string Author, string Version) {
		docProperties = "";
	
		int version = atoi(Version.c_str());
		if (version==0) Version = "15.00";

		docProperties.append("<DocumentProperties xmlns=\"urn:schemas-microsoft-com:office:office\">\n");
		docProperties.append("<Author>" + Author +"</Author>\n");
		docProperties.append("<LastAuthor>" + Author +"</LastAuthor>\n");
		docProperties.append("<Version>" + Version + "</Version>\n");
		docProperties.append("</DocumentProperties>\n\n");

		docProperties.append("<OfficeDocumentSettings xmlns=\"urn:schemas-microsoft-com:office:office\">\n");
		docProperties.append("<AllowPNG/>\n");
		docProperties.append("</OfficeDocumentSettings>\n");
}

bool ExcelXML::addStyle(Style style) {
		
	styleList.push_back(style);
	styleMap.insert( pair<string, unsigned int>(style.getName(), styleList.size()-1) );		//for the ID

	return true;
}

unsigned int ExcelXML::getStyleID(string styleName) {
	std::map<string, unsigned int>::iterator it;
	it = styleMap.find(styleName);
	return it->second;
}

void ExcelXML::addWorksheet(Worksheet worksheet) {
		worksheetList.push_back(worksheet);
}

bool ExcelXML::saveFile(string filename) {
		
		string content = "";		//for overall Content of XML
		string header = createHeader();		//header of Excel XML

		if (docProperties.length()==0) {
				setDocumentProperties("CBLC", "15.00");		//necessary document properties of Excel XML
		}

		//styles needed for tables used in XML
		string styleCode = "";
		styleCode.append("<Styles>\n");
		for (vector<Style>::iterator I = styleList.begin(), E = styleList.end();
						I != E; ++I) {
				Style& style = *I;
				vector<Border> b = style.getBorders();
				Font f = style.getFontType();

				//due to not C++11, only accept 3 conversions: longlong, long double, or _Ulonglong
				styleCode.append("<Style ss:ID=\"s" + std::to_string((long long)(I-styleList.begin())) + "\">\n");		
	
				//Borders
				vector<Border>::iterator BorderI = b.begin(), BorderE = b.end();
				if (BorderI != BorderE) {
						styleCode.append("<Borders>\n");
						do {
							styleCode.append("<Border ss:Position=\"" +BorderI->getPosition()+ "\" ss:LineStyle=\"" +BorderI->getLineStyle()+ "\" ss:Weight=\"" +std::to_string((long long)BorderI->getWeight())+ "\"/>\n");
							++BorderI;
						} while (BorderI != BorderE);
						styleCode.append("</Borders>\n");
				}
				else {
						styleCode.append("<Borders/>\n");
				}
				
				styleCode.append("<Alignment ss:Horizontal=\""+style.getAlignHor()+"\" ss:Vertical=\""+style.getAlignVert()+"\"/>\n");
				styleCode.append("<Interior ss:Color=\""+style.getInteriorColor()+"\" ss:Pattern=\""+style.getPattern()+"\"/>\n");
	
				//Font
				styleCode.append("<Font ss:FontName=\"" +f.getFontName()+ "\" x:Family=\"" +f.getFamily()+ "\" ss:Size=\"" +std::to_string((long long)f.getSize())+ "\" ss:Color=\"" +f.getColor()+ "\"\n");
				styleCode.append("ss:Bold=\"" +std::to_string((long long)f.getBold())+ "\" ss:Italic=\"" +std::to_string((long long)f.getItalic())+ "\" ss:Underline=\"" +f.getUnderline()+ "\"/>\n");

				styleCode.append("</Style>\n");
		}
		styleCode.append("</Styles>\n\n");
		

		
		//concatenate all needed codes so far
		content = header + docProperties + styleCode;

		for (vector<Worksheet>::iterator I = worksheetList.begin(), E = worksheetList.end();
						I != E; ++I) {
				content.append(I->getCode());
		}
		
		content.append("</Workbook>\n");//end of excel workbook -> content is ready for writing to file


		

		return writeToXML(filename, content);
}



bool ExcelXML::writeToXML(string filename, string content) {
		fstream *excelXML = new fstream(filename, fstream::in | fstream::out | fstream::trunc);
		
		if(excelXML->is_open()) {
				excelXML->write(content.c_str(), content.length());
				excelXML->close();
				delete excelXML;
		}
		else {
				return false;
		}
		return true;
}
