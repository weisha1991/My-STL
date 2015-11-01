#include <iostream>
#include "list_test.h"
#include "deque_test.h"
#include "Vector.h"
#include "Stack_test.h"
#include "queue_test.h"
#include "test_heap.h"
#include "priorityQueue_test.h"
#include "Slist_test.h"
#include "rb_tree_test.h"
#include "set_test.h"
#include "map_test.h"
#include "algo_test.h"
#include "test_hashfun.h"
#include "hashtable_test.h"
using std::cout;
using std::endl;
using std::ostream;
using std::istream;


int main()
{
	deque_test::deque_test();
	stack_test::stack_test();
	queue_test::queue_test();
	test_heap::test_heap();
	priorityQueue_test::priorityQueue_test();
	Slist_test::slist_test();
	rb_tree_test::rb_tree_test();
	set_test::set_test();
	map_test::map_test();
	algo_test::algo_test();
	test_hashfun::test_hashfun();
	hashtable_tes::hashtable_tes();
}