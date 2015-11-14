#include "raytracing.h"

namespace glutils {
	namespace raytracing {
		vec3 castRay(float zNear, float width, float height, int viewWidth, int viewHeight, int x, int y) {
			float incrX = width / viewWidth;
			float incrY = height / viewHeight;
			return normalize(vec3(-width / 2 + incrX * x,  height / 2 - incrY * y, -zNear));
		}

		vec3 intersectTriangle(vec3 a, vec3 b, vec3 c, vec3 ray) {

			mat3 A, Ai;

			vec3 solution;

			vec3 betaCoeff = sub(a, b);
			vec3 gammaCoeff = sub(a, c);
			
			A.rows[0] = vec3(ray.x, betaCoeff.x, gammaCoeff.x);
			A.rows[1] = vec3(ray.y, betaCoeff.y, gammaCoeff.y);
			A.rows[2] = vec3(ray.z, betaCoeff.z, gammaCoeff.z);

			float detA = determinant(A);

			for(int i = 0; i < 3; i++) {
				Ai = mat3(A);
				Ai.elements[i] = a.x;
				Ai.elements[i + 3] = a.y;
				Ai.elements[i + 6] = a.z;
				solution.xyz[i] = determinant(Ai) / detA;
			}

			float beta = solution.y;
			float gamma = solution.z;
			float alfa = beta + gamma;

			if(alfa >= 0 && alfa <= 1 &&
				beta >= 0 && beta <= 1 &&
				gamma >= 0 && gamma <= 1)
				return scale(ray, solution.x);
			else
				return vec3();
		}
	}
}