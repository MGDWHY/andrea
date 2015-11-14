#pragma once

namespace LibSpline {

	template<class T>
	class Spline {

	private:
		T* cps;
		float* knots;
		float a, b;
		int cpsCount;
		int order;

		float DeBoor(float t, int n, int i) {
			if(n == 0) {
				if(t >= knots[i] && t < knots[i + 1])
					return 1;
				else
					return 0;
			} else {
				float den1 = knots[i + n] - knots[i];
				float den2 = knots[i + n + 1] - knots[i + 1];

				return (den1 == 0.0f ? 0.0f : (t - knots[i]) / den1 * DeBoor(t, n - 1,  i)) +
					(den2 == 0.0f ? 0.0f : (knots[i + n + 1] - t) / den2 * DeBoor(t, n - 1, i + 1));
			}
		}


		int FindInterval(float t) {

			for(int i = 0; i < GetTotalKnots() - 1; i++)
				if( t >= knots[i] && t < knots[i + 1])
					return i;

		}
	
	public:

		Spline(T* controlPoints, int controlPointsCount, int order, float a, float b) {
			this->cpsCount = controlPointsCount;
			this->order = order;
			this->a = a;
			this->b = b;

			this->cps = new T[cpsCount];

			for(int i = 0; i < controlPointsCount; i++)
				this->cps[i] = controlPoints[i];

			this->knots = new float[GetTotalKnots()];
		}

		void BuildOpenUniformPartition() {
			float knotSpacing = (b - a) / (GetInternalKnots() + 1);

			for(int i = 0; i < order; i++) {
				knots[i] = a;
				knots[GetTotalKnots() - i - 1] = b;
			}

			for(int i = order; i < order + GetInternalKnots(); i++) {
				knots[i] = knotSpacing * (i - order + 1) + a;
			}
		}

		void BuildClosedUniformPartition() {
			float knotSpacing = (b - a) / (GetInternalKnots() + 1);

			for(int i = 0; i < GetTotalKnots(); i++) {
				knots[i] = knotSpacing * (i - order + 1) + a;
			}
		}

		T Evaluate(float t) {
			int l = FindInterval(t);

			T result;

			for(int i = l - GetGrade(); i <= l; i++)
				result = result + cps[i] * DeBoor(t, GetGrade(), i);

			return result;
		}

		T& ControlPoints(int index) { return cps[index]; }
		float &Knots(int index) { return knots[index]; }
		int GetTotalKnots() { return order + cpsCount; }
		int GetInternalKnots() { return cpsCount - order; }
		int GetGrade() { return order - 1; }
		int GetOrder() { return order; }
	};

}