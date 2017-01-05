#pragma once

#include "MonteCarlo.h"

namespace MCsimLF {
	namespace Helpers {
		namespace UI {
			using namespace System;
			using namespace System::Collections::Generic;
			using namespace System::Drawing;
			using namespace System::Windows::Forms;

			using namespace MCsimLF::MC;

			ref class GraphBox
			{
				public:
					property System::Drawing::Size Size {
						public: System::Drawing::Size get() { return System::Drawing::Size(Width, Height); }
					}

					property Int32 Width {
						public: Int32 get() { return Right - Left; }
					}

					property Int32 Height {
						public: Int32 get() { return Bottom - Top; }
					}

					property Int32 Left {
						public: Int32 get() { return _labelBoxY->Left; }
					}

					property Int32 Right {
						public: Int32 get() { return _mainBox->Right; }
					}

					property Int32 Top {
						public: Int32 get() { return _labelBoxY->Top; }
					}

					property Int32 Bottom {
						public: Int32 get() { return _labelBoxX->Bottom; }
					}

					property Point Location {
						public: Point get() { return _labelBoxY->Location; }
						public: void set(Point value)
								{
									Point delta = Point(value.X - Location.X, value.Y - Location.Y);
									_labelBoxX->Location = Point(_labelBoxX->Left + delta.X, _labelBoxX->Top + delta.Y);
									_labelBoxY->Location = Point(_labelBoxY->Left + delta.X, _labelBoxY->Top + delta.Y);
									_mainBox->Location = Point(_mainBox->Left + delta.X, _mainBox->Top + delta.Y);
								}
					}

					property Color BackgroundColor {
						public: Color get() { return _brushBackground->Color; }
						public: void set(Color value) { _brushBackground->Color = value; }
					}

					property Color LabelFontColor {
						public: Color get() { return _brushFont->Color; }
						public: void set(Color value) { _brushFont->Color = value; }
					}

					property Color GridColor {
						public: Color get() { return _penGrid->Color; }
						public: void set(Color value) { _penGrid->Color = value; }
					}

					property Color ZeroLineColor {
						public: Color get() { return _penZero->Color; }
						public: void set(Color value) { _penZero->Color = value; }
					}

					property Color EnergyColor {
						public: Color get() { return _penEnergy->Color; }
						public: void set(Color value) { _penEnergy->Color = value; }
					}

					property Int32 DrawingLimit {
						public: Int32 get() { return 1000000000; }
					}

					property Int32 StepsPerPixel {
						public: Int32 get() { return _stepsPerPixel; }
						public: void set(Int32 value)
								{
									_viewPoint *= _stepsPerPixel;
									_stepsPerPixel = (value > 0) ? value : 1;
									_viewPoint /= _stepsPerPixel;
								}
					}

					property Int32 ViewPoint {
						public: Int32 get() { return _viewPoint; }
						public: void set(Int32 value) { _viewPoint = (value > 0) ? ((value < DrawingLimit) ? value : DrawingLimit) : 0; }
					}

					property Int32 HorizontalSpacing {
						public: Int32 get() { return _spacingX; }
						public: void set(Int32 value) { _spacingX = (value > 0) ? value : 1; }
					}

					property Int32 VerticalSegments {
						public: Int32 get() { return _segmentsY; }
						public: void set(Int32 value) { _segmentsY = (value > 0) ? value : 1; }
					}

					property bool AutoFollow {
						public: bool get() { return _autoFollow; }
						public: void set(bool value) { _autoFollow = value; }
					}

					property bool LocalScaling {
						public: bool get() { return _localScaling; }
						public: void set(bool value) { _localScaling = value; }
					}

					property bool DrawToHalf {
						public: bool get() { return (_drawableWidth != _mainBox->Width); }
						public: void set(bool value) { _drawableWidth = (value) ? _mainBox->Width >> 1 : _mainBox->Width; }
					}

					GraphBox(Control^ parent, String^ name, UInt32 width, UInt32 height);

					void reset();

					void redraw(MonteCarlo^ monteCarlo);
					void clearCanvas();
					void refresh();

					UInt32 getMousedOverStep(Int32 mouseX);
					void addMouseMoveEvent(MouseEventHandler^ handler);

				private:
					Graphics^ _graphics;
					Graphics^ _graphicsX;
					Graphics^ _graphicsY;
					PictureBox^ _mainBox;
					PictureBox^ _labelBoxX;
					PictureBox^ _labelBoxY;
					Font^ _labelFont;
					StringFormat^ _labelFormat;

					double _minValue, _maxValue, _valueDelta;
					bool _autoFollow;
					bool _localScaling;
					Int32 _viewPoint;
					Int32 _spacingX, _segmentsY;
					Int32 _stepsPerPixel;
					Int32 _drawableWidth;

					SolidBrush^ _brushBackground;
					SolidBrush^ _brushFont;
					Pen^ _penGrid;
					Pen^ _penZero;
					Pen^ _penEnergy;

					Int32 _updateDrawingState(UInt32 stepsElapsed);
					void _determineMinMax(MonteCarlo^ monteCarlo, Int32 startStep, Int32 stepsDrawn);
					bool __resetMinMax(MonteCarlo^ monteCarlo, Int32 startStep, Int32 stepsDrawn);

					void _drawGrid();
					void _drawGridX();
					void __drawLineGridX(String^ labelText, Int32 linePos, float labelY);
					void _drawGridY();
					void __drawLinesAboveZero(Int32 gridPos, Int32 spacingY, double energyValue, double segmentDelta, float labelX);
					void __drawLinesBelowZero(Int32 gridPos, Int32 spacingY, double energyValue, double segmentDelta, float labelX);
					void __drawLineGridY(String^ labelText, Int32 linePos, float labelX, Pen^ pen);
					Int32 __getZeroPos(bool% out_drawZero);

					void _drawEnergy(MonteCarlo^ monteCarlo, Int32 startStep, Int32 stepsDrawn);

					Int32 _flipY(Int32 y);
					Int32 _interpret(double value);
					String^ _parseEnergy(double energy, bool scientific);

					void _initializeBoxes(String^ name, UInt32 width, UInt32 height);
					void _initializeGraphics();
			};
		}
	}
}