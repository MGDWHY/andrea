#include "Bspline.h"

namespace glutils {

	Bspline::Bspline(int order, int cpCount, vec3* cps, float *knots) {

		this->order = order;
		this->cpCount = cpCount;

		grade = order - 1;
		totalKnots = cpCount + order;
		internalKnots = cpCount - order;

		this->cps = new vec3[cpCount];
		this->knots = new float[totalKnots];

		for(int i = 0; i < cpCount; i++)
			this->cps[i] = vec3(cps[i]);

		for(int i = 0; i < totalKnots; i++)
			this->knots[i] = knots[i];
	}

	vec3 Bspline::evaluate(float t) {
		vec3 result;
		for(int i = 0; i < cpCount; i++)
			result = add(result, scale(cps[i], deBoor(i, grade, t)));
		
		return result;
	}

	int Bspline::getTotalKnots() { return totalKnots; }
	int Bspline::getInternalKnots() { return internalKnots; }

	int Bspline::getCPCount() { return cpCount; }
	
	int Bspline::getOrder() { return order; }
	int Bspline::getGrade() { return grade;}

	float Bspline::deBoor(int i, int n, float t) {
		if(n == 0) {
			if(knots[i] <= t && knots[i+1] > t)
				return 1.0f;
			else
				return 0.0f;
		} else {
			float den1 = knots[i+n] - knots[i];
			float den2 = knots[i+n+1] - knots[i+1];

			//convenzione x/0 = 0
			float fact1 = den1 != 0.0f ? (t - knots[i]) / den1 : 0.0f; 
			float fact2 = den2 != 0.0f ? (knots[i+n+1] - t) / den2 : 0.0f;

			return fact1 * deBoor(i, n - 1, t) + fact2 * deBoor(i + 1, n - 1, t);
		}
	}
}