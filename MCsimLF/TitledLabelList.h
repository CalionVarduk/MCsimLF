#pragma once

#include "TitledVerticalList.h"
#include "TitledLabel.h"

namespace MCsimLF {
	namespace Helpers {
		namespace UI {
			using namespace System;
			using namespace System::Drawing;
			using namespace System::Windows::Forms;

			ref class TitledLabelList : public TitledVerticalList<Label>
			{
				public:
					TitledLabelList(Control^ parent);
					TitledLabelList(Control^ parent, Int32 spacing, Point controlOffset);

					TitledLabel^ addControl(String^ name, String^ title);

					TitledLabel^ operator[] (Int32 iControl);
					TitledLabel^ getControl(UInt32 iControl);
			};
		}
	}
}