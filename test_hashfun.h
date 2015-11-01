#ifndef TEST_HASHFUN_H
#define TEST_HASHFUN_H
#include "Hashfun.h"
#include "test_file.h"
//#include <string>
namespace test_hashfun{
	void test_hashfun(){
		//std::string s1("long");
		char a[]="long";
		My_STL::hash<char*> hs;
		cout<<"hash code is"<<hs(a)<<endl;
	}
}

#endif