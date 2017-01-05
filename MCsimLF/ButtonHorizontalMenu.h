#pragma once

#include "LabelButton.h"

namespace MCsimLF {
	namespace Helpers {
		namespace UI {
			using namespace System;
			using namespace System::Collections::Generic;
			using namespace System::Drawing;
			using namespace System::Windows::Forms;

			public ref class ButtonHorizontalMenu : public Panel
			{
				public:
					property Int32 Spacing {
						public: Int32 get() { return _spacing; }
						public: void set(Int32 value)
								{
									Int32 delta = value - _spacing;
									_spacing = value;

									for(Int32 i = 1; i < _buttons->Count; ++i)
										_buttons[i]->Left += delta * i;
								}
					}

					property Point StartOffset {
						public: Point get() { return _startOffset; }
						public: void set(Point value)
								{
									Point delta = Point(value.X - _startOffset.X, value.Y - _startOffset.Y);
									_startOffset = value;

									for(Int32 i = 0; i < _buttons->Count; ++i) {
										_buttons[i]->Left += delta.X;
										_buttons[i]->Top += delta.Y;
									}
								}
					}

					property Int32 Count {
						public: Int32 get() { return _buttons->Count; }
					}

					ButtonHorizontalMenu(Control^ parent, String^ name);
					ButtonHorizontalMenu(Control^ parent, String^ name, Int32 spacing, Point startOffset);

					LabelButton^ addButton(String^ name, String^ text);
					void removeButton(UInt32 iButton);

					LabelButton^ operator[] (Int32 iButton);
					LabelButton^ getButton(UInt32 iButton);

				private:
					List<LabelButton^>^ _buttons;
					Int32 _spacing;
					Point _startOffset;
			};
		}
	}
}