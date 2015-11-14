#include <fstream>
#include "binio.h"

using namespace std;
using namespace BinaryIO;

int main() {
	ofstream os;
	os.open("test", ios_base::binary);

	Write<int>(os, 100);
	Write<float>(os, 10.0f);

	os.close();
}