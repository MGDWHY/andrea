#include <iostream>
#include "vecmath.h"

using namespace std;
using namespace glutils;

void printMatrix(const mat4 &m);

int main() {

	GLfloat el1[] = { 1, 4, 1, 0, 2, 7, 9, 1, 5, 1, 2, 1, 0, 9, 8, 2};
	GLfloat el2[] = { 0, 2, 2, 1, 6, 3, 2, 1, 2, 0, 2, 1, 2, 3, 2, 2};

	mat4 m1(el1), m2(el2), res;

	res = mult(m1, m2);

	printMatrix(m1);
	cout << "---" << endl;
	printMatrix(m2);
	cout << "---" << endl;
	printMatrix(res);
	res = transpose(res);
	cout << "---" << endl;
	printMatrix(res);

	int x;
	cin >> x;
}

void printMatrix(const mat4 &m) {
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			cout << m.elements[i*4 + j] << " ";
		}
		cout << endl;
	}
}
