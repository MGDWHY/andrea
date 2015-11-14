#include <iostream>
#include <cmath>

using namespace std;

#define PI 3.141592653589793238

void main() {
	char buf[20];
	int dim = 0;
	double ro = 0.0;

	cout << "Insert dimension: ";
	cin >> dim;
	cout << "Insert ro: ";
	cin >> ro;
	
	int radius = dim / 2;

	double k = 1.0 / (sqrt(2 * PI * pow(ro, 2)));

	for(int i = -radius; i <= radius; i++) {
		double element = k * exp(- pow((double)i, 2) / ( 2 * pow(ro, 2)));
		cout << element << " ";
	}
	cout << endl;
	cin >> ro;

}