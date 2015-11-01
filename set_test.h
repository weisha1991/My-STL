#pragma once
#include "Set.h"
#include "test_file.h"
namespace set_test {
	void set_test() {
		My_STL::set<int> int_set;
		int a[] = { 0,1,2,3,4,5 };
		int_set.insert(a,a+6);
		cout << "size=" << int_set.size() << endl;
		int_set.insert(4);
		cout << int_set.size() << endl;
		int_set.erase(1);
		My_STL::for_each(int_set.begin(), int_set.end(), displat<int>());
		cout << endl;
		cout<<int_set.count(2)<<endl;
		My_STL::set<int> s2 = int_set;
		My_STL::for_each(s2.begin(), s2.end(), displat<int>());
		cout << endl;
		s2 = int_set;
		cout << *s2.find(5) << endl;
		My_STL::for_each(s2.rbegin(), s2.rend(), displat<int>());
		cout << endl;

	}
}
