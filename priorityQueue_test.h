#pragma once
#include "test_file.h"
#include "Priority_queue.h"

namespace priorityQueue_test {
	using namespace::My_STL;
	void priorityQueue_test() {
		vector<int> ivec = { 1,2,3,4,5 };
		My_STL::priority_queue<int> int_pq(ivec.begin(),ivec.end());
		int_pq.push(6);
		cout << int_pq.top() << endl;
		int_pq.pop();
		cout << int_pq.top() << endl;
		int_pq.pop();
		cout << int_pq.top() << endl;
		int_pq.push(7);
		cout << int_pq.top() << endl;

	}
}
