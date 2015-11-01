//#pragma once
#ifndef QUEUE_H
#define QUEUE_H
#include  "Deque.h"
//#include "Iterator1.h"

namespace My_STL {
	template<class T,class Sequence=My_STL::deque<T>>
	class queue {
		//template<class T,class Sequenece>
		friend bool operator==(const queue &rhs, const queue &lhs) { return rhs.sq == lhs.sq; }
		//template<class T,class Sequence>
		friend bool operator<(const queue &rhs, const queue &lhs) { return rhs.sq < lhs.sq; }
	public:
		typedef typename Sequence::value_type value_type;
		typedef typename Sequence::size_type size_type;
		typedef typename Sequence::reference reference;
		typedef typename Sequence::const_reference const_reference;
	protected:
		Sequence sq;
	public:
		bool empty()const { return sq.empty(); }
		size_type size()const { return sq.size(); }
		reference front() { return sq.front(); }
		const_reference front()const { return sq.front(); }
		reference back() { return sq.back(); }
		const_reference back()const { return sq.back(); }
		void push(const value_type &val) { sq.push_back(val); }
		void pop() { sq.pop_front(); }
	};
}


	
#endif
