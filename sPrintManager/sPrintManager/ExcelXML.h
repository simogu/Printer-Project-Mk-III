#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
using namespace std;

struct Font
{
private:
		string				FontName;				//Calibri, Arial, Verdana
		string				Family;					//Swiss
		unsigned int	Size;
		string				Color;
		bool					Bold;
		bool					Italic;
		string					Underline;		//None, Single, Double

public:
		Font() {
				FontName = "Calibri";
				Family = "Swiss";
				Size = 11;
				Color = "#000000";
				Bold = false;
				Italic = false;
				Underline = "None";
		};
		Font(string fontName, string family) {
				FontName = fontName;
				Family = family;
				Size = 11;
				Color = "#000000";
				Bold = false;
				Italic = false;
				Underline = "None";
		};

		string				getFontName() {return FontName;};
		string				getFamily()		{return Family;};
		unsigned int	getSize()			{return Size;};
		string				getColor()		{return Color;};
		bool					getBold()			{return Bold;};
		bool					getItalic()		{return Italic;};
		string				getUnderline(){return Underline;};


		void setSize(unsigned int size) { Size = size;};
		void setColor(string hexColor)	{	Color = hexColor;	};		// eg. #ffffff
		void setBold(bool bold)					{Bold = bold;};
		void setItalic(bool italic)			{Italic = italic;};
		void setUnderline(string underline) {Underline = underline;};
};

struct Border 
{
private:
		string				Position;		// Top, Bottom, Left, Right
		string				LineStyle;	// Continuous, Dot, Dash, 
		unsigned int	Weight;

public:
		Border() {
				Position = "Bottom";
				LineStyle = "Continuous";
				Weight = 0;
		}
		Border(string position, string linestyle, unsigned int weight) {
				Position = position;
				LineStyle = linestyle;
				Weight = weight;
		};

		string getPosition() {return Position;};
		string getLineStyle() {return LineStyle;};
		unsigned int getWeight() {return Weight;};
};

struct Style 
{
private:
		Font FontType;
		vector<Border> BorderStyles;
		string InteriorColor;
		string ID;
		string Name;
		string Pattern;	
		string AlignVert;		//Center, Top, Bottom
		string AlignHor;		//Center, Left, Right
public:
		Style() {
				Name = "Default";
				Font defaultFont;	FontType = defaultFont;
				vector<Border> defaultNoBorder;	BorderStyles = defaultNoBorder;
				InteriorColor = "#ffffff";
				Pattern = "Solid";
				AlignVert = "Bottom";
				AlignHor= "Left";
		}
		Style(string styleName, Font font, vector<Border> borders, string hexInteriorColor, string alignVert, string alignHor) {
				Name = styleName;
				FontType = font;
				BorderStyles = borders;
				InteriorColor = hexInteriorColor;
				Pattern = "Solid";
				AlignVert = alignVert;
				AlignHor= alignHor;
		}

		string getName()		{return Name;};
		Font getFontType()	{return FontType;};
		string getInteriorColor()		{return InteriorColor;};
		vector<Border> getBorders()	{return BorderStyles;};
		string getPattern()	{return Pattern;};
		string getAlignVert()		{return AlignVert;};
		string getAlignHor()		{return AlignHor;};
};

struct Row {
private:
		struct Cell {
		private:
			string StyleID;
			string DataType;
			string Data;
			string Code;
		public:
				Cell(unsigned int styleID, string dataType, string data) {
						StyleID = "s"+std::to_string((long long)styleID);
						DataType = dataType;
						Data = data;
						Code = "";
						Code.append("<Cell ss:StyleID=\""+StyleID+"\"><Data ss:Type=\""+DataType+"\">"+Data+"</Data></Cell>\n");
				};
				string getCode() {return Code;};
		};

		string DataType;	//String, Number
		vector<Cell> cellList;
		string Code;
		unsigned int Height;

public:
		Row(){Height=15;};
		Row(unsigned int height){Height = height;};

		void addCell(unsigned int styleID, string dataType, string data) {
				Cell cell(styleID, dataType, data);
				cellList.push_back(cell);
		};

		string getCode() {
				Code = "";   
				Code.append("<Row ss:AutoFitHeight=\"0\">\n");
				for (unsigned int i=0; i<cellList.size(); i++) {
						Code.append(cellList.at(i).getCode());
				}
				Code.append("</Row>\n");
				return Code;
		};

		vector<Cell> getCellList() { return cellList;};
};


struct Worksheet {
private:
		string Name;
		unsigned int DefaultRowHeight;
		vector<Row> rowList;
		vector<double> columnWidthList;
		string Code;
public:
		Worksheet() {
				Name = "worksheet";
				DefaultRowHeight = 15;
		}
		Worksheet(string name, unsigned int rowHeight) {
				Name = name;
				DefaultRowHeight = rowHeight;
		}
		void addRow(Row row) {
				rowList.push_back(row);
		};
		string getCode() {
				Code = "";
				Code.append("<Worksheet ss:Name=\""+Name+"\">\n");
				Code.append("<Table ss:DefaultRowHeight=\"" +std::to_string((long long)DefaultRowHeight)+ "\">\n");

				if (columnWidthList.size()>0) {
						for (unsigned int i=0; i<columnWidthList.size(); i++) {
								Code.append("<Column ss:Width=\""+std::to_string((long long)columnWidthList.at(i))+"\"/>\n");
						}
				}
				
				for(unsigned int i=0; i<rowList.size(); i++) {
					Code.append(rowList.at(i).getCode());
				}

				Code.append("</Table>\n");
				Code.append("</Worksheet>\n");
				return Code;
		}
		void setColumnWidth(vector<double> widthList) {
				columnWidthList = widthList;
		}
};



class ExcelXML 
{
		private:
				string docProperties;
				vector<Worksheet> worksheetList;

				vector<Style> styleList;
				map<string, unsigned int> styleMap;
				map<string, string> styleCode;


				string createHeader();
				bool writeToXML(string filename, string content);

		public:
				ExcelXML();
				~ExcelXML();

				void setDocumentProperties(string Author, string Version);		//For Excel 2013, Version is 15.00

				bool addStyle(Style style);
				unsigned int getStyleID(string styleName);

				void addWorksheet(Worksheet worksheet);



				bool saveFile(string filename);
};