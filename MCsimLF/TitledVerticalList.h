#pragma once

#include "TitledControl.h"

namespace MCsimLF {
	namespace Helpers {
		namespace UI {
			using namespace System;
			using namespace System::Drawing;
			using namespace System::Windows::Forms;
			using namespace System::Collections::Generic;

			template <class T>
			ref class TitledVerticalList abstract
			{
				public:
					property Int32 Left {
						public: Int32 get() { return _left; }
					}

					property Int32 Right {
						public: Int32 get() { return _right; }
					}

					property Int32 Top {
						public: Int32 get() { return _top; }
					}

					property Int32 Bottom {
						public: Int32 get() { return _bottom; }
					}

					property Int32 Width {
						public: Int32 get() { return _right - _left; }
					}

					property Int32 Height {
						public: Int32 get() { return _bottom - _top; }
					}

					property System::Drawing::Size Size {
						public: System::Drawing::Size get() { return System::Drawing::Size(Width, Height); }
					}

					property Point Location {
						public: Point get() { return _location; }
						public: void set(Point value)
								{
									Point delta = Point(value.X - _location.X, value.Y - _location.Y);
									_location = value;

									_left += delta.X;
									_right += delta.X;
									_top += delta.Y;
									_bottom += delta.Y;

									for(Int32 i = 0; i < _controls->Count; ++i)
										_controls[i]->Location = Point(_controls[i]->Left + delta.X, _controls[i]->Top + delta.Y);
								}
					}

					property Int32 Spacing {
						public: Int32 get() { return _spacing; }
						public: void set(Int32 value)
								{
									Int32 delta = value - _spacing;
									_spacing = value;

									for(Int32 i = 1; i < _controls->Count; ++i)
										_controls[i]->Location = Point(_controls[i]->Left, _controls[i]->Top + delta * i);

									fixSize();
								}
					}

					property Point ControlOffset {
						public: Point get() { return _controlOffset; }
						public: void set(Point value)
								{
									Point delta = Point(value.X - _controlOffset.X, value.Y - _controlOffset.Y);
									_controlOffset = value;

									for(Int32 i = 0; i < _controls->Count; ++i)
										_controls[i]->Offset = _controlOffset;

									fixSize();
								}
					}

					property Int32 Count {
						public: Int32 get() { return _controls->Count; }
					}

					TitledVerticalList(Control^ parent);
					~TitledVerticalList();

					void removeControl(UInt32 iControl);

					void align();
					void clear();
					void fixSize();

				protected:
					Control^ _parent;
					List<TitledControl<T>^>^ _controls;

					template <class U> U^ _interpretControlAs(UInt32 iControl);
					template <class U> U^ _addControlOfType(String^ name, String^ title);

					void _setOneItemSize();
					void _updateSizeWith(UInt32 iControl);

				private:
					Point _controlOffset;
					Point _location;
					Int32 _spacing;
					Int32 _top;
					Int32 _left;
					Int32 _bottom;
					Int32 _right;
			};

			template <class T>
			TitledVerticalList<T>::TitledVerticalList(Control^ parent)
			{
				_parent = parent;
				_controls = gcnew List<TitledControl<T>^>(0);
				_spacing = 0;
				_location = Point(0, 0);
				_controlOffset = Point(5, 0);
				_left = _right = _top = _bottom = 0;
			}

			template <class T>
			TitledVerticalList<T>::~TitledVerticalList()
			{ clear(); }

			template <class T>
			void TitledVerticalList<T>::removeControl(UInt32 iControl)
			{
				if(iControl < (UInt32)_controls->Count) {
					Int32 height = _controls[iControl]->Height + _spacing;
					_controls[iControl]->removeFromParent();
					_controls->RemoveAt(iControl);

					for(Int32 i = iControl + 1; i < _controls->Count; ++i)
						_controls[i]->Location = Point(_controls[i]->Left, _controls[i]->Top - height);

					fixSize();
				}
			}

			template <class T>
			void TitledVerticalList<T>::align()
			{
				if(_controls->Count > 0) {
					Int32 controlX = _controls[0]->TitleRight;
					for(Int32 i = 1; i < _controls->Count; ++i)
						if(controlX < _controls[i]->TitleRight)
							controlX = _controls[i]->TitleRight;

					controlX += _controlOffset.X;
					for(Int32 i = 0; i < _controls->Count; ++i)
						_controls[i]->Offset = Point(controlX - _controls[i]->TitleRight, _controlOffset.Y);

					fixSize();
				}
			}

			template <class T>
			void TitledVerticalList<T>::clear()
			{
				while(_controls->Count > 0) {
					_controls[0]->removeFromParent();
					_controls->RemoveAt(0);
				}
			}

			template <class T>
			void TitledVerticalList<T>::fixSize()
			{
				if(_controls->Count == 0) {
					_left = _right = _location.X;
					_top = _bottom = _location.Y;
				}
				else {
					_setOneItemSize();
					for(Int32 i = 1; i < _controls->Count; ++i)
						_updateSizeWith(i);
				}
			}

			template <class T> template <class U>
			U^ TitledVerticalList<T>::_interpretControlAs(UInt32 iControl)
			{ return (U^)_controls[iControl]; }

			template <class T> template <class U>
			U^ TitledVerticalList<T>::_addControlOfType(String^ name, String^ title)
			{
				U^ control = gcnew U(_parent, name, title);
				_controls->Add(control);

				if(_controls->Count == 1) {
					control->Location = _location;
					_setOneItemSize();
				}
				else {
					UInt32 iPrev = _controls->Count - 2;
					control->Location = Point(_controls[iPrev]->Left, _controls[iPrev]->Bottom + _spacing);
					_updateSizeWith(_controls->Count - 1);
				}

				return control;
			}

			template <class T>
			void TitledVerticalList<T>::_setOneItemSize()
			{
				_left = _controls[0]->Left;
				_right = _controls[0]->Right;
				_top = _controls[0]->Top;
				_bottom = _controls[0]->Bottom;
			}

			template <class T>
			void TitledVerticalList<T>::_updateSizeWith(UInt32 iControl)
			{
				if(_left > _controls[iControl]->Left)
					_left = _controls[iControl]->Left;
				else if(_right < _controls[iControl]->Right)
					_right = _controls[iControl]->Right;

				if(_top > _controls[iControl]->Top)
					_top = _controls[iControl]->Top;
				else if(_bottom < _controls[iControl]->Bottom)
					_bottom = _controls[iControl]->Bottom;
			}
		}
	}
}