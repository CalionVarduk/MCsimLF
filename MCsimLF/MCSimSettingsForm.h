#pragma once

#include "SecondaryForm.h"
#include "MonteCarlo.h"

namespace MCsimLF {
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	using namespace MCsimLF::Helpers;
	using namespace MCsimLF::Helpers::UI;
	using namespace MCsimLF::MC;

	public ref class MCSimSettingsForm : public SecondaryForm
	{
	public:
		MCSimSettingsForm(LabelButton^ _controlButton) : SecondaryForm(_controlButton)
		{
			InitializeComponent();
			_initializeConfirmButton();
			_initializeBoxes();

			_linePen = gcnew Pen(Color::Black);
		}

		void addConfirmEvent(LabelButtonMouseClickHandler^ handler)
		{ buttonConfirm->MouseClickExtension += handler; }

		UInt32 parseStepsPerFrame()
		{ return TextBoxParser::tryUInt32(boxes[0], 1, 500); }

		UInt32 parseAtomCount()
		{ return TextBoxParser::tryUInt32(boxes[5], 1, 99999); }

		array<Int32>^ parseFccAtomCount()
		{
			array<Int32>^ fcc = gcnew array<Int32>(3);
			fcc[0] = TextBoxParser::tryInt32(boxes[2], 0, 9999);
			fcc[1] = TextBoxParser::tryInt32(boxes[3], 0, 9999);
			fcc[2] = TextBoxParser::tryInt32(boxes[4], 0, 9999);
			return fcc;
		}

		double parseTemperature()
		{ return TextBoxParser::tryDouble(boxes[6], 0, 9999); }

		Vector3D parseBoxSize()
		{
			double x = TextBoxParser::tryDouble(boxes[7], 0.01, 9999);
			double y = TextBoxParser::tryDouble(boxes[8], 0.01, 9999);
			double z = TextBoxParser::tryDouble(boxes[9], 0.01, 9999);
			return Vector3D(x, y, z);
		}

		Point3D parseOrigin()
		{
			double x = TextBoxParser::tryDouble(boxes[10], -999, 999);
			double y = TextBoxParser::tryDouble(boxes[11], -999, 999);
			double z = TextBoxParser::tryDouble(boxes[12], -999, 999);
			return Point3D(x, y, z);
		}

		UInt32 parseEnergySyncInterval()
		{ return TextBoxParser::tryUInt32(boxes[1], 1, 99999); }

		bool isFccOn()
		{ return checkBoxes[0]->isActivated; }

		bool isPbcOn()
		{ return checkBoxes[1]->isActivated; }

	protected:
		virtual void _customInvalidation() override
		{
			checkBoxes->invalidateAll();
		}

	private:
#pragma region Fields Declaration
		LabelButton^ buttonConfirm;
		System::Windows::Forms::Label^  labelInfo;

		String^ auxAtomCount;
		TitledTextBoxList^ boxes;

		Int32 fccPosDelta;
		TitledCheckBoxList^ checkBoxes;

		Pen^ _linePen;
#pragma endregion

		void fccCheckBoxClicked(Object^ sender, MouseEventArgs^ e)
		{
			boxes[5]->ReadOnly = checkBoxes[0]->isActivated;
			for(Int32 i = 2; i <= 4; ++i)
				boxes[i]->Visible = checkBoxes[0]->isActivated;

			if(checkBoxes[0]->isActivated)
				_showFccBoxes();
			else
				_hideFccBoxes();
		}

		void _showFccBoxes()
		{
			auxAtomCount = boxes[5]->Text;
			boxes[5]->Text = MonteCarlo::getFccCount(parseFccAtomCount()).ToString();
			boxes[5]->ControlBackColor = Color::LightGray;

			_offsetBoxes(fccPosDelta);
		}

		void _hideFccBoxes()
		{
			boxes[5]->Text = auxAtomCount;
			boxes[5]->ControlBackColor = boxes[6]->ControlBackColor;

			_offsetBoxes(-fccPosDelta);
		}

		void _offsetBoxes(Int32 offset)
		{
			for(Int32 i = 6; i < boxes->Count; ++i)
				boxes[i]->Location = Point(boxes[i]->Left, boxes[i]->Location.Y + offset);

			ClientSize = System::Drawing::Size(ClientSize.Width, ClientSize.Height + offset);
		}

		void Event_textChanged(Object^ sender, EventArgs^ e)
		{ boxes[5]->Text = MonteCarlo::getFccCount(parseFccAtomCount()).ToString(); }

		void Event_onPaint(Object^ sender, PaintEventArgs^ e)
		{ e->Graphics->DrawLine(_linePen, 12, labelInfo->Top - 5, ClientSize.Width - 12, labelInfo->Top - 5); }

#pragma region Custom Initialization
		void _initializeBoxes()
		{
			auxAtomCount = "";

			boxes = gcnew TitledTextBoxList(this, -1, Point(20, -1));

			boxes->addControl("stepsPerFrame", "Steps per Frame:");
			boxes->addControl("energySyncInterval", "Energy Sync Interval:");
			boxes->addControl("fccParticlesX", "FCC Atoms [X]:");
			boxes->addControl("fccParticlesY", "FCC Atoms [Y]:");
			boxes->addControl("fccParticlesZ", "FCC Atoms [Z]:");
			boxes->addControl("particles", "Atoms:");
			boxes->addControl("temperature", "Temperature:");
			boxes->addControl("boxSizeX", "Box Size [X]:");
			boxes->addControl("boxSizeY", "Box Size [Y]:");
			boxes->addControl("boxSizeZ", "Box Size [Z]:");
			boxes->addControl("originX", "Origin [X]:");
			boxes->addControl("originY", "Origin [Y]:");
			boxes->addControl("originZ", "Origin [Z]:");

			boxes[0]->Text = "10";
			boxes[1]->Text = "100";
			boxes[2]->Text = "4";
			boxes[3]->Text = "4";
			boxes[4]->Text = "4";
			boxes[5]->Text = "150";
			boxes[6]->Text = "2,7315";
			boxes[7]->Text = SimSettings::boxLengthX.ToString();
			boxes[8]->Text = SimSettings::boxLengthY.ToString();
			boxes[9]->Text = SimSettings::boxLengthZ.ToString();
			boxes[10]->Text = SimSettings::boxOriginX.ToString();
			boxes[11]->Text = SimSettings::boxOriginY.ToString();
			boxes[12]->Text = SimSettings::boxOriginZ.ToString();

			for(Int32 i = 0; i < boxes->Count; ++i) {
				boxes[i]->TitleForeColor = Color::Black;
				boxes[i]->ControlSize = System::Drawing::Size(150, 21);
				boxes[i]->MaxLength = 15;
			}
			for(Int32 i = 2; i <= 4; ++i) {
				boxes[i]->Visible = false;
				boxes[i]->addTextChangedEvent(gcnew EventHandler(this, &MCSimSettingsForm::Event_textChanged));
			}

			boxes->Location = Point(12, 13);
			boxes->align();

			checkBoxes = gcnew TitledCheckBoxList(this);

			checkBoxes->addControl("fccCheckBox", "Initialize as FCC Structure");
			checkBoxes->addControl("pbcCheckBox", "Apply Periodic Boundary Conditions");

			checkBoxes->ControlOffset = Point(10, checkBoxes[0]->VerticalAllignment + 1);
			checkBoxes->align();

			checkBoxes[0]->MouseClick += gcnew CheckBoxMouseClickHandler(this, &MCSimSettingsForm::fccCheckBoxClicked);

			if(SimSettings::fccOn) checkBoxes[0]->forceActivate();
			if(SimSettings::pbcOn) checkBoxes[1]->forceActivate();

			fccPosDelta = 3 * (boxes[0]->Height - 1);
			_initializePositions();
		}

		void _initializePositions()
		{
			Int32 heightOffset = labelInfo->Height + buttonConfirm->Height + boxes[0]->Height + 80;
			for(Int32 i = 2; i <= 4; ++i)
				boxes[i]->Location = Point(boxes[i]->Left, boxes[i]->Top + heightOffset);

			heightOffset -= fccPosDelta + boxes[0]->Height - 1;
			for(Int32 i = 5; i < boxes->Count; ++i)
				boxes[i]->Location = Point(boxes[i]->Left, boxes[i]->Top + heightOffset);

			boxes->fixSize();
			ClientSize = System::Drawing::Size(boxes->Right + 12, boxes->Bottom + 12);

			buttonConfirm->Location = Point((ClientSize.Width - buttonConfirm->Width) >> 1, boxes[1]->Bottom + 10);
			labelInfo->Location = Point((ClientSize.Width - labelInfo->Width) >> 1, buttonConfirm->Bottom + 15);


			checkBoxes->Location = Point((ClientSize.Width - checkBoxes->Width) >> 1,
											boxes[1]->Bottom + (labelInfo->Height + buttonConfirm->Height + 35));
		}

		void _initializeConfirmButton()
		{
			buttonConfirm = gcnew LabelButton(this, "buttonAcceptChanges", "CONFIRM");
			buttonConfirm->setFixedSize(System::Drawing::Size(75, 25));

			buttonConfirm->normalBackColor = Color::SteelBlue;
			buttonConfirm->normalForeColor = Color::WhiteSmoke;
			buttonConfirm->mouseOverBackColor = Color::FromArgb(84, 156, 216);
			buttonConfirm->mouseOverForeColor = Color::WhiteSmoke;
			buttonConfirm->activeBackColor = Color::WhiteSmoke;
			buttonConfirm->activeForeColor = Color::SteelBlue;
			buttonConfirm->activeMouseOverBackColor = Color::WhiteSmoke;
			buttonConfirm->activeMouseOverForeColor = Color::SteelBlue;
			buttonConfirm->runOnMouseClickProcedure = true;
			buttonConfirm->isActivatedOnHold = true;
			buttonConfirm->changeColorsOnActivation = true;
			buttonConfirm->changeColorsOnMouseOver = true;
			buttonConfirm->resetDisplayedColors();
		}
#pragma endregion

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->labelInfo = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// labelInfo
			// 
			this->labelInfo->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.5F, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(238)));
			this->labelInfo->Name = L"labelInfo";
			this->labelInfo->Size = System::Drawing::Size(306, 30);
			this->labelInfo->TabIndex = 0;
			this->labelInfo->Text = L"Values set below will be loaded into the simulation the next time you press the" 
				L" [RESET SIMULATION] button.";
			this->labelInfo->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// MCSimSettingsForm
			// 
			this->BackColor = System::Drawing::SystemColors::ActiveCaption;
			this->Controls->Add(this->labelInfo);
			this->Name = L"MCSimSettingsForm";
			this->Text = L"Simulation Settings";
			this->Paint += gcnew PaintEventHandler(this, &MCSimSettingsForm::Event_onPaint);
			this->ResumeLayout(false);
			this->PerformLayout();
		}
#pragma endregion
};
}
