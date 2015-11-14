#pragma once

#include <vector>
#include <algorithm>
#include "vecmath.h"
namespace glutils {
	class LinearInterpolator
	{
	private:

		struct FunctionPoint {
			float T;
			float Value;
			bool operator < (FunctionPoint &other);
			FunctionPoint();
			FunctionPoint(float t, float value);
		};

		FunctionPoint min, max;

		std::vector<FunctionPoint> functionPoints;

	public:

		void AddFunctionPoint(float t, float value);

		float GetValueAt(float t);

		LinearInterpolator(void);
		~LinearInterpolator(void);
	};
}

