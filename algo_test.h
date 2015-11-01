#ifndef ALGOTEST_H
#define ALGOTEST_H
#include "Algo.h"
#include "Vector.h"
#include "test_file.h"
namespace algo_test{
	void algo_test(){
		My_STL::vector<int> ivec{1,2,3,4,5};
		My_STL::for_each(ivec.begin(),ivec.end(),displat<int>());
		cout<<endl;
		auto iter=My_STL::lower_bound(ivec.begin(),ivec.end(),4);
		cout<<*iter<<endl;

	}
}
#endif