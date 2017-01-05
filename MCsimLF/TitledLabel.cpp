#include "TitledLabel.h"

using namespace MCsimLF::Helpers::UI;

TitledLabel::TitledLabel(Control^ parent, String^ name)
	: TitledControl<Label>(parent, name)
{
	_initializeLabel(name);
}

TitledLabel::TitledLabel(Control^ parent, String^ name, String^ title)
	: TitledControl<Label>(parent, name, title)
{
	_initializeLabel(name);
}

void TitledLabel::_initializeLabel(String^ name)
{
	_control->AutoSize = true;
	_control->BackColor = Color::Transparent;
	_control->Font = gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.5f, FontStyle::Bold, GraphicsUnit::Point, (Byte)(238));
	Offset = Point(5, 0);
	Location = Point(0, 0);
}