#include "OpenGLView.h"

using namespace MCsimLF;
using namespace MCsimLF::Helpers;
using namespace MCsimLF::OpenGL;

OpenGLView::OpenGLView(Form^ parent, int x, int y, GLsizei width, GLsizei height)
{
	MouseMove = nullptr;
	MouseDown = nullptr;
	MouseUp = nullptr;
	_mouseClicksLeft = _mouseClicksRight = 0;

	_fov = 70;
	_defineSphereDetails(0.25, 19, 15);
	setPosition(0, 0, -30);
	setRotation(0, 0);
	setBackgroundColor(Color::Black);

	_createNewParams(parent->Handle, x, y, width, height);
	_initializeOpenGLView();
}

void OpenGLView::resize(GLsizei width, GLsizei height)
{
	DestroyHandle();
	_createNewParams(_createParams->Parent, _createParams->X, _createParams->Y, width, height);
	_initializeOpenGLView();
}

void OpenGLView::setBackgroundColor(Color color)
{
	_backgroundR = color.R / 255.0f;
	_backgroundG = color.G / 255.0f;
	_backgroundB = color.B / 255.0f;
}

Color OpenGLView::getBackgroundColor()
{
	return Color::FromArgb((Int32)(_backgroundR * 255), (Int32)(_backgroundG * 255), (Int32)(_backgroundB * 255));
}

void OpenGLView::drawScene()
{
	_startDraw();
	_resetTranslationAndRotation();
	_draw();
	_endDraw();
}

Point3D OpenGLView::getPosition()
{ return Point3D(_positionX, _positionY, _positionZ); }

float OpenGLView::getPositionX()
{ return _positionX; }

float OpenGLView::getPositionY()
{ return _positionY; }

float OpenGLView::getPositionZ()
{ return _positionZ; }

void OpenGLView::setPosition(Point3D position)
{
	setPositionX((float)position.x);
	setPositionY((float)position.y);
	setPositionZ((float)position.z);
}

void OpenGLView::setPosition(float x, float y, float z)
{
	setPositionX(x);
	setPositionY(y);
	setPositionZ(z);
}

void OpenGLView::offsetPosition(Vector3D delta)
{
	offsetPositionX((float)delta.x);
	offsetPositionY((float)delta.y);
	offsetPositionZ((float)delta.z);
}

void OpenGLView::offsetPosition(float dx, float dy, float dz)
{
	offsetPositionX(dx);
	offsetPositionY(dy);
	offsetPositionZ(dz);
}

void OpenGLView::setPositionX(float x)
{ _positionX = Limiter<float>::minMaxCheck(x, -1000, 1000); }

void OpenGLView::offsetPositionX(float delta)
{ _positionX = Limiter<float>::minMaxCheck(_positionX + delta, -1000, 1000); }

void OpenGLView::setPositionY(float y)
{ _positionY = Limiter<float>::minMaxCheck(y, -1000, 1000); }

void OpenGLView::offsetPositionY(float delta)
{ _positionY = Limiter<float>::minMaxCheck(_positionY + delta, -1000, 1000); }

void OpenGLView::setPositionZ(float z)
{ _positionZ = Limiter<float>::minMaxCheck(z, -400, 0); }

void OpenGLView::offsetPositionZ(float delta)
{ _positionZ = Limiter<float>::minMaxCheck(_positionZ + delta, -400, 0); }

float OpenGLView::getRotationX()
{ return _rotationX; }

float OpenGLView::getRotationY()
{ return _rotationY; }

void OpenGLView::setRotation(float x, float y)
{
	setRotationX(x);
	setRotationY(y);
}

void OpenGLView::offsetRotation(float dx, float dy)
{
	offsetRotationX(dx);
	offsetRotationY(dy);
}

void OpenGLView::setRotationX(float x)
{
	if(x > 180)
		_rotationX = (float)Math::IEEERemainder(x, 180) - 180;
	else if(x < -180)
		_rotationX = (float)Math::IEEERemainder(x, -180) + 180;
	else
		_rotationX = x;

	_getSinCosRotationX();
}

void OpenGLView::offsetRotationX(float delta)
{
	_rotationX += delta;

	if(_rotationX > 180)
		_rotationX = (float)Math::IEEERemainder(_rotationX, 180) - 180;
	else if(_rotationX < -180)
		_rotationX = (float)Math::IEEERemainder(_rotationX, -180) + 180;

	_getSinCosRotationX();
}

void OpenGLView::setRotationY(float y)
{ _rotationY = Limiter<float>::minMaxCheck(y, -90, 90); }

void OpenGLView::offsetRotationY(float delta)
{ _rotationY = Limiter<float>::minMaxCheck(_rotationY + delta, -90, 90); }

OpenGLView::~OpenGLView()
{ DestroyHandle(); }

void OpenGLView::_draw()
{}

void OpenGLView::_renderSphere(float x, float y, float z, float colorR, float colorG, float colorB)
{
	glPushMatrix();
	glTranslatef(x, y, z);
	glColor3f(colorR, colorG, colorB);
	glCallList(_sphereID);
	glPopMatrix();
}

