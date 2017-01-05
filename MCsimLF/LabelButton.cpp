#include "LabelButton.h"

using namespace MCsimLF::Helpers::UI;

LabelButton::LabelButton(Control^ parent, String^ name, String^ text)
	: Label()
{
	_initializeLabelButton();
	Name = name;
	Text = text;
	parent->Controls->Add(this);
}

void LabelButton::setFixedSize(System::Drawing::Size size)
{
	AutoSize = false;
	Size = size;
}

void LabelButton::resetDisplayedColors()
{
	BackColor = normalBackColor;
	ForeColor = normalForeColor;
}

void LabelButton::forceActivate()
{
	if(isEnabled && isActivatedOnClick) {
		isActivated = true;
		_activateColorChange();
	}
}

void LabelButton::forceDeactivate()
{
	if(isEnabled && isActivatedOnClick) {
		isActivated = false;
		_activateColorChange();
	}
}

void LabelButton::enable()
{
	if(!isEnabled) {
		isEnabled = true;
		BackColor = normalBackColor;
		ForeColor = normalForeColor;
	}
}

void LabelButton::disable()
{
	if(isEnabled) {
		isEnabled = false;
		isActivated = false;
		isMouseDown = false;
		BackColor = disabledBackColor;
		ForeColor = disabledForeColor;
	}
}

void LabelButton::Event_MouseClick(Object^ sender, MouseEventArgs^ e)
{
	if(isEnabled) {
		if(isActivatedOnClick) {
			if(isOnClickDeactivatable || !isActivated) {
				isActivated = !isActivated;
				_activateColorChange();
			}
		}

		if(runOnMouseClickProcedure)
			if(MouseClickExtension != nullptr)
				MouseClickExtension->Invoke(sender, e);
	}
}

void LabelButton::Event_MouseDown(Object^ sender, MouseEventArgs^ e)
{
	if(isEnabled) {
		isMouseDown = true;

		if(isActivatedOnHold) {
			isActivated = true;
			_activateColorChange();
		}
	}
}

void LabelButton::Event_MouseUp(Object^ sender, MouseEventArgs^ e)
{
	if(isEnabled) {
		isMouseDown = false;

		if(isActivatedOnHold) {
			isActivated = false;
			_activateColorChange();
		}
	}
}

void LabelButton::Event_MouseMove(Object^ sender, MouseEventArgs^ e)
{
	isMouseMoving = true;
	isMouseHovering = false;
}

void LabelButton::Event_MouseEnter(Object^ sender, EventArgs^ e)
{
	isMouseWithinBounds = true;

	if(isEnabled)
		_mouseEnterColorChange();
}

void LabelButton::Event_MouseLeave(Object^ sender, EventArgs^ e)
{
	isMouseWithinBounds = false;
	isMouseMoving = false;
	isMouseHovering = false;
	isMouseDown = false;

	if(isActivatedOnHold)
		isActivated = false;

	if(isEnabled)
		_mouseLeaveColorChange();
}

void LabelButton::Event_MouseHover(Object^ sender, EventArgs^ e)
{
	isMouseMoving = false;
	isMouseHovering = true;
}

void LabelButton::_activateColorChange()
{
	if(changeColorsOnActivation) {
		if(isActivated) {
			if(isMouseWithinBounds && changeColorsOnMouseOver) {
				BackColor = activeMouseOverBackColor;
				ForeColor = activeMouseOverForeColor;
			}
			else {
				BackColor = activeBackColor;
				ForeColor = activeForeColor;
			}
		}
		else if(isMouseWithinBounds && changeColorsOnMouseOver) {
			BackColor = mouseOverBackColor;
			ForeColor = mouseOverForeColor;
		}
		else {
			BackColor = normalBackColor;
			ForeColor = normalForeColor;
		}
	}
}

void LabelButton::_mouseEnterColorChange()
{
	if(changeColorsOnMouseOver) {
		if(isActivated && changeColorsOnActivation) {
			BackColor = activeMouseOverBackColor;
			ForeColor = activeMouseOverForeColor;
		}
		else {
			BackColor = mouseOverBackColor;
			ForeColor = mouseOverForeColor;
		}
	}
}

void LabelButton::_mouseLeaveColorChange()
{
	if(changeColorsOnMouseOver) {
		if(isActivated && changeColorsOnActivation) {
			BackColor = activeBackColor;
			ForeColor = activeForeColor;
		}
		else {
			BackColor = normalBackColor;
			ForeColor = normalForeColor;
		}
	}
}

void LabelButton::_initializeLabelButton()
{
	MouseClickExtension = nullptr;

	runOnMouseClickProcedure = false;
	isMouseDown = false;
	isMouseMoving = false;
	isMouseWithinBounds = false;
	isMouseHovering = false;
	isActivated = false;
	isActivatedOnClick = true;
	changeColorsOnMouseOver = false;
	changeColorsOnActivation = false;
	isOnClickDeactivatable = true;
	isEnabled = true;

	Font = gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, FontStyle::Regular, GraphicsUnit::Point, (Byte)238);
	BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
	TextAlign = System::Drawing::ContentAlignment::MiddleCenter;

	_initializeColors();
	_initializeEvents();
}

void LabelButton::_initializeColors()
{
	BackColor = Color::White;
	ForeColor = Color::Black;
	normalBackColor = mouseOverBackColor = activeBackColor = activeMouseOverBackColor = Color::White;
	normalForeColor = mouseOverForeColor = activeForeColor = activeMouseOverForeColor = Color::Black;
	disabledBackColor = Color::LightGray;
	disabledForeColor = Color::DarkGray;
}

void LabelButton::_initializeEvents()
{
	MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &LabelButton::Event_MouseClick);
	MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &LabelButton::Event_MouseDown);
	MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &LabelButton::Event_MouseUp);
	MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &LabelButton::Event_MouseMove);
	MouseEnter += gcnew System::EventHandler(this, &LabelButton::Event_MouseEnter);
	MouseLeave += gcnew System::EventHandler(this, &LabelButton::Event_MouseLeave);
	MouseHover += gcnew System::EventHandler(this, &LabelButton::Event_MouseHover);
}