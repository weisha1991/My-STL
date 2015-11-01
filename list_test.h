#pragma once
#ifndef LIST_TEST_H
#define LIST_TEST_H

#include "Alloc.h"
#include "Allocator.h"
#include "uninitialized.h"
#include <iostream>
#include "ReverseIterator.h"
#include "Functional.h"
#include "Iterator1.h"
#include "Algo.h"
//#include "profile.h"
#include "List.h"
#include "test_file.h"

namespace test_list {
	void list_test() {
		
		My_STL::list<int> ilist;
		cout << ilist.empty() << endl;
		ilist.push_back(1);
		ilist.push_back(2);
		ilist.push_back(3);
		My_STL::for_each(ilist.begin(), ilist.end(), displat<int>());
		cout << endl;
		cout << *ilist.begin() << endl;
		cout << *ilist.end() << endl;
		cout << ilist.back() << endl;
		cout << ilist.front() << endl;
		My_STL::for_each(ilist.rbegin(), ilist.rend(), displat<int>());
		My_STL::list<int> i2{ 1,2,3,4 , };
		cout << endl;
		cout << i2.size() << endl;
		My_STL::for_each(i2.begin(), i2.end(), displat<int>());
		//i2.clear();
		cout << endl;
		cout << i2.size() << endl;
		//i2.pop_back();
		My_STL::list<int> i1{ 1,3,5 };
		i2.merge(i1);
		My_STL::for_each(i2.begin(), i2.end(), displat<int>());
		cout << endl;
		cout << i1.size() << endl;
		i2.push_front(0);
		i2.push_front(-1);
		My_STL::for_each(i2.begin(), i2.end(), displat<int>());
		i2.erase(i2.begin());
		cout << endl;
		My_STL::for_each(i2.begin(), i2.end(), displat<int>());
		i2.pop_front();
		cout << endl;
		My_STL::for_each(i2.begin(), i2.end(), displat<int>());
		i2.reverse();
		cout << endl;
		My_STL::for_each(i2.begin(), i2.end(), displat<int>());
		i2.erase(++++i2.begin(), --i2.end());
		cout << endl;
		My_STL::for_each(i2.begin(), i2.end(), displat<int>());
		i2.insert(i2.end(), 3, 38);
		cout << endl;
		My_STL::for_each(i2.begin(), i2.end(), displat<int>());
		i2.sort();
		cout << endl;
		My_STL::for_each(i2.begin(), i2.end(), displat<int>());
		i2.insert(i2.end(), ilist.begin(), ilist.end());
		cout << endl;
		My_STL::for_each(i2.begin(), i2.end(), displat<int>());
		i2.sort();
		i2.swap(ilist);
		cout << endl;
		My_STL::for_each(i2.begin(), i2.end(), displat<int>());
		cout << endl;
		My_STL::for_each(ilist.begin(), ilist.end(), displat<int>());
		ilist.unique();
		cout << endl;
		My_STL::for_each(ilist.begin(), ilist.end(), displat<int>());
		cout << endl;
		cout << (ilist < i2) << endl;
		ilist.resize(2, 12);
		My_STL::for_each(ilist.begin(), ilist.end(), displat<int>());
	}
}
#endif
