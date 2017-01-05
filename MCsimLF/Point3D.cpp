#include "Point3D.h"

using namespace MCsimLF::MC;

Point3D::Point3D(double _x, double _y, double _z)
	: x(_x), y(_y), z(_z)  // the most bored-looking line of code in the entire multiverse!
{
}

double Point3D::getDistanceSquaredTo(Point3D point)
{
	Vector3D delta = Vector3D(x - point.x, y - point.y, z - point.z);

	if(SimSettings::pbcOn)
		delta.applyPbc();

	return delta.getLengthSquared();
}

double Point3D::getDistanceTo(Point3D point)
{
	return Math::Sqrt(getDistanceSquaredTo(point));
}

void Point3D::add(Vector3D vector)
{
	x += vector.x;
	y += vector.y;
	z += vector.z;

	if(SimSettings::pbcOn)
		applyPbc();
}

void Point3D::subtract(Vector3D vector)
{
	x -= vector.x;
	y -= vector.y;
	z -= vector.z;

	if(SimSettings::pbcOn)
		applyPbc();
}

String^ Point3D::ToString()
{
	return gcnew String("(" + x.ToString() + ", " + y.ToString() + ", " + z.ToString() + ")");
}

void Point3D::applyPbc()
{
	double oX = x - SimSettings::boxOriginX;
	if(oX >= SimSettings::boxLengthHalfX) x -= SimSettings::boxLengthX;
	else if(oX < -SimSettings::boxLengthHalfX) x += SimSettings::boxLengthX;

	double oY = y - SimSettings::boxOriginY;
	if(oY >= SimSettings::boxLengthHalfY) y -= SimSettings::boxLengthY;
	else if(oY < -SimSettings::boxLengthHalfY) y += SimSettings::boxLengthY;

	double oZ = z - SimSettings::boxOriginZ;
	if(oZ >= SimSettings::boxLengthHalfZ) z -= SimSettings::boxLengthZ;
	else if(oZ < -SimSettings::boxLengthHalfZ) z += SimSettings::boxLengthZ;
}