#pragma once

#include "TextBoxParser.h"
#include "ButtonHorizontalMenu.h"
#include "TitledLabelList.h"
#include "TitledTextBoxList.h"
#include "TitledCheckBoxList.h"

namespace MCsimLF {
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	using namespace MCsimLF::Helpers;
	using namespace MCsimLF::Helpers::UI;

	public ref class SecondaryForm : public System::Windows::Forms::Form
	{
	public:
		property bool isShown {
			public: bool get() { return _isShown; }
			private: void set(bool value) { _isShown = value; }
		}

		SecondaryForm(LabelButton^ _controlButton)
		{
			InitializeComponent();
			controlButton = _controlButton;
			isShown = false;
		}

		void showForm()
		{
			if(isShown) {
				System::Media::SystemSounds::Beep->Play();
				Focus();
			}
			else {
				isShown = true;
				Show();
				Refresh();
				_customInvalidation();
			}
		}

	protected:
		~SecondaryForm()
		{
			if (components)
				delete components;
		}

		virtual void _customInvalidation()
		{}

	private:
		LabelButton^ controlButton;
		bool _isShown;
		System::ComponentModel::Container^ components;

		void Event_FormClosing(Object^ sender, FormClosingEventArgs^ e)
		{
			e->Cancel = true;
			controlButton->forceDeactivate();
			isShown = false;
			Hide();
		}

		void Event_FormMove(Object^ sender, EventArgs^ e)
		{ _customInvalidation(); }

		void Event_FormShown(Object^ sender, EventArgs^ e)
		{
			Refresh();
			_customInvalidation();
		}

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->SuspendLayout();
			// 
			// SecondaryForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(284, 262);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedToolWindow;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"SecondaryForm";
			this->Text = L"SecondaryForm";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &SecondaryForm::Event_FormClosing);
			this->Shown += gcnew System::EventHandler(this, &SecondaryForm::Event_FormShown);
			this->Move += gcnew System::EventHandler(this, &SecondaryForm::Event_FormMove);
			this->ResumeLayout(false);

		}
#pragma endregion
	};
}
