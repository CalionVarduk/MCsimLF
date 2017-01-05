#pragma once

namespace MCsimLF {
	namespace Helpers {
		namespace UI {
			using namespace System;
			using namespace System::Drawing;
			using namespace System::Windows::Forms;

			public delegate void LabelButtonMouseClickHandler(Object^ sender, MouseEventArgs^ e);

			public ref class LabelButton : public Label
			{
				public:
					LabelButtonMouseClickHandler^ MouseClickExtension;

					Color normalBackColor;
					Color normalForeColor;

					Color mouseOverBackColor;
					Color mouseOverForeColor;

					Color activeBackColor;
					Color activeForeColor;

					Color activeMouseOverBackColor;
					Color activeMouseOverForeColor;

					Color disabledBackColor;
					Color disabledForeColor;

				#pragma region Properties
					property bool isEnabled { 
						public: bool get() { return _isEnabled; }
						private: void set(bool value) { _isEnabled = value; }
					}

					property bool isMouseDown { 
						public: bool get() { return _mouseDown; }
						private: void set(bool value) { _mouseDown = value; }
					}

					property bool isMouseMoving { 
						public: bool get() { return _mouseMove; }
						private: void set(bool value) { _mouseMove = value; }
					}

					property bool isMouseWithinBounds { 
						public: bool get() { return _mouseWithinBounds; }
						private: void set(bool value) { _mouseWithinBounds = value; }
					}

					property bool isMouseHovering { 
						public: bool get() { return _mouseHover; }
						private: void set(bool value) { _mouseHover = value; }
					}

					property bool isActivated { 
						public: bool get() { return _isActive; }
						private: void set(bool value) { _isActive = value; }
					}

					property bool isActivatedOnClick { 
						public: bool get() { return _activeOnClick; }
						public: void set(bool value) { _activeOnClick = value; }
					}

					property bool isActivatedOnHold { 
						public: bool get() { return !_activeOnClick; }
						public: void set(bool value) { _activeOnClick = !value; }
					}

					property bool changeColorsOnMouseOver { 
						public: bool get() { return _mouseOverColorChange; }
						public: void set(bool value) { _mouseOverColorChange = value; }
					}

					property bool changeColorsOnActivation { 
						public: bool get() { return _activeColorChange; }
						public: void set(bool value) { _activeColorChange = value; }
					}

					property bool runOnMouseClickProcedure { 
						public: bool get() { return _runOnClickProcedure; }
						public: void set(bool value) { _runOnClickProcedure = value; }
					}

					property bool isOnClickDeactivatable { 
						public: bool get() { return _onClickDeactivate; }
						public: void set(bool value) { _onClickDeactivate = value; }
					}
				#pragma endregion

					LabelButton(Control^ parent, String^ name, String^ text);

					void setFixedSize(System::Drawing::Size size);
					void resetDisplayedColors();

					void forceActivate();
					void forceDeactivate();

					void enable();
					void disable();

				private:
				#pragma region Properties Variables
					bool _mouseDown;
					bool _mouseMove;
					bool _mouseWithinBounds;
					bool _mouseHover;
					bool _isActive;
					bool _activeOnClick;
					bool _mouseOverColorChange;
					bool _activeColorChange;
					bool _runOnClickProcedure;
					bool _onClickDeactivate;
					bool _isEnabled;
				#pragma endregion

					void Event_MouseClick(Object^ sender, MouseEventArgs^ e);
					void Event_MouseDown(Object^ sender, MouseEventArgs^ e);
					void Event_MouseUp(Object^ sender, MouseEventArgs^ e);
					void Event_MouseMove(Object^ sender, MouseEventArgs^ e);
					void Event_MouseEnter(Object^ sender, EventArgs^ e);
					void Event_MouseLeave(Object^ sender, EventArgs^ e);
					void Event_MouseHover(Object^ sender, EventArgs^ e);

					void _activateColorChange();
					void _mouseEnterColorChange();
					void _mouseLeaveColorChange();

					void _initializeLabelButton();
					void _initializeColors();
					void _initializeEvents();
			};
		}
	}
}