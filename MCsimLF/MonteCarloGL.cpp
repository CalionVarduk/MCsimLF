#include "MonteCarloGL.h"

using namespace MCsimLF::MC;
using namespace MCsimLF::OpenGL;

MonteCarloGL::MonteCarloGL(Form^ parent, int x, int y, GLsizei width, GLsizei height, MonteCarlo^ monteCarlo)
	: OpenGLView(parent, x, y, width, height)
{
	_monteCarlo = monteCarlo;
	_atomCount = 0;
	_origin = Point3D();
	_boxDiagonalPartial = 0;
	_sphereFarDistanceRatio = 0.8f;

	setSphereNearColor(Color::White);
	setSphereFarColor(Color::Black);
	setPositionMouseDeltas(0, 0, 0);
	setRotationMouseDeltas(0, 0);

	_mouseDownLeft = _mouseDownRight = false;
	_prevMouseLeft = _prevMouseRight = Point(0, 0);
	_mouseLeft = _mouseRight = Point(0, 0);

	_initializeEvents(parent);
}

void MonteCarloGL::updateSimulation()
{
	_atomCount = _monteCarlo->getAtomCount();
	_origin = _monteCarlo->getOrigin();
	Vector3D boxSize = _monteCarlo->getInitialBoxSize();
	_boxDiagonalPartial = (float)Point3D(boxSize.x, boxSize.y, boxSize.z).getDistanceSquaredTo(Point3D()) * 0.25f;
	_boxDiagonalPartial *= _sphereFarDistanceRatio;
}

inline void MonteCarloGL::setSphereNearColor(Color color)
{
	_sphereNearR = color.R / 255.0f;
	_sphereNearG = color.G / 255.0f;
	_sphereNearB = color.B / 255.0f;
}

inline void MonteCarloGL::setSphereFarColor(Color color)
{
	_sphereFarR = color.R / 255.0f;
	_sphereFarG = color.G / 255.0f;
	_sphereFarB = color.B / 255.0f;
}

inline Color MonteCarloGL::getSphereNearColor()
{
	return Color::FromArgb((Int32)(_sphereNearR * 255), (Int32)(_sphereNearG * 255), (Int32)(_sphereNearB * 255));
}

inline Color MonteCarloGL::getSphereFarColor()
{
	return Color::FromArgb((Int32)(_sphereFarR * 255), (Int32)(_sphereFarG * 255), (Int32)(_sphereFarB * 255));
}

inline void MonteCarloGL::setPositionMouseDeltas(float dx, float dy, float dz)
{
	_posMDX = dx;
	_posMDY = dy;
	_posMDZ = dz;
}

inline void MonteCarloGL::setRotationMouseDeltas(float dx, float dy)
{
	_rotMDX = dx;
	_rotMDY = dy;
}

void MonteCarloGL::_draw()
{
	for(UInt32 i = 0; i < _atomCount; ++i)
		_drawAtom(i);
}

void MonteCarloGL::Event_MouseMove(Object^ sender, MouseEventArgs^ e)
{
	if(_mouseDownLeft) {
		_prevMouseLeft = _mouseLeft;
		_mouseLeft = e->Location;

		offsetPositionX((_mouseLeft.X - _prevMouseLeft.X) * _posMDX);
		offsetPositionY((_prevMouseLeft.Y - _mouseLeft.Y) * _posMDY);
	}

	if(_mouseDownRight) {
		_prevMouseRight = _mouseRight;
		_mouseRight = e->Location;

		offsetRotationX((_mouseRight.X - _prevMouseRight.X) * _rotMDX);
		offsetRotationY((_mouseRight.Y - _prevMouseRight.Y) * _rotMDY);
	}
}

void MonteCarloGL::Event_MouseDown(Object^ sender, MouseEventArgs^ e)
{
	if(e->Button == MouseButtons::Left) {
		_mouseDownLeft = true;
		_mouseLeft = e->Location;
	}
	else {
		_mouseDownRight = true;
		_mouseRight = e->Location;
	}
}

void MonteCarloGL::Event_MouseUp(Object^ sender, MouseEventArgs^ e)
{
	((e->Button == MouseButtons::Left) ?		// black magic! well, not really.
		_mouseDownLeft :
		_mouseDownRight) = false;
}

void MonteCarloGL::Event_MouseWheel(Object^ sender, MouseEventArgs^ e)
{ offsetPositionZ(e->Delta * _posMDZ); }

inline void MonteCarloGL::_drawAtom(UInt32 iAtom)
{
	Point3D atom = _monteCarlo->getAtom(iAtom);
	float r, g, b;
	_getAtomColor(r, g, b, (float)atom.getDistanceSquaredTo(_origin));
	_renderSphere((float)atom.x, (float)atom.y, (float)atom.z, r, g, b);
}

inline void MonteCarloGL::_getAtomColor(float% out_r, float% out_g, float% out_b, float atomDistance)
{
	float distanceRatio = atomDistance / _boxDiagonalPartial;
	if(distanceRatio > 1)
		distanceRatio = 1.0f;

	out_r = _sphereNearR - (_sphereNearR - _sphereFarR) * distanceRatio;
	out_g = _sphereNearG - (_sphereNearG - _sphereFarG) * distanceRatio;
	out_b = _sphereNearB - (_sphereNearB - _sphereFarB) * distanceRatio;
}

inline void MonteCarloGL::_initializeEvents(Form^ parent)
{
	MouseMove += gcnew OpenGLMouseEventHandler(this, &MonteCarloGL::Event_MouseMove);
	MouseDown += gcnew OpenGLMouseEventHandler(this, &MonteCarloGL::Event_MouseDown);
	MouseUp += gcnew OpenGLMouseEventHandler(this, &MonteCarloGL::Event_MouseUp);
	parent->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &MonteCarloGL::Event_MouseWheel);
}