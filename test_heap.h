#pragma once
#include "Heap.h"
#include "test_file.h"
#include "Vector.h"
#include "Algobase.h"
#include "Functional.h"
#include "Algo.h"
namespace test_heap {
	void test_heap() {
	/*	My_STL::vector<int> int_heap{ 1,2,3,4,5,6 };
		My_STL::push_heap(int_heap.begin(), int_heap.end());
		My_STL::for_each(int_heap.begin(), int_heap.end(), displat<int>());
		My_STL::vector<int> i1{ 68,50,65,21,31,32,26,19,16,13,24 };
		My_STL::pop_heap(i1.begin(), i1.end());
		cout << endl;
		My_STL::for_each(i1.begin(), i1.end(), displat<int>());
		My_STL::pop_heap(i1.begin(), i1.end()-1);
		cout << endl;
		My_STL::for_each(i1.begin(), i1.end(), displat<int>());
		My_STL::pop_heap(i1.begin(), i1.end() - 2);
		cout << endl;
		My_STL::for_each(i1.begin(), i1.end(), displat<int>());*/
		My_STL::vector<int> i2{ 68,50,65,21,31,32,26,19,16,13,24 };
		My_STL::for_each(i2.begin(), i2.end(), displat<int>());

		My_STL::sort_heap(i2.begin(), i2.end());
		cout << endl;
		My_STL::for_each(i2.begin(), i2.end(), displat<int>());
		My_STL::make_heap(i2.begin(), i2.end());
		cout << endl;
		My_STL::for_each(i2.begin(), i2.end(), displat<int>());
		My_STL::vector<int> i3{ 68,50,65,21,31,32,26,19,16,13,24 };
		My_STL::sort_heap(i3.begin(), i3.end(), My_STL::less<int>());
		cout << endl;
		My_STL::for_each(i3.begin(), i3.end(), displat<int>());
		My_STL::vector<int> i4{ 1,2,3,4,5,6 };
		My_STL::make_heap(i4.begin(), i4.end(), My_STL::less<int>());
		cout << endl;
		My_STL::for_each(i4.begin(), i4.end(), displat<int>());

	}
}


