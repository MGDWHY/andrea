#pragma once

#include <cmath>

class Vec3
{
private:
	float x, y, z;
public:

	float getX() { return x;}
	float getY() { return y;}
	float getZ() { return z;}

	void set(float x, float y, float z) { this->x = x; this->y = y; this->z = z; }
	void set(Vec3* v) { this->x = v->x; this->y = v->y; this->z = v->z; }

	float module() { return sqrt(pow(x,2) + pow(y,2) + pow(z,2)); }

	Vec3* add(Vec3 *v) { return new Vec3(x + v->getX(), y + v->getY(), z + v->getZ()); }
	Vec3* sub(Vec3 *v) { return new Vec3(x - v->getX(), y - v->getY(), z - v->getZ()); }

	Vec3* scale(float f) { return new Vec3(x*f, y*f, z*f); };
	Vec3* cross(Vec3 *v);

	Vec3* normalise();

	Vec3();
	Vec3(float x, float y, float z);
	~Vec3(void);
};

