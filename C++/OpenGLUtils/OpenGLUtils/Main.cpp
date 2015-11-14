#include <iostream>
#include <sstream>
#include "Bspline.h"

using namespace glutils;
using namespace std;

int main(int argc, char **argv) {
	Bspline *s = new Bspline(4, 7, NULL, 0, 1);

	for(int i = 0; i < s->totalKnots; i++)
		cout << s->knots[i] << " ";

	cout << "\n";

	int x;

	cin >> x;
}