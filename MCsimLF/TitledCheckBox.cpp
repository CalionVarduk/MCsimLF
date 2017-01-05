#include "TitledCheckBox.h"

using namespace MCsimLF::Helpers::UI;

TitledCheckBox::TitledCheckBox(Control^ parent, String^ name)
	: TitledControl<Panel>(parent, name)
{
	_initializePanel(name);
}

TitledCheckBox::TitledCheckBox(Control^ parent, String^ name, String^ title)
	: TitledControl<Panel>(parent, name, title)
{
	_initializePanel(name);
}

void TitledCheckBox::invalidate()
{
	_brush->Color = (isActivated) ? _control->ForeColor : _control->BackColor;
	_control->CreateGraphics()->FillRectangle(_brush, 1, 1, _control->Width - 4, _control->Height - 4);
}

void TitledCheckBox::forceActivate()
{
	isActivated = true;
	invalidate();
}

void TitledCheckBox::forceDeactivate()
{
	isActivated = false;
	invalidate();
}

void TitledCheckBox::_initializePanel(String^ name)
{
	MouseClick = nullptr;
	isActivated = false;

	_control->Size = System::Drawing::Size(11, 11);
	_control->BackColor = Color::White;
	_control->ForeColor = Color::DodgerBlue;
	_control->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
	_control->MouseClick += gcnew MouseEventHandler(this, &TitledCheckBox::Event_MouseClick);

	_brush = gcnew SolidBrush(_control->ForeColor);
}

void TitledCheckBox::Event_MouseClick(Object^ sender, MouseEventArgs^ e)
{
	isActivated = !isActivated;
	invalidate();

	if(MouseClick != nullptr)
		MouseClick->Invoke(sender, e);
}