#include "ButtonHorizontalMenu.h"

using namespace MCsimLF::Helpers::UI;

ButtonHorizontalMenu::ButtonHorizontalMenu(Control^ parent, String^ name)
	: Panel()
{
	_buttons = gcnew List<LabelButton^>(0);
	Spacing = 0;
	StartOffset = Point(0, 0);
	Name = name;
	parent->Controls->Add(this);
}

ButtonHorizontalMenu::ButtonHorizontalMenu(Control^ parent, String^ name, Int32 spacing, Point startOffset)
	: Panel()
{
	_buttons = gcnew List<LabelButton^>(0);
	Spacing = spacing;
	StartOffset = startOffset;
	Name = name;
	parent->Controls->Add(this);
}

LabelButton^ ButtonHorizontalMenu::addButton(String^ name, String^ text)
{
	LabelButton^ button = gcnew LabelButton(this, name, text);
	button->BorderStyle = System::Windows::Forms::BorderStyle::None;
	button->AutoSize = true;
	_buttons->Add(button);

	if(_buttons->Count == 1)
		button->Location = _startOffset;
	else {
		UInt32 iPrev = _buttons->Count - 2;
		button->Location = Point(_buttons[iPrev]->Right + _spacing, _buttons[iPrev]->Top);
	}

	return button;
}

void ButtonHorizontalMenu::removeButton(UInt32 iButton)
{
	if(iButton < (UInt32)_buttons->Count) {
		Int32 width = _buttons[iButton]->PreferredWidth + _spacing;
		_buttons->RemoveAt(iButton);

		for(Int32 i = iButton + 1; i < _buttons->Count; ++i)
			_buttons[i]->Left -= width;
	}
}

LabelButton^ ButtonHorizontalMenu::operator[] (Int32 iButton)
{ return _buttons[iButton]; }

LabelButton^ ButtonHorizontalMenu::getButton(UInt32 iButton)
{
	return (iButton < (UInt32)_buttons->Count) ?
			_buttons[iButton] :
			nullptr;
}