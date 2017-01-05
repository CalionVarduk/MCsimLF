#include "Vector3D.h"

using namespace MCsimLF::MC;

Vector3D::Vector3D(double _x, double _y, double _z)
	: x(_x), y(_y), z(_z)  // they look soooo booored... :p
{
}

double Vector3D::getLengthSquared()
{
	return (x * x) + (y * y) + (z * z);
}

double Vector3D::getLength()
{
	return Math::Sqrt(getLengthSquared());
}

Vector3D Vector3D::getNormalised()
{
	double length = getLength();
	return (length > 0) ?
			Vector3D(x / length, y / length, z / length) :
			Vector3D(0, 0, 0);
}

Vector3D Vector3D::getNegated()
{
	return Vector3D(-x, -y, -z);
}

void Vector3D::applyPbc()
{
	if(x >= SimSettings::boxLengthHalfX) x -= SimSettings::boxLengthX;
	else if(x < -SimSettings::boxLengthHalfX) x += SimSettings::boxLengthX;

	if(y >= SimSettings::boxLengthHalfY) y -= SimSettings::boxLengthY;
	else if(y < -SimSettings::boxLengthHalfY) y += SimSettings::boxLengthY;

	if(z >= SimSettings::boxLengthHalfZ) z -= SimSettings::boxLengthZ;
	else if(z < -SimSettings::boxLengthHalfZ) z += SimSettings::boxLengthZ;
}

String^ Vector3D::ToString()
{
	return gcnew String("(" + x.ToString() + ", " + y.ToString() + ", " + z.ToString() + ")");
}