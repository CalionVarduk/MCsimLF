#include "GraphBox.h"

using namespace MCsimLF::Helpers::UI;

GraphBox::GraphBox(Control^ parent, String^ name, UInt32 width, UInt32 height)
{
	_initializeBoxes(name, width, height);
	_initializeGraphics();

	parent->Controls->Add(_labelBoxY);
	parent->Controls->Add(_labelBoxX);
	parent->Controls->Add(_mainBox);

	_spacingX = 100;
	_segmentsY = 8;
	_stepsPerPixel = 4;
	_autoFollow = true;
	_localScaling = false;
	DrawToHalf = true;

	reset();
}

void GraphBox::reset()
{
	_minValue = 0;
	_maxValue = 1;
	_valueDelta = _maxValue - _minValue;
	_viewPoint = 0;
}

void GraphBox::redraw(MonteCarlo^ monteCarlo)
{
	Int32 stepsDrawn = _updateDrawingState(monteCarlo->getStep());
	Int32 startStep = _viewPoint * _stepsPerPixel;

	_determineMinMax(monteCarlo, startStep, stepsDrawn);

	clearCanvas();
	_drawGrid();
	_drawEnergy(monteCarlo, startStep, stepsDrawn);
	refresh();
}

void GraphBox::clearCanvas()
{
	_graphics->FillRectangle(_brushBackground, 0, 0, _mainBox->Width, _mainBox->Height);
	_graphicsX->FillRectangle(_brushBackground, 0, 0, _labelBoxX->Width, _labelBoxX->Height);
	_graphicsY->FillRectangle(_brushBackground, 0, 0, _labelBoxY->Width, _labelBoxY->Height);
}

void GraphBox::refresh()
{
	_mainBox->Refresh();
	_labelBoxX->Refresh();
	_labelBoxY->Refresh();
}

UInt32 GraphBox::getMousedOverStep(Int32 mouseX)
{ return (_viewPoint + mouseX) * _stepsPerPixel; }

void GraphBox::addMouseMoveEvent(MouseEventHandler^ handler)
{ _mainBox->MouseMove += handler; }

Int32 GraphBox::_updateDrawingState(UInt32 stepsElapsed)
{
	Int32 lastPixel = stepsElapsed / _stepsPerPixel;
	if(lastPixel > DrawingLimit + _drawableWidth)
		lastPixel = DrawingLimit + _drawableWidth;

	if(_autoFollow) {
		if((lastPixel - _viewPoint) < 0)
			_viewPoint = 0;
		if((lastPixel - _viewPoint) > _drawableWidth)
			_viewPoint = lastPixel - _drawableWidth;
	}

	Int32 stepsDrawn = lastPixel - _viewPoint + 1;
	return (stepsDrawn > _mainBox->Width) ? _mainBox->Width : stepsDrawn;
}

void GraphBox::_determineMinMax(MonteCarlo^ monteCarlo, Int32 startStep, Int32 stepsDrawn)
{
	if(!__resetMinMax(monteCarlo, startStep, stepsDrawn))
		return;

	for(Int32 i = 0, j = startStep; i < stepsDrawn; ++i, j += _stepsPerPixel) {
		if(_minValue > monteCarlo->getEnergyValueAt(j))
			_minValue = monteCarlo->getEnergyValueAt(j);
		else if(_maxValue < monteCarlo->getEnergyValueAt(j))
			_maxValue = monteCarlo->getEnergyValueAt(j);
	}

	_valueDelta = _maxValue - _minValue;
	if(_valueDelta == 0)
		_valueDelta = _maxValue = monteCarlo->getEnergyValueAt(0);
}

bool GraphBox::__resetMinMax(MonteCarlo^ monteCarlo, Int32 startStep, Int32 stepsDrawn)
{
	if(_localScaling) {
		if(stepsDrawn < 2) {
			_valueDelta = _maxValue = monteCarlo->getEnergyValueAt(0);
			_minValue = 0;
			return false;
		}
		_minValue = _maxValue = monteCarlo->getEnergyValueAt(startStep);
	}
	else {
		_minValue = 0;
		_maxValue = 0;
	}
	return true;
}

void GraphBox::_drawGrid()
{
	_drawGridX();
	_drawGridY();
}

void GraphBox::_drawGridX()
{
	Int32 valueIncrease = _stepsPerPixel * _spacingX;
	Int32 linePos = (((_viewPoint + _spacingX - 1) / _spacingX) * _spacingX) - _viewPoint;
	Int32 lineValue = (_viewPoint + linePos) * _stepsPerPixel;

	float labelY = (float)(_labelBoxX->Height >> 1);

	while(linePos < _mainBox->Width) {
		__drawLineGridX(lineValue.ToString(), linePos, labelY);
		linePos += _spacingX;
		lineValue += valueIncrease;
	}
}

void GraphBox::__drawLineGridX(String^ labelText, Int32 linePos, float labelY)
{
	_graphics->DrawLine(_penGrid, linePos, 0, linePos, _mainBox->Height);
	_graphicsX->DrawString(labelText, _labelFont, _brushFont, linePos + 5.0f, labelY, _labelFormat);
}

