#pragma once

namespace Wave_renew 
{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class ViewForm : public System::Windows::Forms::Form
	{
	private:
		System::Windows::Forms::Button^  button_ShowHeight;
		System::Windows::Forms::Button^  button_ShowDisturbance;
		System::Windows::Forms::Button^  button_SaveLayer;
		System::ComponentModel::Container ^components;

	public:
		ViewForm(void)
		{
			InitializeComponent();
		}
		System::Windows::Forms::PictureBox^  pictureBox_main;

	protected:
		~ViewForm()
		{
			if (components)
			{
				delete components;
			}
		}

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->pictureBox_main = (gcnew System::Windows::Forms::PictureBox());
			this->button_ShowHeight = (gcnew System::Windows::Forms::Button());
			this->button_ShowDisturbance = (gcnew System::Windows::Forms::Button());
			this->button_SaveLayer = (gcnew System::Windows::Forms::Button());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox_main))->BeginInit();
			this->SuspendLayout();
			// 
			// pictureBox_main
			// 
			this->pictureBox_main->Location = System::Drawing::Point(-1, 0);
			this->pictureBox_main->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->pictureBox_main->Name = L"pictureBox_main";
			this->pictureBox_main->Size = System::Drawing::Size(600, 351);
			this->pictureBox_main->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pictureBox_main->TabIndex = 0;
			this->pictureBox_main->TabStop = false;
			// 
			// button_ShowHeight
			// 
			this->button_ShowHeight->Location = System::Drawing::Point(142, 356);
			this->button_ShowHeight->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->button_ShowHeight->Name = L"button_ShowHeight";
			this->button_ShowHeight->Size = System::Drawing::Size(76, 25);
			this->button_ShowHeight->TabIndex = 1;
			this->button_ShowHeight->Text = L"Show Height";
			this->button_ShowHeight->UseVisualStyleBackColor = true;
			this->button_ShowHeight->Click += gcnew System::EventHandler(this, &ViewForm::button_ShowHeight_Click);
			// 
			// button_ShowDisturbance
			// 
			this->button_ShowDisturbance->Location = System::Drawing::Point(248, 356);
			this->button_ShowDisturbance->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->button_ShowDisturbance->Name = L"button_ShowDisturbance";
			this->button_ShowDisturbance->Size = System::Drawing::Size(110, 25);
			this->button_ShowDisturbance->TabIndex = 2;
			this->button_ShowDisturbance->Text = L"Show Disturbance";
			this->button_ShowDisturbance->UseVisualStyleBackColor = true;
			this->button_ShowDisturbance->Click += gcnew System::EventHandler(this, &ViewForm::button_ShowDisturbance_Click);
			// 
			// button_SaveLayer
			// 
			this->button_SaveLayer->Location = System::Drawing::Point(385, 356);
			this->button_SaveLayer->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->button_SaveLayer->Name = L"button_SaveLayer";
			this->button_SaveLayer->Size = System::Drawing::Size(76, 25);
			this->button_SaveLayer->TabIndex = 3;
			this->button_SaveLayer->Text = L"Save Layer";
			this->button_SaveLayer->UseVisualStyleBackColor = true;
			this->button_SaveLayer->Click += gcnew System::EventHandler(this, &ViewForm::button_SaveLayer_Click);
			// 
			// ViewForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(599, 391);
			this->Controls->Add(this->button_SaveLayer);
			this->Controls->Add(this->button_ShowDisturbance);
			this->Controls->Add(this->button_ShowHeight);
			this->Controls->Add(this->pictureBox_main);
			this->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->Name = L"ViewForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"Processing, t = 0h 0m 0s";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox_main))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion

public: 
	System::Void button_ShowHeight_Click(System::Object^  sender, System::EventArgs^  e) 
	{

	}

	System::Void button_ShowDisturbance_Click(System::Object^  sender, System::EventArgs^  e) 
	{

	}

	System::Void button_SaveLayer_Click(System::Object^  sender, System::EventArgs^  e) 
	{
	}
};
}
