#pragma once
#include "test_file.h"
#include "Slist.h"
#include "Algobase.h"
#include "Algo.h"
//#include "profile.h"
namespace Slist_test {
	void slist_test() {
		My_STL::slist<int> slist(3, 1);
		My_STL::for_each(slist.begin(), slist.end(), displat<int>());
		My_STL::slist<int> is{ 1,2,3,4,5,6 };
		cout << endl;
		My_STL::for_each(is.begin(), is.end(), displat<int>());
		cout << endl;
		cout << is.size() << endl;
		cout << is.max_size() << endl;
		cout << *is.begin() << endl;
		cout << is.front() << endl;
		My_STL::slist<int> is1 = is;
		cout << endl;
		My_STL::for_each(is1.begin(), is1.end(), displat<int>());
		is1.pop_front();
		cout << endl;
		My_STL::for_each(is1.begin(), is1.end(), displat<int>());
		is.swap(is1);
		cout << endl;
		My_STL::for_each(is1.begin(), is1.end(), displat<int>());
		//is1.push_front(2);
		is1.insert_after(is1.begin(), 100);
		cout << endl;
		My_STL::for_each(is1.begin(), is1.end(), displat<int>());
		cout << endl;
		cout << *is1.insert(is1.begin(), 2222);
		cout << endl;
		My_STL::for_each(is1.begin(), is1.end(), displat<int>());
		My_STL::slist<int> a{ 1,2,3,4,5 };
		My_STL::slist<int> b{ 6,7,8 };
		a.splice_after(++a.begin(), b.begin(), ++++b.begin());
		cout << endl;
		My_STL::for_each(a.begin(), a.end(),displat<int>());
		cout << endl;
		My_STL::for_each(b.begin(), b.end(), displat<int>());
		b.splice_after(b.begin(), a.begin());
		cout << endl;
		My_STL::for_each(b.begin(), b.end(), displat<int>());
		My_STL::slist<int> c{ 1,2,3,4,5 };
		My_STL::slist<int> d{6};
		d.splice(d.begin(), c,++c.begin(),c.end());
		cout << endl;
		My_STL::for_each(d.begin(), d.end(), displat<int>());
		d.reverse();
		cout << endl;
		My_STL::for_each(d.begin(), d.end(), displat<int>());
		My_STL::slist<int> e{1,1,1,2,2,3};
		e.unique();
		cout << endl;			
		My_STL::for_each(e.begin(), e.end(), displat<int>());
		My_STL::slist<int> f{ 4,5,6};
		f.merge(e);
		cout << endl;
		My_STL::for_each(f.begin(), f.end(), displat<int>());
		My_STL::slist<double> ds{ 4,2,1 };
		ds.sort();
		My_STL::for_each(ds.begin(), ds.end(), displat<int>());

		//My_STL::slist<int> tmp;
		//time_clock::ProfilerInstance::start();
		//for (long int i = 0; i < 100000000;++i)
		//	tmp.push_front(i);
		//time_clock::ProfilerInstance::finish();
		//cout << endl;
		//time_clock::ProfilerInstance::dumpDuringTime(cout);
	}
}
