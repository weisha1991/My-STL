#pragma once
#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H
#include "Iterator1.h"
#include "Heap.h"
#include "TypeTraits.h"
#include "Vector.h"
#include "Functional.h"
namespace My_STL {

	template<class T,class Sequence=vector<T>,
		class Compare=less<typename Sequence::value_type >>
		class priority_queue {
		public:
			typedef typename Sequence::value_type value_type;
			typedef typename Sequence::reference reference;
			typedef typename Sequence::const_reference const_reference;
			typedef typename Sequence::size_type size_type;
		protected:
			Sequence sq;
			Compare comp;
		public:
			priority_queue() :sq() { }
			priority_queue(const Compare &c) :sq(), comp(c) {}

			template<class InputIterator>
			priority_queue(InputIterator first, InputIterator last) : sq(first, last) {
				My_STL::make_heap(sq.begin(), sq.end(), comp);
			}
			template<class InputIterator>
			priority_queue(InputIterator first, InputIterator last, const Compare &x) : sq(first, last),
				comp(x) {
				My_STL::make_heap(sq.begin(), sq.end(), comp);
			}

			void push(const value_type &val) {
				try {
					sq.push_back(val);
					push_heap(sq.begin(), sq.end(), comp);
				}
				catch (...) {
					sq.clear();
					throw;
				}
			}
			void pop() {
				try {
					pop_heap(sq.begin(), sq.end(), comp);
					sq.pop_back();
				}
				catch (...) {
					sq.clear();
					throw;
				}
			}
			bool empty()const { return sq.empty(); }
			size_type size()const { return sq.size(); }
			const_reference top()const { return sq.front(); }
		};

}


#endif
