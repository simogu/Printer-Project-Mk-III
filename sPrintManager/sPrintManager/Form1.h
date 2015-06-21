#pragma once
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include "confirmDeletePopup.h"
#include "PrinterManager.h"
#include "FormController.h"


using namespace std; 


namespace sPrintManager {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	
	
	/// <summary>
	/// Summary for Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{

		
	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;	
		FormController* controller;

	
	public:
		
		Form1(void)
		{
			 //printerSelected = false;
			 
			 //setup UI
			 InitializeComponent();

			 //setup double buffer to remove flickering
			 HWND hWnd = static_cast<HWND>(this->listView1->Handle.ToPointer());
			 SendMessage(hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_DOUBLEBUFFER, LVS_EX_DOUBLEBUFFER);
			 
			//setup keyboard controls
			this->KeyPreview = true;
			this->KeyDown +=
				gcnew KeyEventHandler(this, &Form1::Form1_KeyDown);
			
			//setup timed loop
			Timer^ MyTimer = gcnew Timer();
			MyTimer->Interval = 500; // 45 mins
			MyTimer->Tick += gcnew EventHandler(this,&Form1::MyTimer_Tick);
			MyTimer->Start();

			//setup link with controller and printer
			InitializeLink();
		}

		
	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			delete controller;

			if (components)
			{
				delete components;
			}
		}
	
	private: System::Windows::Forms::ListView^  listView1;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::ColumnHeader^  columnHeader1;
	private: System::Windows::Forms::ColumnHeader^  columnHeader2;
	private: System::Windows::Forms::ColumnHeader^  columnHeader3;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::Button^  button3;
	private: System::Windows::Forms::Button^  button4;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::ColumnHeader^  columnHeader9;
	private: System::Windows::Forms::ColumnHeader^  columnHeader4;
	private: System::Windows::Forms::ColumnHeader^  columnHeader5;
	private: System::Windows::Forms::ColumnHeader^  columnHeader6;
	private: System::Windows::Forms::ColumnHeader^  columnHeader7;
	private: System::Windows::Forms::ColumnHeader^  columnHeader8;
	private: System::Windows::Forms::ComboBox^  comboBox1;
	private: System::Windows::Forms::Panel^  panel1;
	private: System::Windows::Forms::Label^  label10;
	private: System::Windows::Forms::Button^  button5;
	private: System::Windows::Forms::Label^  label9;
	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Button^  button6;


	protected: 

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->listView1 = (gcnew System::Windows::Forms::ListView());
			this->columnHeader1 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader2 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader3 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader4 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader5 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader6 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader7 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader8 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader9 = (gcnew System::Windows::Forms::ColumnHeader());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->button4 = (gcnew System::Windows::Forms::Button());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->comboBox1 = (gcnew System::Windows::Forms::ComboBox());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->button5 = (gcnew System::Windows::Forms::Button());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->button6 = (gcnew System::Windows::Forms::Button());
			this->panel1->SuspendLayout();
			this->SuspendLayout();
			// 
			// listView1
			// 
			this->listView1->Activation = System::Windows::Forms::ItemActivation::OneClick;
			this->listView1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom));
			this->listView1->BackColor = System::Drawing::SystemColors::ButtonHighlight;
			this->listView1->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(9) {this->columnHeader1, this->columnHeader2, 
				this->columnHeader3, this->columnHeader4, this->columnHeader5, this->columnHeader6, this->columnHeader7, this->columnHeader8, 
				this->columnHeader9});
			this->listView1->FullRowSelect = true;
			this->listView1->GridLines = true;
			this->listView1->Location = System::Drawing::Point(0, 86);
			this->listView1->Name = L"listView1";
			this->listView1->Size = System::Drawing::Size(971, 347);
			this->listView1->TabIndex = 0;
			this->listView1->UseCompatibleStateImageBehavior = false;
			this->listView1->View = System::Windows::Forms::View::Details;
			this->listView1->ColumnClick += gcnew System::Windows::Forms::ColumnClickEventHandler(this, &Form1::listView1_ColumnClick);
			this->listView1->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::listView1_SelectedIndexChanged);
			// 
			// columnHeader1
			// 
			this->columnHeader1->Text = L"ID";
			this->columnHeader1->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->columnHeader1->Width = 39;
			// 
			// columnHeader2
			// 
			this->columnHeader2->Text = L"Description";
			this->columnHeader2->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->columnHeader2->Width = 278;
			// 
			// columnHeader3
			// 
			this->columnHeader3->Text = L"User";
			this->columnHeader3->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->columnHeader3->Width = 117;
			// 
			// columnHeader4
			// 
			this->columnHeader4->Text = L"Total Pgs";
			this->columnHeader4->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// columnHeader5
			// 
			this->columnHeader5->Text = L"Copies";
			this->columnHeader5->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->columnHeader5->Width = 57;
			// 
			// columnHeader6
			// 
			this->columnHeader6->Text = L"Printed";
			this->columnHeader6->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->columnHeader6->Width = 54;
			// 
			// columnHeader7
			// 
			this->columnHeader7->Text = L"Status";
			this->columnHeader7->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->columnHeader7->Width = 78;
			// 
			// columnHeader8
			// 
			this->columnHeader8->Text = L"File Size";
			this->columnHeader8->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->columnHeader8->Width = 88;
			// 
			// columnHeader9
			// 
			this->columnHeader9->Text = L"Submitted At";
			this->columnHeader9->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->columnHeader9->Width = 176;
			// 
			// label1
			// 
			this->label1->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(12, 8);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(78, 13);
			this->label1->TabIndex = 1;
			this->label1->Text = L"Connected To:";
			// 
			// button1
			// 
			this->button1->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->button1->Location = System::Drawing::Point(645, 34);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 2;
			this->button1->Text = L"Resume(F4)";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
			// 
			// button2
			// 
			this->button2->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->button2->Location = System::Drawing::Point(552, 34);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(75, 23);
			this->button2->TabIndex = 3;
			this->button2->Text = L"Pause(F3)";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &Form1::button2_Click);
			// 
			// button3
			// 
			this->button3->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->button3->Location = System::Drawing::Point(444, 33);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(75, 23);
			this->button3->TabIndex = 4;
			this->button3->Text = L"Resume(F2)";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &Form1::button3_Click);
			// 
			// button4
			// 
			this->button4->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->button4->Location = System::Drawing::Point(343, 33);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(75, 23);
			this->button4->TabIndex = 5;
			this->button4->Text = L"Pause(F1)";
			this->button4->UseVisualStyleBackColor = true;
			this->button4->Click += gcnew System::EventHandler(this, &Form1::button4_Click);
			// 
			// label2 //total pages
			// 
			this->label2->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->label2->BackColor = System::Drawing::SystemColors::ControlLightLight;
			this->label2->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->label2->Location = System::Drawing::Point(850, 30);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(100, 30);
			this->label2->TabIndex = 6;
			this->label2->Text = L"0";
			this->label2->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// comboBox1
			// 
			this->comboBox1->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->comboBox1->BackColor = System::Drawing::SystemColors::Info;
			this->comboBox1->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->comboBox1->FormattingEnabled = true;
			this->comboBox1->Location = System::Drawing::Point(191, 35);
			this->comboBox1->Name = L"comboBox1";
			this->comboBox1->Size = System::Drawing::Size(121, 21);
			this->comboBox1->TabIndex = 7;
			this->comboBox1->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::comboBox1_SelectedIndexChanged);
			// 
			// panel1
			// 
			this->panel1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->panel1->Controls->Add(this->label10);
			this->panel1->Controls->Add(this->button5);
			this->panel1->Controls->Add(this->label9);
			this->panel1->Controls->Add(this->label8);
			this->panel1->Controls->Add(this->label7);
			this->panel1->Controls->Add(this->label6);
			this->panel1->Controls->Add(this->label5);
			this->panel1->Controls->Add(this->label4);
			this->panel1->Controls->Add(this->label3);
			this->panel1->Controls->Add(this->comboBox1);
			this->panel1->Controls->Add(this->label2);
			this->panel1->Controls->Add(this->label1);
			this->panel1->Controls->Add(this->button1);
			this->panel1->Controls->Add(this->button2);
			this->panel1->Controls->Add(this->button3);
			this->panel1->Controls->Add(this->button4);
			this->panel1->Location = System::Drawing::Point(0, -1);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(971, 68);
			this->panel1->TabIndex = 8;
			// 
			// label10
			// 
			this->label10->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->label10->AutoSize = true;
			this->label10->Location = System::Drawing::Point(868, 8);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(64, 13);
			this->label10->TabIndex = 16;
			this->label10->Text = L"Total Pages";
			// 
			// button5
			// 
			this->button5->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->button5->Location = System::Drawing::Point(737, 34);
			this->button5->Name = L"button5";
			this->button5->Size = System::Drawing::Size(75, 23);
			this->button5->TabIndex = 15;
			this->button5->Text = L"Restart(F5)";
			this->button5->UseVisualStyleBackColor = true;
			this->button5->Click += gcnew System::EventHandler(this, &Form1::button5_Click);
			// 
			// label9
			// 
			this->label9->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(642, 8);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(84, 13);
			this->label9->TabIndex = 14;
			this->label9->Text = L"Print Job Control";
			// 
			// label8
			// 
			this->label8->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(460, 7);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(0, 13);
			this->label8->TabIndex = 13;
			// 
			// label7
			// 
			this->label7->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(375, 7);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(79, 13);
			this->label7->TabIndex = 12;
			this->label7->Text = L"Printer (Status):";
			// 
			// label6
			// 
			this->label6->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(213, 7);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(70, 13);
			this->label6->TabIndex = 11;
			this->label6->Text = L"Select Printer";
			// 
			// label5
			// 
			this->label5->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(136, 39);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(0, 13);
			this->label5->TabIndex = 10;
			// 
			// label4
			// 
			this->label4->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(12, 39);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(126, 13);
			this->label4->TabIndex = 9;
			this->label4->Text = L"Number of Local Printers:";
			// 
			// label3
			// 
			this->label3->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(91, 8);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(0, 13);
			this->label3->TabIndex = 8;
			// 
			// button6
			// 
			this->button6->Anchor = System::Windows::Forms::AnchorStyles::Top;
			this->button6->Location = System::Drawing::Point(0, 65);
			this->button6->Name = L"button6";
			this->button6->Size = System::Drawing::Size(75, 23);
			this->button6->TabIndex = 9;
			this->button6->Text = L"Select All";
			this->button6->UseVisualStyleBackColor = true;
			this->button6->Click += gcnew System::EventHandler(this, &Form1::button6_Click);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(971, 433);
			this->Controls->Add(this->button6);
			this->Controls->Add(this->panel1);
			this->Controls->Add(this->listView1);
			this->Name = L"Form1";
			this->Text = L"sPrint Mk-III";
			this->panel1->ResumeLayout(false);
			this->panel1->PerformLayout();
			this->ResumeLayout(false);

		}
		
