#include "TitledTextBoxList.h"

using namespace MCsimLF::Helpers::UI;

TitledTextBoxList::TitledTextBoxList(Control^ parent)
	: TitledVerticalList<TextBox>(parent)
{}

TitledTextBoxList::TitledTextBoxList(Control^ parent, Int32 spacing, Point controlOffset)
	: TitledVerticalList<TextBox>(parent)
{
	Spacing = spacing;
	ControlOffset = controlOffset;
}

TitledTextBox^ TitledTextBoxList::addControl(String^ name, String^ title)
{ return _addControlOfType<TitledTextBox>(name, title); }

TitledTextBox^ TitledTextBoxList::operator[] (Int32 iControl)
{ return _interpretControlAs<TitledTextBox>(iControl); }

TitledTextBox^ TitledTextBoxList::getControl(UInt32 iControl)
{
	return (iControl < (UInt32)_controls->Count) ?
		_interpretControlAs<TitledTextBox>(iControl) :
		nullptr;
}