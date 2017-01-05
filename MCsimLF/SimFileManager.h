#pragma once

#include "HuffmanCompression.h"
#include "CVAwfulCipher.h"

namespace MCsimLF {
	namespace FileManip {
		using namespace System;
		using namespace System::IO;

		public ref class SimFileManager
		{
			public:
				SimFileManager();

				bool save(String^ path, array<Byte>^ monteCarlo);
				array<Byte>^ load(String^ path);

			private:
				CVAwfulCipher^ _cipher;

				bool _isSignatureCorrect(array<Byte>^ toCheck);
				static array<Byte>^ _getSignature();
				static array<Byte>^ _initKey();
		};
	}
}