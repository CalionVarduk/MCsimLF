#pragma once

#include <windowsx.h>
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "Point3D.h"
#include "Limiter.h"

namespace MCsimLF {
	namespace OpenGL {
		using namespace System;
		using namespace System::Windows::Forms;
		using namespace System::Drawing;

		using namespace MCsimLF::MC;
		using namespace MCsimLF::Helpers;

		public delegate void OpenGLMouseEventHandler(Object^ sender, MouseEventArgs^ e);

		public ref class OpenGLView : public System::Windows::Forms::NativeWindow
		{
			public:
				OpenGLMouseEventHandler^ MouseMove;
				OpenGLMouseEventHandler^ MouseDown;
				OpenGLMouseEventHandler^ MouseUp;

				OpenGLView(Form^ parent, int x, int y, GLsizei width, GLsizei height);

				void resize(GLsizei width, GLsizei height);

				void setBackgroundColor(Color color);
				Color getBackgroundColor();

				void drawScene();

				Point3D getPosition();
				float getPositionX();
				float getPositionY();
				float getPositionZ();

				void setPosition(Point3D position);
				void setPosition(float x, float y, float z);
				void offsetPosition(Vector3D delta);
				void offsetPosition(float dx, float dy, float dz);

				void setPositionX(float x);
				void offsetPositionX(float delta);
				void setPositionY(float y);
				void offsetPositionY(float delta);
				void setPositionZ(float z);
				void offsetPositionZ(float delta);

				float getRotationX();
				float getRotationY();

				void setRotation(float x, float y);
				void offsetRotation(float dx, float dy);

				void setRotationX(float x);
				void offsetRotationX(float delta);
				void setRotationY(float y);
				void offsetRotationY(float delta);

			protected:
				double _fov;

				float _positionX;
				float _positionY;
				float _positionZ;

				float _rotationX;
				float _rotationY;
				float _rotationZ;

				float _backgroundR;
				float _backgroundG;
				float _backgroundB;

				~OpenGLView();

				virtual void _draw();
				void _renderSphere(float x, float y, float z, float colorR, float colorG, float colorB);

				virtual void WndProc(Message% m) override;

			private:
				CreateParams^ _createParams;
				HDC _hDC;
				HGLRC _hglrc;
				UInt32 _sphereID;

				double _sphereRadius;
				Int32 _sphereSlices;
				Int32 _sphereStacks;

				float _cosRotX, _sinRotX;

				Int32 _mouseClicksLeft;
				Int32 _mouseClicksRight;

				void _startDraw();
				void _resetTranslationAndRotation();
				void _endDraw();

				void _createNewParams(IntPtr parentHndl, int x, int y, GLsizei width, GLsizei height);
				GLint _setPixelFormat(HDC hdc);
				void _initializeView(double aspect);
				void _setupPerspective(double aspect);

				void _getSinCosRotationX();

				void _defineSphereDetails(double radius, Int32 slices, Int32 stacks);
				void _initializeOpenGLView();
		};
	}
}