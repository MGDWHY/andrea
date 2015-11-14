#pragma once

#include "vecmath.h"

using namespace Vecmath;

namespace glutils {
	namespace raytracing {
		vec3 castRay(float zNear, float width, float height, int viewWidth, int viewHeight, int x, int y);
		vec3 intersectTriangle(vec3 a, vec3 b, vec3 c, vec3 ray);
	}
}