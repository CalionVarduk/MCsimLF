#pragma once

#include "MonteCarloGL.h"
#include "SimpleFpsCounter.h"
#include "MCSimSettingsForm.h"
#include "MCViewSettingsForm.h"
#include "MCEnergyGraphForm.h"
#include "MCSimInfoForm.h"

namespace MCsimLF {
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	using namespace MCsimLF::Helpers;
	using namespace MCsimLF::Helpers::UI;
	using namespace MCsimLF::OpenGL;
	using namespace MCsimLF::MC;

	public ref class MCSimForm : public System::Windows::Forms::Form
	{
	public:
		MCSimForm()
		{
			InitializeComponent();

			fixedButtonSize = System::Drawing::Size(65, 20);
			initializeControlPanel(125);
	
			monteCarlo = gcnew MonteCarlo();
			monteCarloGL = gcnew MonteCarloGL(this, 0, controlPanel->Height, ClientSize.Width, ClientSize.Height - controlPanel->Height, monteCarlo);

			simSettings = gcnew MCSimSettingsForm(menu[4]);
			viewSettings = gcnew MCViewSettingsForm(menu[5]);
			energyGraph = gcnew MCEnergyGraphForm(menu[6], monteCarlo);
			moreInfo = gcnew MCSimInfoForm(buttonMoreInfo);

			resetSimulation();
			updateView();

			fpsCounter = gcnew SimpleFpsCounter();

			_setEvents();
			_setPositionDeltas(0.25f, 0.25f, 0.5f);
			_setRotationDeltas(1.0f, 1.0f, 1.0f);
			viewMouseMove(nullptr, nullptr);
			viewMouseWheel(nullptr, nullptr);

			simTimer->Start();
		}

	protected:
		~MCSimForm()
		{
			if (components)
				delete components;
		}

	private:
	#pragma region Fields Declaration
		MCSimSettingsForm^ simSettings;
		MCViewSettingsForm^ viewSettings;
		MCEnergyGraphForm^ energyGraph;
		MCSimInfoForm^ moreInfo;
		MonteCarloGL^ monteCarloGL;
		SimpleFpsCounter^ fpsCounter;
		
		MonteCarlo^ monteCarlo;
		UInt32 metropolisStepsPerFrame;

		System::Drawing::Size fixedButtonSize;
		float posDX, posDY, posDZ;
		float rotDX, rotDY, rotDZ;

		ButtonHorizontalMenu^ menu;

		LabelButton^ buttonRun;
		LabelButton^ buttonPause;
		LabelButton^ buttonMoreInfo;

		System::Windows::Forms::Label^  labelZoomButtons;
		LabelButton^ buttonZoomIn;
		LabelButton^ buttonZoomOut;

		System::Windows::Forms::Label^  labelMoveButtons;
		LabelButton^ buttonMoveLeft;
		LabelButton^ buttonMoveRight;
		LabelButton^ buttonMoveUp;
		LabelButton^ buttonMoveDown;

		System::Windows::Forms::Label^  labelRotateButtons;
		LabelButton^ buttonRotateLeft;
		LabelButton^ buttonRotateRight;
		LabelButton^ buttonRotateUp;
		LabelButton^ buttonRotateDown;

		TitledLabelList^ viewInfo;
		TitledLabelList^ simInfo;

		System::Windows::Forms::Timer^  simTimer;
		System::Windows::Forms::Panel^  controlPanel;

		System::ComponentModel::IContainer^  components;
#pragma endregion

		void simTimer_Tick(Object^ sender, EventArgs^ e)
		{
			handleButtons();

			if(buttonRun->isActivated)
				for(UInt32 i = 0; i < metropolisStepsPerFrame; ++i)
					monteCarlo->metropolisStep();

			monteCarloGL->drawScene();
			energyGraph->updateGraph();
			fpsCounter->countFrame();
			updateInfoLabels();
		}

		void pauseButtonPressed(Object^ sender, MouseEventArgs^ e)
		{ buttonRun->forceDeactivate(); }

		void runButtonPressed(Object^ sender, MouseEventArgs^ e)
		{ buttonPause->forceDeactivate(); }

		void moreInfoButtonPressed(Object^ sender, MouseEventArgs^ e)
		{ moreInfo->showForm(); }

		void loadButtonPressed(Object^ sender, MouseEventArgs^ e)
		{
			buttonRun->forceDeactivate();
			buttonPause->forceActivate();

            OpenFileDialog^ dialog = gcnew OpenFileDialog();
            dialog->Filter = "Monte Carlo Simulation |*.cvmc";
            dialog->ShowDialog();

            if(dialog->FileName != "") {
				if(monteCarlo->loadFromFile(dialog->FileName)) {
					monteCarlo->setEnergySyncInterval(simSettings->parseEnergySyncInterval());
					monteCarloGL->updateSimulation();
					updateSettings(monteCarlo->getInitialBoxSize(), monteCarlo->getOrigin());
					setStaticInfo();
					MessageBox::Show("Simulation loaded successfully!");
				}
				else
					MessageBox::Show("Simulation loading unsuccessful.");
			}
		}

		void saveButtonPressed(Object^ sender, MouseEventArgs^ e)
		{
			buttonRun->forceDeactivate();
			buttonPause->forceActivate();

            SaveFileDialog^ dialog = gcnew SaveFileDialog();
            dialog->Filter = "Monte Carlo Simulation |*.cvmc";
            dialog->ShowDialog();

            if(dialog->FileName != "") {
				monteCarlo->saveToFile(dialog->FileName);
				MessageBox::Show("Simulation saved successfully!");
			}
		}

		void resetSimButtonPressed(Object^ sender, MouseEventArgs^ e)
		{
			buttonRun->forceDeactivate();
			buttonPause->forceActivate();
			resetSimulation();
		}

		void resetViewButtonPressed(Object^ sender, MouseEventArgs^ e)
		{
			monteCarloGL->setPosition(0, 0, -30);
			monteCarloGL->setRotation(0, 0);
			viewMouseMove(sender, e);
			viewMouseWheel(sender, e);
		}

		void settingsSimButtonPressed(Object^ sender, MouseEventArgs^ e)
		{ simSettings->showForm(); }

		void settingsViewButtonPressed(Object^ sender, MouseEventArgs^ e)
		{ viewSettings->showForm(); }

		void graphButtonPressed(Object^ sender, MouseEventArgs^ e)
		{ energyGraph->showForm(); }

		void viewSettingsConfirmPressed(Object^ sender, MouseEventArgs^ e)
		{ updateView(); }

		void simSettingsConfirmPressed(Object^ sender, MouseEventArgs^ e)
		{
			metropolisStepsPerFrame = simSettings->parseStepsPerFrame();
			monteCarlo->setEnergySyncInterval(simSettings->parseEnergySyncInterval());
			setStaticInfo();
		}

		void resetSimulation()
		{
			metropolisStepsPerFrame = simSettings->parseStepsPerFrame();

			SimSettings::pbcOn = false;
			SimSettings::fccOn = simSettings->isFccOn();

			if(SimSettings::fccOn)
				monteCarlo->initializeFcc(simSettings->parseFccAtomCount(), simSettings->parseBoxSize(),
											simSettings->parseTemperature(), simSettings->parseOrigin());
			else
				monteCarlo->initializeNormal(simSettings->parseAtomCount(), simSettings->parseBoxSize(),
												simSettings->parseTemperature(), simSettings->parseOrigin());

			monteCarlo->setEnergySyncInterval(simSettings->parseEnergySyncInterval());
			monteCarloGL->updateSimulation();
			updateSettings(monteCarlo->getInitialBoxSize(), monteCarlo->getOrigin());

			SimSettings::pbcOn = simSettings->isPbcOn();

			setStaticInfo();
			updateInfoLabels();
			energyGraph->resetGraph();
		}

		void updateView()
		{
			monteCarloGL->setSphereNearColor(viewSettings->parseSphereNearColor());
			monteCarloGL->setSphereFarColor(viewSettings->parseSphereFarColor());

			Color background = viewSettings->parseBackgroundColor();
			monteCarloGL->setBackgroundColor(background);
			BackColor = background;
		}

		void updateInfoLabels()
		{
			simInfo[4]->Text = monteCarlo->getStep().ToString();
			simInfo[5]->Text = monteCarlo->getEnergy().ToString() + " zJ";
		}

		void displayFps()
		{
			simInfo[3]->Text = String::Format("{0:0.00}", fpsCounter->getFpsCount());
		}

		void handleButtons()
		{
			handleZoomButtons();
			handleMoveButtons();
			handleRotateButtons();
		}

		void handleZoomButtons()
		{
			if(buttonZoomIn->isActivated) {
				monteCarloGL->offsetPositionZ(posDZ);
				viewInfo[2]->Text = String::Format("{0:0.##}", monteCarloGL->getPositionZ());
			}

			if(buttonZoomOut->isActivated) {
				monteCarloGL->offsetPositionZ(-posDZ);
				viewInfo[2]->Text = String::Format("{0:0.##}", monteCarloGL->getPositionZ());
			}
		}

		void handleMoveButtons()
		{
			if(buttonMoveLeft->isActivated) {
				monteCarloGL->offsetPositionX(posDX);
				viewInfo[0]->Text = String::Format("{0:0.##}", -monteCarloGL->getPositionX());
			}

			if(buttonMoveRight->isActivated) {
				monteCarloGL->offsetPositionX(-posDX);
				viewInfo[0]->Text = String::Format("{0:0.##}", -monteCarloGL->getPositionX());
			}

			if(buttonMoveUp->isActivated) {
				monteCarloGL->offsetPositionY(-posDY);
				viewInfo[1]->Text = String::Format("{0:0.##}", -monteCarloGL->getPositionY());
			}

			if(buttonMoveDown->isActivated) {
				monteCarloGL->offsetPositionY(posDY);
				viewInfo[1]->Text = String::Format("{0:0.##}", -monteCarloGL->getPositionY());
			}
		}

		void handleRotateButtons()
		{
			if(buttonRotateLeft->isActivated) {
				monteCarloGL->offsetRotationX(-rotDX);
				viewInfo[3]->Text = String::Format("{0:0.##}", monteCarloGL->getRotationX());
			}

			if(buttonRotateRight->isActivated) {
				monteCarloGL->offsetRotationX(rotDX);
				viewInfo[3]->Text = String::Format("{0:0.##}", monteCarloGL->getRotationX());
			}

			if(buttonRotateUp->isActivated) {
				monteCarloGL->offsetRotationY(-rotDY);
				viewInfo[4]->Text = String::Format("{0:0.##}", monteCarloGL->getRotationY());
			}

			if(buttonRotateDown->isActivated) {
				monteCarloGL->offsetRotationY(rotDY);
				viewInfo[4]->Text = String::Format("{0:0.##}", monteCarloGL->getRotationY());
			}
		}

		void setStaticInfo()
		{
			simInfo[0]->Text = metropolisStepsPerFrame.ToString();
			simInfo[1]->Text = monteCarlo->getAtomCount().ToString();
			simInfo[2]->Text = String::Format("{0:0.##}", monteCarlo->getTemperature()) + " K";
			moreInfo->setInfo(monteCarlo);
		}

		void updateSettings(Vector3D boxSize, Point3D origin)
		{
			SimSettings::boxOriginX = origin.x;
			SimSettings::boxOriginY = origin.y;
			SimSettings::boxOriginZ = origin.z;

			SimSettings::boxLengthX = boxSize.x;
			SimSettings::boxLengthY = boxSize.y;
			SimSettings::boxLengthZ = boxSize.z;

			SimSettings::boxLengthHalfX = SimSettings::boxLengthX * 0.5;
			SimSettings::boxLengthHalfY = SimSettings::boxLengthY * 0.5;
			SimSettings::boxLengthHalfZ = SimSettings::boxLengthZ * 0.5;
		}

		void viewMouseMove(Object^ sender, MouseEventArgs^ e)
		{
			viewInfo[0]->Text = String::Format("{0:0.##}", -monteCarloGL->getPositionX());
			viewInfo[1]->Text = String::Format("{0:0.##}", -monteCarloGL->getPositionY());
			viewInfo[3]->Text = String::Format("{0:0.##}", monteCarloGL->getRotationX());
			viewInfo[4]->Text = String::Format("{0:0.##}", monteCarloGL->getRotationY());
		}

		void viewMouseWheel(Object^ sender, MouseEventArgs^ e)
		{
			viewInfo[2]->Text = String::Format("{0:0.##}", monteCarloGL->getPositionZ());
		}

		void MCsimForm_Resize(Object^ sender, EventArgs^ e)
		{
			controlPanel->Width = ClientSize.Width + 2;
			menu->Width = ClientSize.Width;
			monteCarloGL->resize(ClientSize.Width, ClientSize.Height - controlPanel->Height);
		}

#pragma region Control Panel Initialization
		void initializeControlPanel(UInt32 height)
		{
			controlPanel->Size = System::Drawing::Size(ClientSize.Width + 2, height + 2);
			initializeStripMenu();
			initializeButtons();
			initializeTitledLabels();
			initializeControlButtons(Point(simInfo[0]->ControlLeft + 65, simInfo->Top + (fixedButtonSize.Height >> 2)));
		}
	
		void initializeButtons()
		{
			Int32 halfPanelHeight = (controlPanel->Height + menu->Height) >> 1;

			initializeZoomButtons(Point(5, halfPanelHeight - (fixedButtonSize.Height >> 1)));
			initializeMoveButtons(Point(buttonZoomIn->Right + 8, halfPanelHeight - fixedButtonSize.Height));
			initializeRotateButtons(Point(buttonMoveRight->Right + 8, halfPanelHeight - fixedButtonSize.Height));
		}

		void initializeStripMenu()
		{
			menu = gcnew ButtonHorizontalMenu(controlPanel, "menu", 15, Point(5, 3));
			menu->Location = Point(0, 0);
			menu->BackColor = Color::SteelBlue;
			menu->ForeColor = Color::WhiteSmoke;
			menu->Size = System::Drawing::Size(controlPanel->Width, 24);

			menu->addButton("menuButtonLoad", "LOAD FROM FILE");
			menu->addButton("menuButtonSave", "SAVE TO FILE");
			menu->addButton("menuButtonResetSim", "RESET SIMULATION");
			menu->addButton("menuButtonResetView", "RESET VIEW");
			menu->addButton("menuButtonSettingsSim", "SIMULATION SETTINGS");
			menu->addButton("menuButtonSettingsView", "VIEW SETTINGS");
			menu->addButton("menuButtonGraph", "ENERGY GRAPH");

			setMenuButtonProperties(menu[0], true);
			setMenuButtonProperties(menu[1], true);
			setMenuButtonProperties(menu[2], true);
			setMenuButtonProperties(menu[3], true);
			setMenuButtonProperties(menu[4], false);
			setMenuButtonProperties(menu[5], false);
			setMenuButtonProperties(menu[6], false);
		}

		void setMenuButtonProperties(LabelButton^ button, bool activatedOnHold)
		{
			button->normalBackColor = menu->BackColor;
			button->normalForeColor = menu->ForeColor;
			button->mouseOverBackColor = Color::LightSteelBlue;
			button->mouseOverForeColor = menu->ForeColor;
			button->activeBackColor = Color::LightSteelBlue;
			button->activeForeColor = menu->ForeColor;
			button->activeMouseOverBackColor = Color::LightSteelBlue;
			button->activeMouseOverForeColor = menu->ForeColor;
			button->runOnMouseClickProcedure = true;
			button->isActivatedOnHold = activatedOnHold;
			button->isOnClickDeactivatable = activatedOnHold;
			setButtonColorChanging(button);
		}

		void initializeControlButtons(Point origin)
		{
			buttonRun = gcnew LabelButton(controlPanel, "buttonRun", "RUN");
			buttonRun->setFixedSize(fixedButtonSize);
			setControlButtonProperties(buttonRun);
			buttonRun->Location = origin;

			buttonPause = gcnew LabelButton(controlPanel, "buttonPause", "PAUSE");
			buttonPause->setFixedSize(fixedButtonSize);
			setControlButtonProperties(buttonPause);
			buttonPause->Location = Point(buttonRun->Right - 1, origin.Y);
		
			buttonPause->forceActivate();
		}

		void setControlButtonProperties(LabelButton^ button)
		{
			button->normalBackColor = Color::DarkGray;
			button->normalForeColor = Color::Maroon;
			button->mouseOverBackColor = Color::Silver;
			button->mouseOverForeColor = Color::Firebrick;
			button->activeBackColor = Color::WhiteSmoke;
			button->activeForeColor = Color::Firebrick;
			button->activeMouseOverBackColor = Color::WhiteSmoke;
			button->activeMouseOverForeColor = Color::Red;
			button->runOnMouseClickProcedure = true;
			button->isActivatedOnHold = false;
			button->isOnClickDeactivatable = false;
			setButtonColorChanging(button);
		}

		void initializeZoomButtons(Point origin)
		{
			buttonZoomIn = gcnew LabelButton(controlPanel, "buttonZoomIn", "In");
			buttonZoomIn->setFixedSize(fixedButtonSize);
			setZoomButtonProperties(buttonZoomIn);
			buttonZoomIn->Location = origin;

			buttonZoomOut = gcnew LabelButton(controlPanel, "buttonZoomOut", "Out");
			buttonZoomOut->setFixedSize(fixedButtonSize);
			setZoomButtonProperties(buttonZoomOut);
			buttonZoomOut->Location = Point(origin.X, buttonZoomIn->Bottom - 1);

			labelZoomButtons->ForeColor = buttonZoomIn->normalForeColor;
			labelZoomButtons->Location = Point(origin.X + ((fixedButtonSize.Width - labelZoomButtons->Width) >> 1),
												origin.Y - labelZoomButtons->Height - 3);
		}

		void setZoomButtonProperties(LabelButton^ button)
		{
			button->normalBackColor = Color::Khaki;
			button->normalForeColor = Color::Chocolate;
			button->mouseOverBackColor = Color::LemonChiffon;
			button->mouseOverForeColor = Color::Peru;
			button->activeBackColor = Color::Ivory;
			button->activeForeColor = Color::DarkOrange;
			button->activeMouseOverBackColor = Color::Ivory;
			button->activeMouseOverForeColor = Color::DarkOrange;
			button->isActivatedOnHold = true;
			setButtonColorChanging(button);
		}

		void initializeMoveButtons(Point origin)
		{
			buttonMoveLeft = gcnew LabelButton(controlPanel, "buttonMoveLeft", "Left");
			buttonMoveLeft->setFixedSize(fixedButtonSize);
			setMoveButtonProperties(buttonMoveLeft);
			buttonMoveLeft->Location = Point(origin.X, origin.Y + fixedButtonSize.Height);
			
			buttonMoveRight = gcnew LabelButton(controlPanel, "buttonMoveRight", "Right");
			buttonMoveRight->setFixedSize(fixedButtonSize);
			setMoveButtonProperties(buttonMoveRight);
			buttonMoveRight->Location = Point(buttonMoveLeft->Right - 1, buttonMoveLeft->Top);
			
			buttonMoveUp = gcnew LabelButton(controlPanel, "buttonMoveUp", "Up");
			buttonMoveUp->setFixedSize(fixedButtonSize);
			setMoveButtonProperties(buttonMoveUp);
			buttonMoveUp->Location = Point(buttonMoveLeft->Left + (fixedButtonSize.Width >> 1), origin.Y + 1);

			buttonMoveDown = gcnew LabelButton(controlPanel, "buttonMoveDown", "Down");
			buttonMoveDown->setFixedSize(fixedButtonSize);
			setMoveButtonProperties(buttonMoveDown);
			buttonMoveDown->Location = Point(buttonMoveUp->Left, buttonMoveLeft->Bottom - 1);

			labelMoveButtons->ForeColor = buttonMoveLeft->normalForeColor;
			labelMoveButtons->Location = Point(buttonMoveUp->Left + ((fixedButtonSize.Width - labelMoveButtons->Width) >> 1),
												buttonMoveUp->Top - labelMoveButtons->Height - 3);
		}

		void setMoveButtonProperties(LabelButton^ button)
		{
			button->normalBackColor = Color::LightSteelBlue;
			button->normalForeColor = Color::Navy;
			button->mouseOverBackColor = Color::LightBlue;
			button->mouseOverForeColor = Color::RoyalBlue;
			button->activeBackColor = Color::Azure;
			button->activeForeColor = Color::CornflowerBlue;
			button->activeMouseOverBackColor = Color::Azure;
			button->activeMouseOverForeColor = Color::CornflowerBlue;
			button->isActivatedOnHold = true;
			setButtonColorChanging(button);
		}

		void initializeRotateButtons(Point origin)
		{
			buttonRotateLeft = gcnew LabelButton(controlPanel, "buttonRotateLeft", "Left");
			buttonRotateLeft->setFixedSize(fixedButtonSize);
			setRotateButtonProperties(buttonRotateLeft);
			buttonRotateLeft->Location = Point(origin.X, origin.Y + fixedButtonSize.Height);

			buttonRotateRight = gcnew LabelButton(controlPanel, "buttonRotateRight", "Right");
			buttonRotateRight->setFixedSize(fixedButtonSize);
			setRotateButtonProperties(buttonRotateRight);
			buttonRotateRight->Location = Point(buttonRotateLeft->Right - 1, buttonRotateLeft->Top);

			buttonRotateUp = gcnew LabelButton(controlPanel, "buttonRotateUp", "Up");
			buttonRotateUp->setFixedSize(fixedButtonSize);
			setRotateButtonProperties(buttonRotateUp);
			buttonRotateUp->Location = Point(buttonRotateLeft->Left + (fixedButtonSize.Width >> 1), origin.Y + 1);

			buttonRotateDown = gcnew LabelButton(controlPanel, "buttonRotateDown", "Down");
			buttonRotateDown->setFixedSize(fixedButtonSize);
			setRotateButtonProperties(buttonRotateDown);
			buttonRotateDown->Location = Point(buttonRotateUp->Left, buttonRotateLeft->Bottom - 1);

			labelRotateButtons->ForeColor = buttonRotateLeft->normalForeColor;
			labelRotateButtons->Location = Point(buttonRotateUp->Left + ((fixedButtonSize.Width - labelRotateButtons->Width) >> 1),
												buttonRotateUp->Top - labelRotateButtons->Height - 3);
		}

		void setRotateButtonProperties(LabelButton^ button)
		{
			button->normalBackColor = Color::MediumAquamarine;
			button->normalForeColor = Color::DarkGreen;
			button->mouseOverBackColor = Color::Aquamarine;
			button->mouseOverForeColor = Color::Green;
			button->activeBackColor = Color::Honeydew;
			button->activeForeColor = Color::ForestGreen;
			button->activeMouseOverBackColor = Color::Honeydew;
			button->activeMouseOverForeColor = Color::ForestGreen;
			button->isActivatedOnHold = true;
			setButtonColorChanging(button);
		}

		void setButtonColorChanging(LabelButton^ button)
		{
			button->changeColorsOnActivation = true;
			button->changeColorsOnMouseOver = true;
			button->resetDisplayedColors();
		}

		void initializeTitledLabels()
		{
			initializeViewLabels(Point(buttonRotateRight->Right + 10, menu->Height + 19));

			buttonMoreInfo = gcnew LabelButton(controlPanel, "buttonMoreInfo", "M\nO\nR\nE\n \nI\nN\nF\nO");
			buttonMoreInfo->Font = gcnew System::Drawing::Font(L"Microsoft Sans Serif", 6, FontStyle::Regular, GraphicsUnit::Point, (Byte)238);
			buttonMoreInfo->setFixedSize(System::Drawing::Size(15, controlPanel->Height - menu->Height - 10));
			setControlButtonProperties(buttonMoreInfo);
			buttonMoreInfo->Location = Point(viewInfo[0]->ControlLeft + 60, viewInfo->Top - viewInfo[0]->Height);

			initializeSimulationLabels(Point(buttonMoreInfo->Right + 8, viewInfo->Top - 15));
		}

		void initializeViewLabels(Point origin)
		{
			viewInfo = gcnew TitledLabelList(controlPanel, 0, Point(2, 0));

			viewInfo->addControl("infoPositionX", "View Location [X]:");
			viewInfo->addControl("infoPositionY", "View Location [Y]:");
			viewInfo->addControl("infoPositionZ", "View Location [Z]:");
			viewInfo->addControl("infoRotationX", "View Angle [X]:");
			viewInfo->addControl("infoRotationY", "View Angle [Y]:");

			viewInfo->Location = origin;
			viewInfo->align();

			for(Int32 i = 0; i < viewInfo->Count; ++i) {
				viewInfo[i]->TitleForeColor = Color::Black;
				viewInfo[i]->ControlForeColor = Color::Black;
			}
		}

		void initializeSimulationLabels(Point origin)
		{
			simInfo = gcnew TitledLabelList(controlPanel, 0, Point(2, 0));

			simInfo->addControl("infoStepsPerFrame", "Steps per Frame:");
			simInfo->addControl("infoParticles", "Atoms:");
			simInfo->addControl("infoTemperature", "Temperature:");
			simInfo->addControl("infoFps", "Frames per Second:");
			simInfo->addControl("infoSimSteps", "Simulation Steps:");
			simInfo->addControl("infoEnergy", "Energy:");

			simInfo->Location = origin;
			simInfo->align();

			for(Int32 i = 0; i < 3; ++i) {
				simInfo[i]->TitleForeColor = Color::Black;
				simInfo[i]->ControlForeColor = Color::Black;
			}
			for(Int32 i = 3; i < simInfo->Count; ++i) {
				simInfo[i]->TitleForeColor = labelMoveButtons->ForeColor;
				simInfo[i]->ControlForeColor = labelRotateButtons->ForeColor;
			}

			simInfo[3]->Text = String::Format("{0:0.00}", 60.0);
		}
#pragma endregion

#pragma region Additional initialization
		void _setPositionDeltas(float dx, float dy, float dz)
		{
			posDX = dx;
			posDY = dy;
			posDZ = dz;
			monteCarloGL->setPositionMouseDeltas(dx * 0.25f, dy * 0.25f, dz * 0.0625f);
		}

		void _setRotationDeltas(float dx, float dy, float dz)
		{
			rotDX = dx;
			rotDY = dy;
			rotDZ = dz;
			monteCarloGL->setRotationMouseDeltas(dx * 0.125f, dy * 0.125f);
		}

		void _setEvents()
		{
			simSettings->addConfirmEvent(gcnew LabelButtonMouseClickHandler(this, &MCSimForm::simSettingsConfirmPressed));
			viewSettings->addConfirmEvent(gcnew LabelButtonMouseClickHandler(this, &MCSimForm::viewSettingsConfirmPressed));

			monteCarloGL->MouseMove += gcnew OpenGLMouseEventHandler(this, &MCSimForm::viewMouseMove);
			MouseWheel += gcnew MouseEventHandler(this, &MCSimForm::viewMouseWheel);

			menu[0]->MouseClickExtension += gcnew LabelButtonMouseClickHandler(this, &MCSimForm::loadButtonPressed);
			menu[1]->MouseClickExtension += gcnew LabelButtonMouseClickHandler(this, &MCSimForm::saveButtonPressed);
			menu[2]->MouseClickExtension += gcnew LabelButtonMouseClickHandler(this, &MCSimForm::resetSimButtonPressed);
			menu[3]->MouseClickExtension += gcnew LabelButtonMouseClickHandler(this, &MCSimForm::resetViewButtonPressed);
			menu[4]->MouseClickExtension += gcnew LabelButtonMouseClickHandler(this, &MCSimForm::settingsSimButtonPressed);
			menu[5]->MouseClickExtension += gcnew LabelButtonMouseClickHandler(this, &MCSimForm::settingsViewButtonPressed);
			menu[6]->MouseClickExtension += gcnew LabelButtonMouseClickHandler(this, &MCSimForm::graphButtonPressed);

			buttonRun->MouseClickExtension += gcnew LabelButtonMouseClickHandler(this, &MCSimForm::runButtonPressed);
			buttonPause->MouseClickExtension += gcnew LabelButtonMouseClickHandler(this, &MCSimForm::pauseButtonPressed);
			buttonMoreInfo->MouseClickExtension += gcnew LabelButtonMouseClickHandler(this, &MCSimForm::moreInfoButtonPressed);

			fpsCounter->FrameCountUpdate += gcnew FpsCounterUpdateHandler(this, &MCSimForm::displayFps);
		}
#pragma endregion

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->simTimer = (gcnew System::Windows::Forms::Timer(this->components));
			this->labelMoveButtons = (gcnew System::Windows::Forms::Label());
			this->labelRotateButtons = (gcnew System::Windows::Forms::Label());
			this->labelZoomButtons = (gcnew System::Windows::Forms::Label());
			this->controlPanel = (gcnew System::Windows::Forms::Panel());
			this->controlPanel->SuspendLayout();
			this->SuspendLayout();
			// 
			// simTimer
			// 
			this->simTimer->Interval = 15;
			this->simTimer->Tick += gcnew System::EventHandler(this, &MCSimForm::simTimer_Tick);
			// 
			// labelMoveButtons
			// 
			this->labelMoveButtons->AutoSize = true;
			this->labelMoveButtons->BackColor = System::Drawing::Color::Transparent;
			this->labelMoveButtons->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(238)));
			this->labelMoveButtons->Location = System::Drawing::Point(33, 15);
			this->labelMoveButtons->Name = L"labelMoveButtons";
			this->labelMoveButtons->Size = System::Drawing::Size(46, 17);
			this->labelMoveButtons->TabIndex = 0;
			this->labelMoveButtons->Text = L"Move";
			// 
			// labelRotateButtons
			// 
			this->labelRotateButtons->AutoSize = true;
			this->labelRotateButtons->BackColor = System::Drawing::Color::Transparent;
			this->labelRotateButtons->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(238)));
			this->labelRotateButtons->Location = System::Drawing::Point(113, 15);
			this->labelRotateButtons->Name = L"labelRotateButtons";
			this->labelRotateButtons->Size = System::Drawing::Size(56, 17);
			this->labelRotateButtons->TabIndex = 1;
			this->labelRotateButtons->Text = L"Rotate";
			// 
			// labelZoomButtons
			// 
			this->labelZoomButtons->AutoSize = true;
			this->labelZoomButtons->BackColor = System::Drawing::Color::Transparent;
			this->labelZoomButtons->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(238)));
			this->labelZoomButtons->Location = System::Drawing::Point(205, 15);
			this->labelZoomButtons->Name = L"labelZoomButtons";
			this->labelZoomButtons->Size = System::Drawing::Size(48, 17);
			this->labelZoomButtons->TabIndex = 2;
			this->labelZoomButtons->Text = L"Zoom";
			// 
			// controlPanel
			// 
			this->controlPanel->BackColor = System::Drawing::SystemColors::ActiveCaption;
			this->controlPanel->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->controlPanel->Controls->Add(this->labelZoomButtons);
			this->controlPanel->Controls->Add(this->labelRotateButtons);
			this->controlPanel->Controls->Add(this->labelMoveButtons);
			this->controlPanel->Location = System::Drawing::Point(-1, -1);
			this->controlPanel->Name = L"controlPanel";
			this->controlPanel->Size = System::Drawing::Size(200, 100);
			this->controlPanel->TabIndex = 17;
			// 
			// MCSimForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::ActiveCaption;
			this->ClientSize = System::Drawing::Size(1024, 768);
			this->Controls->Add(this->controlPanel);
			this->MinimumSize = System::Drawing::Size(1040, 806);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Name = L"MCSimForm";
			this->Text = L"Monte Carlo Simulation";
			this->Resize += gcnew System::EventHandler(this, &MCSimForm::MCsimForm_Resize);
			this->controlPanel->ResumeLayout(false);
			this->controlPanel->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion
};
}
