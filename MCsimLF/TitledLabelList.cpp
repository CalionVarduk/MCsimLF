#include "TitledLabelList.h"

using namespace MCsimLF::Helpers::UI;

TitledLabelList::TitledLabelList(Control^ parent)
	: TitledVerticalList<Label>(parent)
{}

TitledLabelList::TitledLabelList(Control^ parent, Int32 spacing, Point controlOffset)
	: TitledVerticalList<Label>(parent)
{
	Spacing = spacing;
	ControlOffset = controlOffset;
}

TitledLabel^ TitledLabelList::addControl(String^ name, String^ title)
{ return _addControlOfType<TitledLabel>(name, title); }

TitledLabel^ TitledLabelList::operator[] (Int32 iControl)
{ return _interpretControlAs<TitledLabel>(iControl); }

TitledLabel^ TitledLabelList::getControl(UInt32 iControl)
{
	return (iControl < (UInt32)_controls->Count) ?
		_interpretControlAs<TitledLabel>(iControl) :
		nullptr;
}