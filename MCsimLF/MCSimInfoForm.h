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

	using namespace MCsimLF::Helpers::UI;

	public ref class MCSimInfoForm : public SecondaryForm
	{
	public:
		MCSimInfoForm(LabelButton^ _controlButton) : SecondaryForm(_controlButton)
		{
			InitializeComponent();
			_initializeLabels();
		}

		void setInfo(MonteCarlo^ monteCarlo)
		{
			moreSimInfo[0]->Text = String::Format("{0:0.##}", monteCarlo->getInitialBoxSize().x) + " Ã";
			moreSimInfo[1]->Text = String::Format("{0:0.##}", monteCarlo->getInitialBoxSize().y) + " Ã";
			moreSimInfo[2]->Text = String::Format("{0:0.##}", monteCarlo->getInitialBoxSize().z) + " Ã";
			moreSimInfo[3]->Text = String::Format("{0:0.##}", monteCarlo->getInitialBoxVolume()) + " Ã^3";
			moreSimInfo[4]->Text = String::Format("{0:0.##}", monteCarlo->getOrigin().x);
			moreSimInfo[5]->Text = String::Format("{0:0.##}", monteCarlo->getOrigin().y);
			moreSimInfo[6]->Text = String::Format("{0:0.##}", monteCarlo->getOrigin().z);
			moreSimInfo[7]->Text = monteCarlo->getEnergySyncInterval().ToString();
			moreSimInfo[8]->Text = SimSettings::pbcOn.ToString();

			moreSimInfo->fixSize();
			ClientSize = System::Drawing::Size(moreSimInfo->Right + 12, author->Bottom + 6);
		}

	private:
		TitledLabelList^ moreSimInfo;
		Label^ author;

		void _initializeLabels()
		{
			moreSimInfo = gcnew TitledLabelList(this, 0, Point(10, 0));

			moreSimInfo->addControl("infoBoxSizeX", "Initial Box Size [X]:");
			moreSimInfo->addControl("infoBoxSizeY", "Initial Box Size [Y]:");
			moreSimInfo->addControl("infoBoxSizeZ", "Initial Box Size [Z]:");
			moreSimInfo->addControl("infoBoxVolume", "Initial Box Volume:");
			moreSimInfo->addControl("infoOriginX", "Origin [X]:");
			moreSimInfo->addControl("infoOriginY", "Origin [Y]:");
			moreSimInfo->addControl("infoOriginZ", "Origin [Z]:");
			moreSimInfo->addControl("infoEnergySyncInterval", "Energy Sync Interval:");
			moreSimInfo->addControl("infoPbcOn", "Periodic Boundary Conditions:");

			moreSimInfo->Location = Point(12, 12);
			moreSimInfo->align();

			author = gcnew Label();
			author->AutoSize = true;
			author->BackColor = Color::Transparent;
			author->ForeColor = Color::FromArgb((Int32)(BackColor.R * 0.8f), (Int32)(BackColor.G * 0.8f), (Int32)(BackColor.B * 0.8f));
			author->Font = gcnew System::Drawing::Font("Microsoft Sans Serif", 6.5f, FontStyle::Italic);
			author->Text = L"Autor: Lukasz Furlepa\r\nProwadzacy: dr hab. Aleksander Dawid\r\nSymulacje i modelowanie komputerowe, UŒ 2015";
			author->Location = Point(12, moreSimInfo->Bottom + 6);
			Controls->Add(author);

			ClientSize = System::Drawing::Size(moreSimInfo->Right + 12, author->Bottom + 6);
		}

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->BackColor = System::Drawing::SystemColors::ActiveCaption;
			this->Name = L"MCSimInfoForm";
			this->Text = L"Additional Information";
			this->ResumeLayout(false);
			this->PerformLayout();
		}
#pragma endregion
	};
}
