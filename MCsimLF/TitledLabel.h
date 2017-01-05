#pragma once

#include "TitledControl.h"

namespace MCsimLF {
	namespace Helpers {
		namespace UI {
			using namespace System;
			using namespace System::Windows::Forms;
			using namespace System::Drawing;

			ref class TitledLabel : public TitledControl<Label>
			{
				public:
					property String^ Text {
						public: String^ get() { return _control->Text; }
						public: void set(String^ value) { _control->Text = value; }
					}

					property Font^ LabelFont {
						public: Font^ get() { return _control->Font; }
						public: void set(Font^ value) { _control->Font = value; }
					}

					TitledLabel(Control^ parent, String^ name);
					TitledLabel(Control^ parent, String^ name, String^ title);

				private:
					void _initializeLabel(String^ name);
			};
		}
	}
}