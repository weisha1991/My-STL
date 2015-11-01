#pragma once
#ifndef STACK_H
#define STACK_H
#include "Deque.h"
#include "Iterator1.h"
#include "TypeTraits.h"
#include "TypeTraits.h"
namespace My_STL {

	template<class T,class Sequence=deque<T>>
	class stack;
	template<class T,class Sequence>
	bool operator==(const stack<T,Sequence>&,const stack<T,Sequence>&);
	template<class T,class Sequence>
	bool operator<(const stack<T,Sequence>&,const stack<T,Sequence>&);


	template<class T,class Sequence>
	class stack {
		friend bool operator==<T,Sequence>(const stack<T,Sequence>&,const stack<T,Sequence>&);
		friend bool operator<  <T,Sequence>(const stack<T,Sequence>&,const stack<T,Sequence>&);
	public:
		typedef typename Sequence::value_type value_type;
		typedef typename Sequence::size_type size_type;
		typedef typename Sequence::reference reference;
		typedef typename Sequence::const_reference const_reference;
	protected:
		Sequence c;
	public:
		bool empty()const { return c.empty(); }
		size_type size()const { return c.size(); }
		reference top() { return c.back(); }
		const_reference top()const { return c.back(); }
		void push(const value_type &x) { c.push_back(x); }
		void pop() { c.pop_back(); }
	};

	template<class T,class Sequence>
	bool operator==(const stack<T,Sequence> &lhs,const stack<T,Sequence> &rhs){
		return lhs.c==rhs.c;
	}
	template<class T,class Sequence>
	bool operator<(const stack<T,Sequence> &x,const stack<T,Sequence> &y){
		return x.c<y.c;
	}



}

#endif
