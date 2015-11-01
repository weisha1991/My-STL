#pragma once
#ifndef ALGO_H
#define ALGO_H
#include "Iterator1.h"
#include "TypeTraits.h"

namespace My_STL {
	
	template<class InputIterator,class Function>
	Function for_each(InputIterator first, InputIterator last, Function f) {
		for (; first != last; ++first)
			f(*first);
		return f;
	}

	template<class  ForwardIterator,class T,class Distance>
	ForwardIterator _lower_bound(ForwardIterator first,ForwardIterator last,const T &value,
		Distance*,forward_iterator_tag){
		Distance len=0;
		distance(first,last,len);
		Distance half;
		ForwardIterator middle;
		while(len>0){
			half=len>>1;
			middle=first;
			advance(middle,half);
			if(*middle<value){
				first=middle;
				++first;
				len=len-half-1;
			}
			else
				len=half;
		}
		return first;
	}

	template<class RandomAccessIterator,class T,class Distance>
	RandomAccessIterator _lower_bound(RandomAccessIterator first,RandomAccessIterator last,
		const T &value,Distance *,random_access_iterator_tag){
		Distance len=last-first;
		Distance half;
		RandomAccessIterator middle;
		while(len>0){
			half=len>>1;
			middle=first+half;
			if(*middle<value){
				first=middle+1;
				len=len-half-1;
			}
			else
				len=half;
		}
		return first;
	}

	template<class ForwardIterator,class T>
	ForwardIterator lower_bound(ForwardIterator first,ForwardIterator last,const T &val){
		return _lower_bound(first,last,val,distance_type(first),iterator_category(first));
	}

	template<class  ForwardIterator,class T,class Compare,class Distance>
	ForwardIterator _lower_bound(ForwardIterator first,ForwardIterator last,const T &value,Compare comp,
		Distance*,forward_iterator_tag){
		Distance len=0;
		distance(first,last,len);
		Distance half;
		ForwardIterator middle;
		while(len>0){
			half=len>>1;
			middle=first;
			advance(middle,half);
			if(comp(*middle,value)){
				first=middle;
				++first;
				len=len-half-1;
			}
			else
				len=half;
		}
		return first;
	}

	template<class RandomAccessIterator,class T,class Compare,class Distance>
	RandomAccessIterator _lower_bound(RandomAccessIterator first,RandomAccessIterator last,
		const T &value,Compare comp,Distance *,random_access_iterator_tag){
		Distance len=last-first;
		Distance half;
		RandomAccessIterator middle;
		while(len>0){
			half=len>>1;
			middle=first+half;
			if(comp(*middle<value)){
				first=middle+1;
				len=len-half-1;
			}
			else
				len=half;
		}
		return first;
	}

	template<class ForwardIterator,class T,class Compare>
	ForwardIterator lower_bound(ForwardIterator first,ForwardIterator last,const T &val,Compare comp){
		return _lower_bound(first,last,val,comp,distance_type(first),iterator_category(first));
	}

}


#endif
