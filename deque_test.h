#pragma once
#ifndef DEQUE_TEST_H
#define DEQUE_TEST_H
#include "Deque.h"
#include "Alloc.h"
#include "Algobase.h"
#include "Vector.h"
#include "test_file.h"
#include "Algo.h"
// #include "profile.h"
namespace deque_test {
	void deque_test() {
		My_STL::deque<int> dqint(3,1);
		My_STL::vector<int> ivec = { 5,6,7,8 };
		My_STL::for_each(dqint.begin(), dqint.end(), displat<int>());
		cout << endl;
		My_STL::deque<int> dq2(ivec.begin(), ivec.end());
		dq2.push_back(9);
		My_STL::for_each(dq2.begin(), dq2.end(), displat<int>());
		cout << endl;
		dq2.push_front(4);
		My_STL::for_each(dq2.begin(), dq2.end(), displat<int>());
		cout << endl;
		cout << "begin()=" << *dq2.begin() << ", "
			"end()=" << *dq2.end() << "," << "back()=" << dq2.back() << ",front()" << dq2.front() << endl;

		cout << "size()=" << dq2.size() << endl;
		cout << "max_size()=" << dq2.max_size() << endl;
		My_STL::deque<int>::iterator dqiter = dq2.begin();
		cout << "iterator operation test:" << endl;
		cout << "*dqiter=" << *dqiter << endl;
		cout << "dqiter[]=" << dqiter[4] << endl;
		cout << *(dqiter + 3-1) << endl;
		My_STL::deque<double> dqdoule{ 1,2,3,4,5,6 };
		My_STL::for_each(dqdoule.begin(), dqdoule.end(), displat<double>());
		dqdoule.pop_back();
		cout << endl;
		My_STL::for_each(dqdoule.begin(), dqdoule.end(), displat<double>());
		dqdoule.pop_front();
		cout << endl;
		My_STL::for_each(dqdoule.begin(), dqdoule.end(), displat<double>());
		My_STL::deque<int> i1;
		cout << endl;
	/*	time_clock::ProfilerInstance::start();
		const int max = 1000000;
		int i = 0;
		for (; i != max / 2; ++i) {
			i1.push_front(i);
		}
		for (; i != max; ++i) {
			i1.push_back(i);
		}*/
	/*	time_clock::ProfilerInstance::finish();
		time_clock::ProfilerInstance::dumpDuringTime(cout);*/
		cout << *(dqdoule.erase(dqdoule.begin())) << endl;;
		cout << endl;
		My_STL::for_each(dqdoule.begin(), dqdoule.end(), displat<double>());
		cout << endl;
		dqdoule.clear();
		cout << endl;
		My_STL::for_each(dqdoule.begin(), dqdoule.end(), displat<double>());
		cout << dqdoule.size() << endl;
		My_STL::deque<int> tmp = { 1,2,3,4,5 };
		tmp.erase(tmp.begin() + 1, tmp.end() - 1);
		My_STL::for_each(tmp.begin(), tmp.end(), displat<int>());
		tmp.insert(tmp.begin() + 1, 2);
		cout << endl;
		My_STL::for_each(tmp.begin(), tmp.end(), displat<int>());
		tmp.insert(tmp.end(), 3);
		cout << endl;
		My_STL::for_each(tmp.begin(), tmp.end(), displat<int>());


	}
}


#endif
