#pragma once

#include "Limiter.h"
#include "TitledTextBox.h"

namespace MCsimLF {
	namespace Helpers {
		using namespace System;
		using namespace System::Windows::Forms;

		using namespace MCsimLF::Helpers::UI;

		ref class TextBoxParser abstract
		{
			public:
				static Int32 tryInt32(TextBox^ box);
				static Int32 tryInt32(TextBox^ box, Int32 min, Int32 max);

				static UInt32 tryUInt32(TextBox^ box);
				static UInt32 tryUInt32(TextBox^ box, UInt32 min, UInt32 max);

				static float tryFloat(TextBox^ box);
				static float tryFloat(TextBox^ box, float min, float max);

				static double tryDouble(TextBox^ box);
				static double tryDouble(TextBox^ box, double min, double max);

				static Int32 tryInt32(TitledTextBox^ box);
				static Int32 tryInt32(TitledTextBox^ box, Int32 min, Int32 max);

				static UInt32 tryUInt32(TitledTextBox^ box);
				static UInt32 tryUInt32(TitledTextBox^ box, UInt32 min, UInt32 max);

				static float tryFloat(TitledTextBox^ box);
				static float tryFloat(TitledTextBox^ box, float min, float max);

				static double tryDouble(TitledTextBox^ box);
				static double tryDouble(TitledTextBox^ box, double min, double max);
		};
	}
}