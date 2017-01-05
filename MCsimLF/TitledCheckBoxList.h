#pragma once

#include "TitledVerticalList.h"
#include "TitledCheckBox.h"

namespace MCsimLF {
	namespace Helpers {
		namespace UI {
			using namespace System;
			using namespace System::Drawing;
			using namespace System::Windows::Forms;

			ref class TitledCheckBoxList : public TitledVerticalList<Panel>
			{
				public:
					TitledCheckBoxList(Control^ parent);
					TitledCheckBoxList(Control^ parent, Int32 spacing, Point controlOffset);

					TitledCheckBox^ addControl(String^ name, String^ title);

					TitledCheckBox^ operator[] (Int32 iControl);
					TitledCheckBox^ getControl(UInt32 iControl);

					void invalidateAll();
			};
		}
	}
}