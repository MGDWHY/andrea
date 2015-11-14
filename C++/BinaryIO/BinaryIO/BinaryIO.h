#pragma once

#include <iostream>
#include <string>

using namespace std;

class BinaryIO {
public:
	template <class T> static T Read(istream& is);
	template <class T> static void Write(ostream& os, const T& value);
};