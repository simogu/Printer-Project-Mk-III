#pragma once

namespace sPrintManager {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for confirmFlushPrinter
	/// </summary>
	public ref class confirmFlushPrinter : public System::Windows::Forms::Form
	{
	public:
		confirmFlushPrinter(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~confirmFlushPrinter()
		{
			if (components)
			{
				delete components;
			}
		}

	private: System::Windows::Forms::Button^  button1;
	protected: 
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::Label^  label1;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->DialogResult = System::Windows::Forms::DialogResult::Yes;
			this->button1->Location = System::Drawing::Point(31, 59);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 0;
			this->button1->Text = L"Yes";
			this->button1->UseVisualStyleBackColor = true;
			// 
			// button2
			// 
			this->button2->DialogResult = System::Windows::Forms::DialogResult::No;
			this->button2->Location = System::Drawing::Point(233, 59);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(75, 23);
			this->button2->TabIndex = 1;
			this->button2->Text = L"No";
			this->button2->TextImageRelation = System::Windows::Forms::TextImageRelation::TextAboveImage;
			this->button2->UseVisualStyleBackColor = true;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(28, 9);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(290, 39);
			this->label1->TabIndex = 2;
			this->label1->Text = L"Print Flushing can help to reset printers to working state\nbut will also delete a" 
				L"ll jobs, and sPrint Mk-III will be restarted.\nProceed\?";
			// 
			// confirmFlushPrinter
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(340, 106);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->button1);
			this->Name = L"confirmFlushPrinter";
			this->Text = L"Confirm Flush Printers ";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	};
}
