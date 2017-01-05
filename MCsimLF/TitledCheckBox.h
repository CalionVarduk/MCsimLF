#pragma once

#include "TitledControl.h"

namespace MCsimLF {
	namespace Helpers {
		namespace UI {
			using namespace System;
			using namespace System::Windows::Forms;
			using namespace System::Drawing;

			public delegate void CheckBoxMouseClickHandler(Object^ sender, MouseEventArgs^ e);

			ref class TitledCheckBox : public TitledControl<Panel>
			{
				public:
					CheckBoxMouseClickHandler^ MouseClick;

					property bool isActivated {
						public: bool get() { return _isActivated; }
						private: void set(bool value) { _isActivated = value; }
					}

					TitledCheckBox(Control^ parent, String^ name);
					TitledCheckBox(Control^ parent, String^ name, String^ title);

					void invalidate();
					void forceActivate();
					void forceDeactivate();

				private:
					bool _isActivated;
					SolidBrush^ _brush;

					void _initializePanel(String^ name);

					void Event_MouseClick(Object^ sender, MouseEventArgs^ e);
			};
		}
	}
}