void GraphBox::_drawGridY()
{
	bool drawZero;
	Int32 zeroPos = __getZeroPos(drawZero);

	Int32 spacingY = ((zeroPos < (_mainBox->Height >> 1)) ? _mainBox->Height - zeroPos : zeroPos) / _segmentsY;
	double segmentDelta = _valueDelta * (spacingY / (double)_mainBox->Height);

	float labelX = (float)(_labelBoxY->Width >> 1);

	if(drawZero) {
		__drawLineGridY("0.00", zeroPos, labelX, _penZero);
		__drawLinesBelowZero(zeroPos + spacingY, spacingY, -segmentDelta, segmentDelta, labelX);
		__drawLinesAboveZero(zeroPos - spacingY, spacingY, segmentDelta, segmentDelta, labelX);
	}
	else if(_maxValue > 0)
		__drawLinesAboveZero(zeroPos, spacingY, _minValue, segmentDelta, labelX);
	else
		__drawLinesBelowZero(zeroPos, spacingY, _maxValue, segmentDelta, labelX);
}

void GraphBox::__drawLinesAboveZero(Int32 gridPos, Int32 spacingY, double energyValue, double segmentDelta, float labelX)
{
	while(gridPos >= 0) {
		__drawLineGridY(_parseEnergy(energyValue, (energyValue >= 1e6 || energyValue <= 1e-2)), gridPos, labelX, _penGrid);
		gridPos -= spacingY;
		energyValue += segmentDelta;
	}
}

void GraphBox::__drawLinesBelowZero(Int32 gridPos, Int32 spacingY, double energyValue, double segmentDelta, float labelX)
{
	while(gridPos < _mainBox->Height) {
		__drawLineGridY(_parseEnergy(energyValue, (energyValue <= -1e6 || energyValue >= -1e-2)), gridPos, labelX, _penGrid);
		gridPos += spacingY;
		energyValue -= segmentDelta;
	}
}

void GraphBox::__drawLineGridY(String^ labelText, Int32 linePos, float labelX, Pen^ pen)
{
	_graphics->DrawLine(pen, 0, linePos, _mainBox->Width, linePos);
	_graphicsY->DrawString(labelText, _labelFont, _brushFont, labelX, linePos + 5.0f, _labelFormat);
}

Int32 GraphBox::__getZeroPos(bool% out_drawZero)
{
	out_drawZero = ((_maxValue <= _valueDelta) && (-_minValue <= _valueDelta));

	if(out_drawZero) {
		Int32 pos = _flipY(_interpret(0));
		return (pos < 0) ? 0 : pos;
	}
	else if(_maxValue > 0)
		return _mainBox->Height - 1;
	else
		return 0;
}

void GraphBox::_drawEnergy(MonteCarlo^ monteCarlo, Int32 startStep, Int32 stepsDrawn)
{
	if(stepsDrawn > 1) {
		array<Point>^ linePoints = gcnew array<Point>(stepsDrawn);

		for(Int32 i = 0, j = startStep; i < stepsDrawn; ++i, j += _stepsPerPixel)
			linePoints[i] = Point(i, _flipY(_interpret(monteCarlo->getEnergyValueAt(j))));

		_graphics->DrawLines(_penEnergy, linePoints);
	}
}

Int32 GraphBox::_flipY(Int32 y)
{ return _mainBox->Height - y - 1; }

Int32 GraphBox::_interpret(double value)
{ return (Int32)(((value - _minValue) / _valueDelta) * _mainBox->Height); }

String^ GraphBox::_parseEnergy(double energy, bool scientific)
{ return String::Format((scientific) ? "{0:E}" : "{0:0.00}", energy); }

void GraphBox::_initializeBoxes(String^ name, UInt32 width, UInt32 height)
{
	_labelBoxY = gcnew PictureBox();
	_labelBoxY->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
	_labelBoxY->BackColor = Color::White;
	_labelBoxY->Size = System::Drawing::Size(70, height + 10);
	_labelBoxY->Image = gcnew Bitmap(_labelBoxY->Width, _labelBoxY->Height);
	_labelBoxY->Location = Point(0, 0);
	_labelBoxY->Name = name + "_LABELBOXY";

	_labelBoxX = gcnew PictureBox();
	_labelBoxX->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
	_labelBoxX->BackColor = Color::White;
	_labelBoxX->Size = System::Drawing::Size(width + 5, 20);
	_labelBoxX->Image = gcnew Bitmap(_labelBoxX->Width, _labelBoxX->Height);
	_labelBoxX->Location = Point(_labelBoxY->Right, _labelBoxY->Bottom);
	_labelBoxX->Name = name + "_LABELBOXX";

	_mainBox = gcnew PictureBox();
	_mainBox->BorderStyle = System::Windows::Forms::BorderStyle::None;
	_mainBox->BackColor = Color::White;
	_mainBox->Size = System::Drawing::Size(width, height);
	_mainBox->Image = gcnew Bitmap(_mainBox->Width, _mainBox->Height);
	_mainBox->Location = Point(_labelBoxY->Right + 5, _labelBoxY->Top + 5);
	_mainBox->Name = name + "_MAINBOX";
}

void GraphBox::_initializeGraphics()
{
	_graphicsX = Graphics::FromImage(_labelBoxX->Image);
	_graphicsY = Graphics::FromImage(_labelBoxY->Image);
	_graphics = Graphics::FromImage(_mainBox->Image);

	_labelFont = gcnew Font("Microsoft Sans Serif", 5.5f);
	_labelFormat = gcnew StringFormat();
	_labelFormat->Alignment = StringAlignment::Center;
	_labelFormat->LineAlignment = StringAlignment::Center;

	_brushBackground = gcnew SolidBrush(Color::White);
	_brushFont = gcnew SolidBrush(Color::Black);
	_penGrid = gcnew Pen(Color::LightGray);
	_penZero = gcnew Pen(Color::Black);
	_penEnergy = gcnew Pen(Color::Red);
}