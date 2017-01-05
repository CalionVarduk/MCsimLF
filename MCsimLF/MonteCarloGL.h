#pragma once

#include "MonteCarlo.h"
#include "OpenGLView.h"

namespace MCsimLF {
	namespace OpenGL {
		using namespace System::Windows::Forms;
		using namespace System::Drawing;

		using namespace MCsimLF::MC;

		public ref class MonteCarloGL : public OpenGLView
		{
			public:
				MonteCarloGL(Form^ parent, int x, int y, GLsizei width, GLsizei height, MonteCarlo^ monteCarlo);

				void updateSimulation();

				void setSphereNearColor(Color color);
				void setSphereFarColor(Color color);
				Color getSphereNearColor();
				Color getSphereFarColor();

				void setPositionMouseDeltas(float dx, float dy, float dz);
				void setRotationMouseDeltas(float dx, float dy);

			protected:
				virtual void _draw() override;

				void Event_MouseMove(Object^ sender, MouseEventArgs^ e);
				void Event_MouseDown(Object^ sender, MouseEventArgs^ e);
				void Event_MouseUp(Object^ sender, MouseEventArgs^ e);
				void Event_MouseWheel(Object^ sender, MouseEventArgs^ e);

			private:
				MonteCarlo^ _monteCarlo;
				UInt32 _atomCount;
				Point3D _origin;
				float _boxDiagonalPartial;
				float _sphereFarDistanceRatio;

				float _sphereNearR;
				float _sphereNearG;
				float _sphereNearB;
				float _sphereFarR;
				float _sphereFarG;
				float _sphereFarB;

				float _posMDX;
				float _posMDY;
				float _posMDZ;
				float _rotMDX;
				float _rotMDY;

				bool _mouseDownLeft;
				bool _mouseDownRight;
				Point _prevMouseLeft;
				Point _prevMouseRight;
				Point _mouseLeft;
				Point _mouseRight;

				void _drawAtom(UInt32 iAtom);
				void _getAtomColor(float% out_r, float% out_g, float% out_b, float atomDistance);

				void _initializeEvents(Form^ parent);
		};
	}
}