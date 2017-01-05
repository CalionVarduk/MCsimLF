#pragma once

#include "SecondaryForm.h"

namespace MCsimLF {
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	using namespace MCsimLF::Helpers;
	using namespace MCsimLF::Helpers::UI;

	public ref class MCViewSettingsForm : public SecondaryForm
	{
	public:
		MCViewSettingsForm(LabelButton^ _controlButton) : SecondaryForm(_controlButton)
		{
			InitializeComponent();
			_initializeConfirmButton();
			_initializeBoxes();

			Event_SphereNearPanelUpdate(nullptr, nullptr);
			Event_SphereFarPanelUpdate(nullptr, nullptr);
			Event_BackgroundPanelUpdate(nullptr, nullptr);
		}

		void addConfirmEvent(LabelButtonMouseClickHandler^ handler)
		{ buttonConfirm->MouseClickExtension += handler; }

		Color parseSphereNearColor()
		{
			Int32 r = TextBoxParser::tryInt32(boxes[0], 0, 255);
			Int32 g = TextBoxParser::tryInt32(boxes[1], 0, 255);
			Int32 b = TextBoxParser::tryInt32(boxes[2], 0, 255);
			return Color::FromArgb(r, g, b);
		}

		Color parseSphereFarColor()
		{
			Int32 r = TextBoxParser::tryInt32(boxes[3], 0, 255);
			Int32 g = TextBoxParser::tryInt32(boxes[4], 0, 255);
			Int32 b = TextBoxParser::tryInt32(boxes[5], 0, 255);
			return Color::FromArgb(r, g, b);
		}

		Color parseBackgroundColor()
		{
			Int32 r = TextBoxParser::tryInt32(boxes[6], 0, 255);
			Int32 g = TextBoxParser::tryInt32(boxes[7], 0, 255);
			Int32 b = TextBoxParser::tryInt32(boxes[8], 0, 255);
			return Color::FromArgb(r, g, b);
		}

	private:
#pragma region Fields Declaration
		LabelButton^ buttonConfirm;

		TitledTextBoxList^ boxes;
		
		System::Windows::Forms::Panel^  panelSphereNear;
		System::Windows::Forms::Panel^  panelSphereFar;
		System::Windows::Forms::Panel^  panelBackground;
#pragma endregion

		void Event_SphereNearPanelUpdate(Object^ sender, EventArgs^ e)
		{ panelSphereNear->BackColor = parseSphereNearColor(); }

		void Event_SphereFarPanelUpdate(Object^ sender, EventArgs^ e)
		{ panelSphereFar->BackColor = parseSphereFarColor(); }

		void Event_BackgroundPanelUpdate(Object^ sender, EventArgs^ e)
		{ panelBackground->BackColor = parseBackgroundColor(); }

#pragma region Custom Initialization
		void _initializeBoxes()
		{
			boxes = gcnew TitledTextBoxList(this, -1, Point(20, -1));

			boxes->addControl("sphereNearR", "Sphere Near Color [R]:");
			boxes->addControl("sphereNearG", "Sphere Near Color [G]:");
			boxes->addControl("sphereNearB", "Sphere Near Color [B]:");
			boxes->addControl("sphereFarR", "Sphere Far Color [R]:");
			boxes->addControl("sphereFarG", "Sphere Far Color [G]:");
			boxes->addControl("sphereFarB", "Sphere Far Color [B]:");
			boxes->addControl("backgroundR", "Background Color [R]:");
			boxes->addControl("backgroundG", "Background Color [G]:");
			boxes->addControl("backgroundB", "Background Color [B]:");
			
			boxes[0]->Text = "255";
			boxes[1]->Text = "70";
			boxes[2]->Text = "0";
			boxes[3]->Text = "0";
			boxes[4]->Text = "185";
			boxes[5]->Text = "255";
			boxes[6]->Text = "0";
			boxes[7]->Text = "15";
			boxes[8]->Text = "50";

			for(Int32 i = 0; i < boxes->Count; ++i) {
				boxes[i]->ControlSize = System::Drawing::Size(100, 21);
				boxes[i]->MaxLength = 3;
			}

			for(Int32 i = 0; i < boxes->Count; i += 3) {
				boxes[i]->TitleForeColor = Color::Maroon;
				boxes[i]->ControlForeColor = Color::Maroon;
				boxes[i + 1]->TitleForeColor = Color::DarkGreen;
				boxes[i + 1]->ControlForeColor = Color::DarkGreen;
				boxes[i + 2]->TitleForeColor = Color::MidnightBlue;
				boxes[i + 2]->ControlForeColor = Color::MidnightBlue;
			}

			for(Int32 i = 0; i <= 2; ++i)
				boxes[i]->addTextChangedEvent(gcnew EventHandler(this, &MCViewSettingsForm::Event_SphereNearPanelUpdate));
			for(Int32 i = 3; i <= 5; ++i)
				boxes[i]->addTextChangedEvent(gcnew EventHandler(this, &MCViewSettingsForm::Event_SphereFarPanelUpdate));
			for(Int32 i = 6; i < boxes->Count; ++i)
				boxes[i]->addTextChangedEvent(gcnew EventHandler(this, &MCViewSettingsForm::Event_BackgroundPanelUpdate));

			boxes->Location = Point(12, 13);
			boxes->align();
			_initializePositions();
		}

		void _initializePositions()
		{
			Int32 heightOffset = 10;
			for(Int32 i = 3; i <= 5; ++i)
				boxes[i]->Location = Point(boxes[i]->Left, boxes[i]->Top + heightOffset);

			heightOffset += 10;
			for(Int32 i = 6; i < boxes->Count; ++i)
				boxes[i]->Location = Point(boxes[i]->Left, boxes[i]->Top + heightOffset);

			boxes->fixSize();

			panelSphereNear->Size = System::Drawing::Size(25, boxes[2]->Bottom - boxes[0]->Top);
			panelSphereFar->Size = System::Drawing::Size(25, boxes[5]->Bottom - boxes[3]->Top);
			panelBackground->Size = System::Drawing::Size(25, boxes[8]->Bottom - boxes[6]->Top);

			panelSphereNear->Location = Point(boxes->Right - 1, boxes[0]->Top);
			panelSphereFar->Location = Point(boxes->Right - 1, boxes[3]->Top);
			panelBackground->Location = Point(boxes->Right - 1, boxes[6]->Top);

			ClientSize = System::Drawing::Size(panelSphereNear->Right + 12, boxes->Bottom + buttonConfirm->Height + 22);

			buttonConfirm->Location = Point((ClientSize.Width - buttonConfirm->Width) >> 1,
											ClientSize.Height - buttonConfirm->Height - 12);
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
			this->panelSphereNear = (gcnew System::Windows::Forms::Panel());
			this->panelSphereFar = (gcnew System::Windows::Forms::Panel());
			this->panelBackground = (gcnew System::Windows::Forms::Panel());
			this->SuspendLayout();
			// 
			// panelSphereNear
			// 
			this->panelSphereNear->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->panelSphereNear->Name = L"panelSphereNear";
			// 
			// panelSphereFar
			// 
			this->panelSphereFar->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->panelSphereFar->Name = L"panelSphereFar";
			// 
			// panelBackground
			// 
			this->panelBackground->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->panelBackground->Name = L"panelBackground";
			// 
			// MCViewSettingsForm
			// 
			this->BackColor = System::Drawing::SystemColors::ActiveCaption;
			this->Controls->Add(this->panelBackground);
			this->Controls->Add(this->panelSphereFar);
			this->Controls->Add(this->panelSphereNear);
			this->Name = L"MCViewSettingsForm";
			this->Text = L"View Settings";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	};
}
