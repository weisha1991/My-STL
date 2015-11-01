#ifndef HASHTABLE_TEST_H
#define HASHTABLE_TEST_H

#include "test_file.h"
#include "Hashfun.h"
//#include "Functional.h"
#include "Hashtable.h"

namespace hashtable_tes{
	void hashtable_tes(){
		using My_STL::hashtable;
		using My_STL::hash;
		using My_STL::identity;
		using My_STL::equal_to;
		hashtable<int,int,hash<int>,identity<int>,equal_to<int>> ht(100,hash<int>(),equal_to<int>());
		cout<<ht.size()<<endl;
		cout<<ht.bucket_count()<<endl;
		int a[]={11,12,13,14,15,16};
		for(size_t i=0;i<6;++i)
			ht.insert_unique(a[i]);
		// for(auto iter=ht.begin();iter!=ht.end();++iter)
		// 	cout<<*iter<<endl;
		cout<<ht.size()<<endl;
		//cout<<*ht.find(12)<<endl;
		// auto iter=ht.begin();
		// cout<<*iter<<endl;
		// ht.print_inside(cout);
		// cout<<*++iter<<endl;
		My_STL::for_each(ht.begin(),ht.end(),displat<int>());
		cout<<endl;
		ht.insert_equal(11);
		ht.insert_equal(11);
		My_STL::for_each(ht.begin(),ht.end(),displat<int>());
		cout<<endl;
		auto pii=ht.equal_range(12);
		cout<<*pii.first<<" "<<*(pii.second)<<endl;
		My_STL::vector<int> ivec={21,39,48};
		ht.insert_equal(ivec.begin(),ivec.end());
		My_STL::for_each(ht.begin(),ht.end(),displat<int>());
		cout<<endl;
		My_STL::vector<int> ivec1={11,12,13,14,15};

		ht.insert_unique(ivec1.begin(), ivec1.end() );
		My_STL::for_each(ht.begin(),ht.end(),displat<int>());
		cout<<endl;
		cout<<ht.elems_in_bucket(10)<<endl;
		ht.clear();
		cout<<ht.size()<<endl;
		
	}
}


#endif