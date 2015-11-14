#pragma once

#include "vecmath.h"

using namespace Vecmath;

namespace glutils {
	class Bspline {
	private:
		float *knots;

		vec3 *cps;

		int order, grade, cpCount, totalKnots, internalKnots;
		
		float deBoor(int i, int n, float t);
	public:
		Bspline(int order, int cpCount, vec3* cps, float *knots);

		int getInternalKnots();
		int getTotalKnots();

		int getCPCount();

		int getOrder();
		int getGrade();

		vec3 evaluate(float t);

		
	};
}