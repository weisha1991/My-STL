#pragma once
#include "RBTree.h"
#include "test_file.h"
#include "Algobase.h"
#include "Functional.h"
#include "Vector.h"
//#include "profile.h"
namespace rb_tree_test {
	void rb_tree_test() {
		
		My_STL::rb_tree<int, int,My_STL::identity<int>,My_STL::less<int>> rb1;
		cout << "tree elements:"<<rb1.size() << endl;
		/*for (int i = 5; i >= 0; --i)
			rb1.insert_unique(i);*/
		//cout << *rb1.begin() << endl;
		//cout << "tree elements:" << rb1.size() << endl;
		rb1.insert_unique(2);
		rb1.insert_unique(0);
		rb1.insert_unique(4);
		rb1.insert_unique(3);
		rb1.print_tree(cout);
		rb1.insert_equal(++rb1.begin(), 12);
		rb1.print_tree(cout);
		My_STL::vector<int> iq = { 1,2,3,4,5 };
		rb1.insert_equal(iq.begin(), iq.end());
		//cout << "tree elements:" << rb1.size() << endl;
		My_STL::for_each(rb1.begin(), rb1.end(), displat<int>());
		cout << endl;
		rb1.erase(++rb1.begin(),--rb1.end());
		My_STL::for_each(rb1.begin(), rb1.end(), displat<int>());
		cout << endl;
		My_STL::rb_tree<int, int, My_STL::identity<int>, My_STL::less<int>> rb2;
		rb2.insert_equal(1);
		rb2.insert_equal(1);
		rb2.insert_equal(2);
		rb2.insert_equal(2);
		rb2.insert_equal(3);
		rb2.insert_equal(3);
		My_STL::for_each(rb2.begin(), rb2.end(), displat<int>());
		cout << endl;
		
		cout << *rb2.find(3) << endl;
		//rb2.erase(rb2.begin(),rb2.end());
		cout << rb2.size() << endl;
		cout << rb2.count(2) << endl;
		My_STL::for_each(rb2.begin(), rb2.end(), displat<int>());
		cout << endl;
		rb2.insert_equal(3);
		My_STL::for_each(rb2.begin(), rb2.end(), displat<int>());
		cout << endl;
		My_STL::rb_tree<int, int, My_STL::identity<int>, My_STL::less<int>> rb3=rb2;
		//rb3 = rb2;
		My_STL::for_each(rb3.begin(), rb3.end(), displat<int>());
		cout << endl;
		rb3.print_tree(cout);
		cout<<std::boolalpha<<rb3._rb_tree_verify()<<endl;
	}
}