void OpenGLView::WndProc(Message% m)
{
	switch(m.Msg) {
		case WM_MOUSEMOVE:
			if(MouseMove != nullptr)
				MouseMove->Invoke(this, gcnew MouseEventArgs(MouseButtons::None,
															0,
															GET_X_LPARAM((Int32)m.LParam),
															GET_Y_LPARAM((Int32)m.LParam),
															0));
			break;

		case WM_LBUTTONDOWN:
			if(MouseDown != nullptr)
				MouseDown->Invoke(this, gcnew MouseEventArgs(MouseButtons::Left,
															++_mouseClicksLeft,
															GET_X_LPARAM((Int32)m.LParam),
															GET_Y_LPARAM((Int32)m.LParam),
															0));
			break;

		case WM_LBUTTONUP:
			if(MouseUp != nullptr)
				MouseUp->Invoke(this, gcnew MouseEventArgs(MouseButtons::Left,
															_mouseClicksLeft,
															GET_X_LPARAM((Int32)m.LParam),
															GET_Y_LPARAM((Int32)m.LParam),
															0));
			break;

		case WM_RBUTTONDOWN:
			if(MouseDown != nullptr)
				MouseDown->Invoke(this, gcnew MouseEventArgs(MouseButtons::Right,
															++_mouseClicksRight,
															GET_X_LPARAM((Int32)m.LParam),
															GET_Y_LPARAM((Int32)m.LParam),
															0));
			break;

		case WM_RBUTTONUP:
			if(MouseUp != nullptr)
				MouseUp->Invoke(this, gcnew MouseEventArgs(MouseButtons::Right,
															_mouseClicksRight,
															GET_X_LPARAM((Int32)m.LParam),
															GET_Y_LPARAM((Int32)m.LParam),
															0));
			break;
	}

	NativeWindow::WndProc(m);
}

void OpenGLView::_startDraw()
{
	glClearColor(_backgroundR, _backgroundG, _backgroundB, 0.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
}

void OpenGLView::_resetTranslationAndRotation()
{
	glTranslatef(_positionX, _positionY, _positionZ);
	glRotatef(_rotationX, 0.0f, 1.0f, 0.0f);
	glRotatef(_rotationY, _cosRotX, 0.0f, _sinRotX);
}

void OpenGLView::_endDraw()
{
	glPopMatrix();
	SwapBuffers(_hDC);
}

void OpenGLView::_createNewParams(IntPtr parentHndl, int x, int y, GLsizei width, GLsizei height)
{
	_createParams = gcnew CreateParams();

	_createParams->X = x;
	_createParams->Y = y;
	_createParams->Height = height;
	_createParams->Width = width;

	_createParams->Parent = parentHndl;
	_createParams->Style = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
}

GLint OpenGLView::_setPixelFormat(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pfd = { 
		sizeof(PIXELFORMATDESCRIPTOR),    // size of this pfd 
		1,                                // version number 
		PFD_DRAW_TO_WINDOW |              // support window 
		PFD_SUPPORT_OPENGL |              // support OpenGL 
		PFD_DOUBLEBUFFER,                 // double buffered 
		PFD_TYPE_RGBA,                    // RGBA type 
		24,                               // 24-bit color depth 
		0, 0, 0, 0, 0, 0,                 // color bits ignored 
		0,                                // no alpha buffer 
		0,                                // shift bit ignored 
		0,                                // no accumulation buffer 
		0, 0, 0, 0,                       // accum bits ignored 
		32,                               // 32-bit z-buffer     
		0,                                // no stencil buffer 
		0,                                // no auxiliary buffer 
		PFD_MAIN_PLANE,                   // main layer 
		0,                                // reserved 
		0, 0, 0                           // layer masks ignored 
	}; 
    
	GLint iPixelFormat = ChoosePixelFormat(hdc, &pfd);
     
	if(!iPixelFormat || !SetPixelFormat(hdc, iPixelFormat, &pfd))
		return 0;

	_hglrc = wglCreateContext(_hDC);

	if(!_hglrc || !wglMakeCurrent(_hDC, _hglrc))
		return 0;
    
	return 1;
}

void OpenGLView::_initializeView(double aspect)
{
	float light_position[] = { 1.0f, 1.0f, 1.0f, 0.0f };
	float sourceLight[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	float model_ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f };

	glLightfv(GL_LIGHT0, GL_DIFFUSE, sourceLight);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, model_ambient);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);

	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	_sphereID = glGenLists(1);
	GLUquadric* quadric = gluNewQuadric();

	gluQuadricDrawStyle(quadric, GLU_FILL);
	gluQuadricNormals(quadric, GLU_SMOOTH);
	glNewList(_sphereID, GL_COMPILE);
	gluSphere(quadric, _sphereRadius, _sphereSlices, _sphereStacks);
	glEndList();

	_setupPerspective(aspect);
}

void OpenGLView::_setupPerspective(double aspect)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(_fov, aspect, 0.1, 500.0);
	 
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, 0.0f);
}

void OpenGLView::_getSinCosRotationX()
{
	float _radiansX = _rotationX * 0.01745329252f;
	_cosRotX = (float)Math::Cos(_radiansX);
	_sinRotX = (float)Math::Sin(_radiansX);
}

void OpenGLView::_defineSphereDetails(double radius, Int32 slices, Int32 stacks)
{
	_sphereRadius = (radius > 0.01) ? radius : 0.01;
	_sphereSlices = (slices > 0) ? slices : 1;
	_sphereStacks = (stacks > 0) ? stacks : 1;
}

void OpenGLView::_initializeOpenGLView()
{
	CreateHandle(_createParams);
	_hDC = GetDC((HWND)Handle.ToPointer());

	if(_hDC)
		if(_setPixelFormat(_hDC))
			_initializeView(_createParams->Width / (double)_createParams->Height);
}