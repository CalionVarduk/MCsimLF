#pragma once

#include "TitledVerticalList.h"
#include "TitledTextBox.h"

namespace MCsimLF {
	namespace Helpers {
		namespace UI {
			using namespace System;
			using namespace System::Drawing;
			using namespace System::Windows::Forms;

			ref class TitledTextBoxList : public TitledVerticalList<TextBox>
			{
				public:
					TitledTextBoxList(Control^ parent);
					TitledTextBoxList(Control^ parent, Int32 spacing, Point controlOffset);

					TitledTextBox^ addControl(String^ name, String^ title);

					TitledTextBox^ operator[] (Int32 iControl);
					TitledTextBox^ getControl(UInt32 iControl);
			};
		}
	}
}