#include "BinaryIO.h"

template <class T> T BinaryIO::Read(istream& is) {
	T val;
	is.read((char*)&T, sizeof(T));
}

template <class T> void BinaryIO::Write(ostream& os, const T& value) {
	os.write((char*)&T, sizeof(T));
}