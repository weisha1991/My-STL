#pragma once
#include "Stack.h"
#include "test_file.h"
namespace stack_test {
	void stack_test(){
		My_STL::stack<int> int_stack;
		int_stack.push(1);
		cout << int_stack.size() << endl;
		cout << int_stack.top() << endl;
		int_stack.pop();
		cout << int_stack.size() << endl;
	}
}
