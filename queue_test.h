#pragma once
#include "Queue.h"
#include "test_file.h"
#include "Algobase.h"
namespace queue_test {
	
	
	void queue_test(){
		My_STL::queue<int> int_que;
		int_que.push(2);
		cout << int_que.size() << endl;
		cout << int_que.front() << endl;
		cout << int_que.back() << endl;
		int_que.pop();
	}

}
