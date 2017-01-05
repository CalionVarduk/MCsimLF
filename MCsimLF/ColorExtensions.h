#pragma once

[System::Runtime::CompilerServices::ExtensionAttribute]
public ref class ColorExtensions abstract sealed
{
	public:
		[System::Runtime::CompilerServices::ExtensionAttribute]
		static System::Drawing::Color GetScaled(System::Drawing::Color color, float scale)
		{
			using namespace System;

			if(scale < 0)
				scale = 0;

			float maxScaleR = (color.R > 0) ? (255.0f / color.R) : 255.0f;
			float maxScaleG = (color.G > 0) ? (255.0f / color.G) : 255.0f;
			float maxScaleB = (color.B > 0) ? (255.0f / color.B) : 255.0f;

			scale = Math::Min(scale, maxScaleR);
			scale = Math::Min(scale, maxScaleG);
			scale = Math::Min(scale, maxScaleB);

			return System::Drawing::Color::FromArgb((Int32)(color.R * scale), (Int32)(color.G * scale), (Int32)(color.B * scale));
		}
};