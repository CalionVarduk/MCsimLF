#include "TitledTextBox.h"

using namespace MCsimLF::Helpers::UI;

TitledTextBox::TitledTextBox(Control^ parent, String^ name)
	: TitledControl<TextBox>(parent, name)
{
	_initializeTextBox(name);
}

TitledTextBox::TitledTextBox(Control^ parent, String^ name, String^ title)
	: TitledControl<TextBox>(parent, name, title)
{
	_initializeTextBox(name);
}

void TitledTextBox::addTextChangedEvent(EventHandler^ handler)
{ _control->TextChanged += handler; }

void TitledTextBox::_initializeTextBox(String^ name)
{
	_control->BackColor = System::Drawing::SystemColors::InactiveCaption;
	_control->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
	_control->Font = gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, FontStyle::Bold, GraphicsUnit::Point, (Byte)238);
	_control->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
	Offset = Point(5, 0);
	Location = Point(0, 0);
}