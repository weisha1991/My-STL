#pragma once
#ifndef TEST_FILE_H
#define TEST_FILE_H
#include <iostream>
//#include "profile.h"
using std::cout;
using std::endl;
using std::ostream;
using std::istream;

template<typename T>
struct displat {
	void operator()(const T& x)
	{
		std::cout << x << " ";
	}
};
#endif
