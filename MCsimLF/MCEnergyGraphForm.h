#pragma once

#include "SecondaryForm.h"
#include "GraphBox.h"

namespace MCsimLF {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	using namespace MCsimLF::Helpers;
	using namespace MCsimLF::MC;

	public ref class MCEnergyGraphForm : public SecondaryForm
	{
	public:
		MCEnergyGraphForm(LabelButton^ _controlButton, MonteCarlo^ _monteCarlo) : SecondaryForm(_controlButton)
		{
			InitializeComponent();
			_initializeGraph(700, 400);

			monteCarlo = _monteCarlo;

			KeyDown += gcnew KeyEventHandler(this, &MCEnergyGraphForm::Event_KeyDown);
			KeyUp += gcnew KeyEventHandler(this, &MCEnergyGraphForm::Event_KeyUp);

			redrawCounter = 0;
			redrawInterval = 6;
			graphMouseX = -1;
			keyLeftDown = false;
			keyRightDown = false;
		}

		void updateGraph()
		{
			graphInfo[2]->Text = graph->StepsPerPixel.ToString();

			if(!graph->AutoFollow) {
				if(keyLeftDown)
					graph->ViewPoint -= 5;
				if(keyRightDown)
					graph->ViewPoint += 5;
			}

			if(graphMouseX >= 0) {
				UInt32 step = graph->getMousedOverStep(graphMouseX);
				if(step <= monteCarlo->getStep()) {
					double energy = monteCarlo->getEnergyValueAt(step);
					graphInfo[0]->Text = step.ToString();
					graphInfo[1]->Text = energy.ToString();
				}
				else {
					graphInfo[0]->Text = "---";
					graphInfo[1]->Text = "---";
				}
			}

			if(redrawCounter++ >= redrawInterval) {
				graph->redraw(monteCarlo);
				redrawCounter = 0;
			}
		}

		void resetGraph()
		{ graph->reset(); }

		void Event_KeyDown(Object^ sender, KeyEventArgs^ e)
		{
			if(!graph->AutoFollow) {
				if(e->KeyCode == Keys::Left)
					keyLeftDown = true;
				else if(e->KeyCode == Keys::Right)
					keyRightDown = true;
			}
		}

		void Event_KeyUp(Object^ sender, KeyEventArgs^ e)
		{
			if(e->KeyCode == Keys::Left)
				keyLeftDown = false;
			else if(e->KeyCode == Keys::Right)
				keyRightDown = false;
		}

	protected:
		virtual void _customInvalidation() override
		{
			checkBoxes->invalidateAll();
		}

	private:
#pragma region Fields Declaration
		GraphBox^ graph;
		MonteCarlo^ monteCarlo;

		bool keyLeftDown;
		bool keyRightDown;

		UInt32 redrawCounter;
		UInt32 redrawInterval;
		Int32 graphMouseX;

		TitledLabelList^ graphInfo;
		TitledCheckBoxList^ checkBoxes;
#pragma endregion

		void Event_MouseMoveGraph(Object^ sender, MouseEventArgs^ e)
		{ graphMouseX = e->X; }

		void Event_MouseMoveForm(Object^ sender, MouseEventArgs^ e)
		{
			graphMouseX = -1;
			graphInfo[0]->Text = "---";
			graphInfo[1]->Text = "---";
		}

		void Event_AutoFollowClicked(Object^ sender, MouseEventArgs^ e)
		{ graph->AutoFollow = !graph->AutoFollow; }

		void Event_ToHalfClicked(Object^ sender, MouseEventArgs^ e)
		{ graph->DrawToHalf = !graph->DrawToHalf; }

		void Event_ScalingClicked(Object^ sender, MouseEventArgs^ e)
		{ graph->LocalScaling = !graph->LocalScaling; }

		void Event_MouseWheel(Object^ sender, MouseEventArgs^ e)
		{ graph->StepsPerPixel += (e->Delta > 0) ? -1 : 1; }

#pragma region Custom Initialization
		void _initializeGraph(UInt32 width, UInt32 height)
		{
			graph = gcnew GraphBox(this, "graph", width, height);
			graph->DrawToHalf = false;

			graph->Location = Point(12, 12);
			graph->addMouseMoveEvent(gcnew MouseEventHandler(this, &MCEnergyGraphForm::Event_MouseMoveGraph));
			MouseMove += gcnew MouseEventHandler(this, &MCEnergyGraphForm::Event_MouseMoveForm);
			MouseWheel += gcnew MouseEventHandler(this, &MCEnergyGraphForm::Event_MouseWheel);

			graphInfo = gcnew TitledLabelList(this, 0, Point(10, 0));

			graphInfo->addControl("infoMouseStep", "Simulation's Step:");
			graphInfo->addControl("infoMouseEnergy", "Energy at Step:");
			graphInfo->addControl("infoStepsPerPixel", "Steps per Pixel:");

			graphInfo[0]->Text = "---";
			graphInfo[1]->Text = "---";
			graphInfo[2]->Text = graph->StepsPerPixel.ToString();

			graphInfo->Location = Point(graph->Right + 12, graph->Top);
			graphInfo->align();

			checkBoxes = gcnew TitledCheckBoxList(this);

			checkBoxes->addControl("boxAutoFollow", "Auto Follow the Drawn Energy");
			checkBoxes->addControl("boxDrawToHalf", "Draw Energy to the Half of the Graph");
			checkBoxes->addControl("boxLocalScaling", "Scale Graph Locally");

			checkBoxes->ControlOffset = Point(10, checkBoxes[0]->VerticalAllignment + 1);
			checkBoxes->Location = Point(graphInfo->Left, graphInfo->Bottom + 20);
			checkBoxes->align();

			checkBoxes[0]->MouseClick += gcnew CheckBoxMouseClickHandler(this, &MCEnergyGraphForm::Event_AutoFollowClicked);
			checkBoxes[1]->MouseClick += gcnew CheckBoxMouseClickHandler(this, &MCEnergyGraphForm::Event_ToHalfClicked);
			checkBoxes[2]->MouseClick += gcnew CheckBoxMouseClickHandler(this, &MCEnergyGraphForm::Event_ScalingClicked);

			ClientSize = System::Drawing::Size(graphInfo->Right + 150, graph->Bottom + 12);

			if(graph->AutoFollow)
				checkBoxes[0]->forceActivate();

			if(graph->DrawToHalf)
				checkBoxes[1]->forceActivate();

			if(graph->LocalScaling)
				checkBoxes[2]->forceActivate();
		}
#pragma endregion

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->BackColor = System::Drawing::SystemColors::ActiveCaption;
			this->Name = L"MCEnergyGraphForm";
			this->Text = L"Energy Graph";
			this->ResumeLayout(false);
			this->PerformLayout();
		}
#pragma endregion
	};
}
