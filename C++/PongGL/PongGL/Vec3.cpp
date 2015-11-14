#include "Vec3.h"


Vec3::Vec3()
{
	this->x = 0;
	this->y = 0;
	this->z = 0;
}

Vec3::Vec3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}


Vec3::~Vec3(void)
{
}

Vec3* Vec3::normalise() {
	float m = module();
	return new Vec3(x / m, y / m, z / m);
}

Vec3* Vec3::cross(Vec3* v) {
	float _x, _y, _z;
	_x = y * v->z - v->y * z;
	_y = z * v->x - v->z * x;
	_z = x * v->y - v->x * y;
	return new Vec3(_x, _y, _z);
}