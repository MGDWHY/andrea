#pragma once

#include <iostream>
#include <string>

using namespace std;

namespace BinaryIO {
	template <class T> T Read(istream& is) {
		T val;
		is.read((char*)&val, sizeof(T));
		return val;
	}

	template <class T> void Read(istream& is, T * buffer, int howMany) {
		while(howMany-- > 0)
			is.read((char*)buffer++, sizeof(T));
	}

	template <class T> void Write(ostream& os, const T& value) {
		os.write((char*)&value, sizeof(T));
	}

	template <class T> void Write(ostream& os, T * buffer, int howMany) {
		while(howMany-- > 0)
			os.write((char*)buffer++, sizeof(T));
	}
}