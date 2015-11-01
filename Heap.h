#pragma once
#ifndef HEAP_H
#define HEAP_H
#include "Iterator1.h"

namespace My_STL {
	template<class RandonAccessIterator,class Distance,class T>
	void _push_heap(RandonAccessIterator first, Distance holeIndex, Distance topIndex, T value) {
		Distance parent = (holeIndex - 1) / 2;
		while (holeIndex > topIndex&&*(first + parent) < value) {
			*(first + holeIndex) = *(first + parent);
			holeIndex = parent;
			parent = (holeIndex - 1) / 2;
		}
		*(first + holeIndex) = value;
	}
	
	template<class RandomAccessIterator,class Distance,class T>
	void _push_heap_aux(RandomAccessIterator first,RandomAccessIterator last,
		Distance *,T*) {
		_push_heap(first, Distance((last - first) - 1), Distance(0), T(*(last - 1)));
	}
	template<class RandomAccessIterator>
	inline void push_heap(RandomAccessIterator first, RandomAccessIterator last) {
		_push_heap_aux(first, last, distance_type(first), value_type(first));
	}

	template <class RandomAccessIterator, class Distance, class T, class Compare>
	void _push_heap(RandomAccessIterator first, Distance holeIndex,
		Distance topIndex, T value, Compare comp) {
		Distance parent = (holeIndex - 1) / 2;
		while (holeIndex > topIndex && comp(*(first + parent), value)) {
			*(first + holeIndex) = *(first + parent);
			holeIndex = parent;
			parent = (holeIndex - 1) / 2;
		}
		*(first + holeIndex) = value;
	}

	template <class RandomAccessIterator, class Compare, class Distance, class T>
	inline void _push_heap_aux(RandomAccessIterator first,
		RandomAccessIterator last, Compare comp,
		Distance*, T*) {
		_push_heap(first, Distance((last - first) - 1), Distance(0),
			T(*(last - 1)), comp);
	}

	template <class RandomAccessIterator, class Compare>
	inline void push_heap(RandomAccessIterator first, RandomAccessIterator last,
		Compare comp) {
		_push_heap_aux(first, last, comp, distance_type(first), value_type(first));
	}
	template<class RandomAccessIterator,class Distance,class T>
	void _adjust_heap(RandomAccessIterator first, Distance holeIndex, Distance len, T value) {
		Distance topIndex = holeIndex;
		Distance second_child = 2 * (holeIndex + 1);
		while (second_child < len) {
			if (*(first + second_child) < *(first + (second_child - 1)))
				--second_child;
			*(first + holeIndex) = *(first + second_child);
			holeIndex = second_child;
			second_child = (holeIndex + 1) * 2;
		}
		if (second_child == len) {
			*(first + holeIndex) = *(first + second_child - 1);
			holeIndex = second_child- 1;
		}
		_push_heap(first, holeIndex, topIndex, value);
	}

	template <class RandomAccessIterator, class T, class Distance>
	inline void _pop_heap(RandomAccessIterator first, RandomAccessIterator last,
		RandomAccessIterator result, T value, Distance*) {
		*result = *first;
		_adjust_heap(first, Distance(0), Distance(last - first), value);
	}

	template <class RandomAccessIterator, class T>
	inline void _pop_heap_aux(RandomAccessIterator first,
		RandomAccessIterator last, T*) {
		_pop_heap(first, last - 1, last - 1, T(*(last - 1)), distance_type(first));
	}

	template <class RandomAccessIterator>
	inline void pop_heap(RandomAccessIterator first, RandomAccessIterator last) {
		_pop_heap_aux(first, last, value_type(first));
	}

	template<class RandomAccessIterator, class Distance, class T,class Compare>
	void _adjust_heap(RandomAccessIterator first, Distance holeIndex
		, Distance len, T value,Compare comp) {
		Distance topIndex = holeIndex;
		Distance second_child = 2 * (holeIndex + 1);
		while (second_child < len) {
			if (comp(*(first + second_child),*(first + (second_child - 1))))
				--second_child;
			*(first + holeIndex) = *(first + second_child);
			holeIndex = second_child;
			second_child = (holeIndex + 1) * 2;
		}
		if (second_child == len) {
			*(first + holeIndex) = *(first + second_child - 1);
			holeIndex = second_child - 1;
		}
		_push_heap(first,holeIndex, topIndex, value,comp);
	}

	template <class RandomAccessIterator, class T, class Distance,class Compare>
	inline void _pop_heap(RandomAccessIterator first, RandomAccessIterator last,
		RandomAccessIterator result, T value, Distance*,Compare comp) {
		*result = *first;
		_adjust_heap(first, Distance(0), Distance(last - first), value,comp);
	}

	template <class RandomAccessIterator, class T,class Compare>
	inline void _pop_heap_aux(RandomAccessIterator first,
		RandomAccessIterator last,Compare comp, T*) {
		_pop_heap(first, last - 1, last - 1, T(*(last - 1)), distance_type(first),comp);
	}

	template <class RandomAccessIterator,class Compare>
	inline void pop_heap(RandomAccessIterator first, RandomAccessIterator last,Compare comp) {
		_pop_heap_aux(first, last,comp,value_type(first));
	}

	template<class RandomAccessIterator>
	void sort_heap(RandomAccessIterator first, RandomAccessIterator last) {
		while (last - first >1)
		{
			pop_heap(first, last);
			--last;
		}
	}

	template<class RandomAccessIterator,class Compare>
	void sort_heap(RandomAccessIterator first, RandomAccessIterator last,
		Compare comp) {
		while (last - first >1)
		{
			pop_heap(first, last,comp);
			--last;
		}
	}

	template<class RandomAccessIterator,class Distance,class T>
	void _make_heap(RandomAccessIterator first, RandomAccessIterator last, T*, Distance*) {
		if (last - first < 2)return;
		Distance len = last - first;
		Distance parent = (len - 2) / 2;
		while (true) {
			_adjust_heap(first, parent, len, T(*(first + parent)));
			if (parent == 0)
				return;
			--parent;
		}
	}

	template<class RandomAccessIterator>
	inline void make_heap(RandomAccessIterator first, RandomAccessIterator last) {
		_make_heap(first, last, value_type(first), distance_type(first));
	}

	template<class RandomAccessIterator, class Distance, class T,class Compare>
	void _make_heap(RandomAccessIterator first, RandomAccessIterator last,Compare comp,
		T*, Distance*) {
		if (last - first < 2)return;
		Distance len = last - first;
		Distance parent = (len - 2) / 2;
		while (true) {
			_adjust_heap(first, parent, len, T(*(first + parent)),comp);
			if (parent == 0)
				return;
			--parent;
		}
	}

	template<class RandomAccessIterator,class Compare>
	inline void make_heap(RandomAccessIterator first, RandomAccessIterator last,Compare comp) {
		_make_heap(first, last,comp, value_type(first), distance_type(first));
	}
}


#endif
