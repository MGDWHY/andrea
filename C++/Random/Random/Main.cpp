#include <iostream>
#include "Randomizer.h"

using namespace std;

int main() {
	int x;
	Randomizer::init();
	cout << Randomizer::nextTrigonometricAngle()[0];
	cin>>x;
}