#include <iostream>
#include "xbo.h"
#include "wfo.h"

using namespace std;
using namespace ExtendedBinary3DObject;
using namespace WaveFront;

int main() {
	WFObject * obj = WFObject::FromFile("models/sonic0.obj");
	WFMaterialLibrary * matlib = WFMaterialLibrary::FromFile("models/sonic.mtl");

	ToFile(obj, matlib, "prova.xbo");
	XBO * xbo = FromFile("prova.xbo");

	cout << "Shading Groups: " << xbo->ShadingGroupCount << endl;

	for(int i = 0; i < xbo->ShadingGroupCount; i++) {
		cout << "Shading group " << i << ": " << xbo->ShadingGroups[i].VerticesCount << " vertices" << endl;
	}

	int x;
	cin >> x;

	delete obj;
	delete xbo;
}