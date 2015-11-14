#include <iostream>

#include "raytracing.h"

using namespace std;
using namespace glutils;
using namespace glutils::raytracing;


void main() {
	GLfloat width = 16, height = 9;
	int viewWidth = 64, viewHeight = 36, x;

	vec3 a = vec3(-8, 4, -5);
	vec3 b = vec3(-8, 0, -5);
	vec3 c = vec3(0, 4, -5);

	for(int j = 0; j < viewHeight; j++) {
		for(int i = 0; i < viewWidth; i++) {
			vec3 ray = castRay(5, width, height, viewWidth, viewHeight, i, j);
			vec3 p = intersectTriangle(a, b, c, ray);
			if(p.x != 0.0 || p.y != 0.0 || p.z != 0.0)
				cout << "1";
			else
				cout << "0";
		}
		cout << endl;
	}

	cin >> x;
}