#pragma endregion

	private: void InitializeLink(){

		//intialize form controller
		controller = new FormController();

		//get list of printer
		vector<string> printerList = controller->getPrinterListEvent();
		
		//populate to dropdown list
		for(int i=0;i<printerList.size();i++)
		{
			this->comboBox1->Items->Add(gcnew System::String(printerList.at(i).c_str()));
		}

		//label number of local printers
		stringstream s;
		s<<printerList.size();
		this->label5->Text = gcnew System::String(s.str().c_str());

		//indicate connection
		this->label3->Text = "This Computer";		

	}

	private: vector<int> getSelectedJobs()
	{
		vector<int> selectedJobs;
		for(int i = 0; i<listView1->SelectedItems->Count; i++)
		{
			string s;
			MarshalString(listView1->Items[listView1->SelectedIndices[i]]->SubItems[0]->Text,s);	
			selectedJobs.push_back(atoi(s.c_str()));
		}
		return selectedJobs;
	}
	
	private: System::Void listView1_ColumnClick(System::Object^  sender, System::Windows::Forms::ColumnClickEventArgs^  e) {
		listView1->Sort();
	}

	private: System::Void Form1_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e){

		if(e->KeyCode == Keys::Delete && listView1->SelectedItems->Count>0)
		{
			ShowDeleteDialogBox();			
		}
				
		if (e->KeyCode == Keys::A && e->Control)
		{
			cout<<"key"<<endl;
			listView1->MultiSelect = true;
			for (int i = 0;i<listView1->Items->Count;i++)
			{
				listView1->Items[i]->Selected = true;
			}
		}

		if (e->KeyCode == Keys::F1)
		{
			controller->setPausePrinterEvent();
		}

		if (e->KeyCode == Keys::F2)
		{
			controller->setUnpausePrinterEvent();
		}

		if (e->KeyCode == Keys::F3)
		{
			controller->setControlJobEvent(getSelectedJobs(),1);
		}

		if (e->KeyCode == Keys::F4)
		{
			controller->setControlJobEvent(getSelectedJobs(),2);
		}

		if (e->KeyCode == Keys::F5)
		{
			controller->setControlJobEvent(getSelectedJobs(),4);
		}
					
	}

			 
	private: void ShowDeleteDialogBox()
	{
		confirmDeletePopup^ testDialog = gcnew confirmDeletePopup;

		// Show testDialog as a modal dialog and determine if DialogResult = OK. 
		if ( testDialog->ShowDialog( this ) == System::Windows::Forms::DialogResult::Yes )
		{	
			//set delete job event to controller
			controller->setControlJobEvent(getSelectedJobs(),5);

			//remove selected entries;
			for(int i =listView1->SelectedItems->Count-1; i>=0 ; i--)
			{
				string s;
				MarshalString(listView1->Items[listView1->SelectedIndices[i]]->SubItems[0]->Text,s);	
				listView1->Items->RemoveAt(listView1->SelectedIndices[i]);
			}
					
		}
				
		delete testDialog;
	}
				
	private: void MarshalString ( String ^ s, string& os ) {

		using namespace Runtime::InteropServices;
		const char* chars = (const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
		os = chars;
		Marshal::FreeHGlobal(IntPtr((void*)chars));
	}

	private: System::Void listView1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
			 
		//add up total pages from selected print jobs
		int sum = 0;
		for(int i = 0; i<listView1->SelectedItems->Count; i++)
		{
			string pages;
			MarshalString(listView1->Items[listView1->SelectedIndices[i]]->SubItems[3]->Text,pages);
			string copies;
			MarshalString(listView1->Items[listView1->SelectedIndices[i]]->SubItems[4]->Text,copies);

			int p = atoi (pages.c_str());
			int c = atoi (copies.c_str());

			sum = sum + p*c;
				
		}
			 
		stringstream ss;
		ss << sum;
		
		//update the total pages label
		String^ totalPages = gcnew String(ss.str().c_str());
		this->label2->Text = totalPages;
	}

	private: System::Void MyTimer_Tick(System::Object^  sender, System::EventArgs^  e)
	{
				
		//set the printer status and update listview color
		string pStatus = controller->getPrinterStateEvent();
		stringstream s;
		s<<pStatus;

		this->label8->Text = gcnew System::String(s.str().c_str());

		if(pStatus.compare("PAUSED")==0) 
		{
			this->listView1->BackColor = System::Drawing::Color::LightGray;
		}
		else if(pStatus.compare("READY")==0)
		{
			this->listView1->BackColor = System::Drawing::Color::White;
		}
			
		vector<int> prevSelected;

		//record previously selected list items
		for(int i = 0;i<this->listView1->SelectedItems->Count;i++)
		{
			prevSelected.push_back(listView1->SelectedIndices[i]);
		}

		//get current printer jobs
		vector<vector<string>> jobs = controller->getPrinterJobEvent();
		array< ListViewItem^ >^ items = gcnew array< ListViewItem^ >(jobs.size());

		for (int i=0;i<jobs.size();i++) {

			cli::array<System::String^>^ newData = gcnew cli::array<System::String^>(8);
			for(int j=0;j<jobs.at(i).size()-1;j++)
			{	
				newData[j] =  gcnew String(jobs.at(i).at(j+1).c_str());							
			}
			items[i] = gcnew ListViewItem(gcnew String(jobs.at(i).at(0).c_str()));
			items[i]->SubItems->AddRange(newData);
		}

		//clear and repopulate listview
		listView1->Items->Clear();
		listView1->Items->AddRange(items);

		//reselect previously selected items
		for(int i = 0;i<prevSelected.size();i++)
		{
			listView1->Items[prevSelected.at(i)]->Selected = true;
		}

		//refresh current selected printer
		string currentPrinter;
		MarshalString(comboBox1->Text,currentPrinter);	
		controller->refreshSelectedPrinterEvent(currentPrinter);

	}
		 //SELECT ALL JOBS BUTTON
