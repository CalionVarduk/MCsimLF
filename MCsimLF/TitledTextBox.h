#pragma once

#include "TitledControl.h"

namespace MCsimLF {
	namespace Helpers {
		namespace UI {
			using namespace System;
			using namespace System::Windows::Forms;
			using namespace System::Drawing;

			ref class TitledTextBox : public TitledControl<TextBox>
			{
				public:
					property String^ Text {
						public: String^ get() { return _control->Text; }
						public: void set(String^ value) { _control->Text = value; }
					}

					property Int32 MaxLength {
						public: Int32 get() { return _control->MaxLength; }
						public: void set(Int32 value) { _control->MaxLength = value; }
					}

					property bool ReadOnly {
						public: bool get() { return _control->ReadOnly; }
						public: void set(bool value) { _control->ReadOnly = value; }
					}

					property Font^ BoxFont {
						public: Font^ get() { return _control->Font; }
						public: void set(Font^ value) { _control->Font = value; }
					}

					TitledTextBox(Control^ parent, String^ name);
					TitledTextBox(Control^ parent, String^ name, String^ title);

					void addTextChangedEvent(EventHandler^ handler);

				private:
					void _initializeTextBox(String^ name);
			};
		}
	}
}