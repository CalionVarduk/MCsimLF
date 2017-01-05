#include "TitledCheckBoxList.h"

using namespace MCsimLF::Helpers::UI;

TitledCheckBoxList::TitledCheckBoxList(Control^ parent)
	: TitledVerticalList<Panel>(parent)
{}

TitledCheckBoxList::TitledCheckBoxList(Control^ parent, Int32 spacing, Point controlOffset)
	: TitledVerticalList<Panel>(parent)
{
	Spacing = spacing;
	ControlOffset = controlOffset;
}

TitledCheckBox^ TitledCheckBoxList::addControl(String^ name, String^ title)
{ return _addControlOfType<TitledCheckBox>(name, title); }

TitledCheckBox^ TitledCheckBoxList::operator[] (Int32 iControl)
{ return _interpretControlAs<TitledCheckBox>(iControl); }

TitledCheckBox^ TitledCheckBoxList::getControl(UInt32 iControl)
{
	return (iControl < (UInt32)_controls->Count) ?
		_interpretControlAs<TitledCheckBox>(iControl) :
		nullptr;
}

void TitledCheckBoxList::invalidateAll()
{
	for(Int32 i = 0; i < _controls->Count; ++i)
		this[i]->invalidate();
}