private: System::Void button6_Click(System::Object^  sender, System::EventArgs^  e) {
			 
			 listView1->MultiSelect = true;
			 for (int i = 0;i<listView1->Items->Count;i++)
			 {
				listView1->Items[i]->Selected = true;
			 }
			 listView1->Focus();
		 }

		 //SELECT PRINTER DROP-DOWN
private: System::Void comboBox1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {

			 listView1->Focus();
			 string selectedPrinter;

			 MarshalString(comboBox1->Text,selectedPrinter);	
			 controller->setCurrentPrinterEvent(selectedPrinter);

		 }

		 //PAUSE PRINTER BUTTON
private: System::Void button4_Click(System::Object^  sender, System::EventArgs^  e) {
			 listView1->Focus();
			 controller->setPausePrinterEvent();
		 }

		 //RESUME PRINTER BUTTON
private: System::Void button3_Click(System::Object^  sender, System::EventArgs^  e) {
			 listView1->Focus();
			 controller->setUnpausePrinterEvent();		  
		 }

		 //PAUSE JOBS BUTTON
private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {	 
			 listView1->Focus();
			 controller->setControlJobEvent(getSelectedJobs(),1);
		 }

		 //RESUME JOBS BUTTON
private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
			 listView1->Focus();
			 controller->setControlJobEvent(getSelectedJobs(),2);
		 }

		 //RESTART JOBS BUTTON
private: System::Void button5_Click(System::Object^  sender, System::EventArgs^  e) {	 
			 listView1->Focus();
			 controller->setControlJobEvent(getSelectedJobs(),4);
		 }

};
}

