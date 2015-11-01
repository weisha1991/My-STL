#pragma once
#ifndef LIST_H
#define LIST_H
#include "Iterator1.h"
#include <stddef.h>
#include "Alloc.h"
#include "ReverseIterator.h"
#include "Construct.h"
#include "Utility.h"
#include "Algobase.h"
#include "TypeTraits.h"
#include <iostream>
#include <initializer_list>
namespace My_STL {

	template<class T>
	struct _list_node {
		friend std::ostream& operator<<(std::ostream &os, const _list_node &item) {
			os << item.data << " ";
			return os;
		}
		typedef void* void_pointer;
		void_pointer next;
		void_pointer prev;
		T data;
	};

	template<class T,class Ref,class Ptr>
	struct _list_iterator {
	public:
		typedef _list_iterator<T, T&, T*> iterator;
		typedef _list_iterator<T, Ref, Ptr> self;

		typedef bidirectional_iterator_tag iterator_category;
		typedef T value_type;
		typedef Ptr pointer;
		typedef Ref reference;
		typedef _list_node<T>* link_type;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;

		link_type node;

		_list_iterator(link_type x):node(x){}
		_list_iterator(){}
		_list_iterator(const iterator &x):node(x.node){}

		bool operator==(const self &x)const { return node == x.node; }
		bool operator!=(const self &x)const { return node != x.node; }
		reference operator*()const { return (*node).data; }
		pointer operator->()const { return &(operator*()); }
		
		self& operator++() {
			node = (link_type)((*node).next);
			return *this;
		}

		self operator++(int) {
			self tmp = *this;
			++*this;
			return tmp;
		}

		self& operator--() {
			node = (link_type)((*node).prev);
			return *this;
		}

		self operator--(int) {
			self tmp = *this;
			--*this;
			return tmp;
		}
	};


	template<class T,class Alloc>
	class list;
	template<class T, class Alloc>
	 bool operator==(const list<T,Alloc> &,const list<T,Alloc> &);

	template<class T,class Alloc=alloc>
	class list {
	protected:
		typedef void* void_pointer;
		typedef _list_node<T> list_node;
		typedef simple_alloc<list_node, Alloc> list_node_allocator;
	public:
		typedef T value_type;
		typedef value_type* pointer;
		typedef const value_type* const_pointer;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef list_node* link_type;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
	public:
		typedef _list_iterator<T, T&, T*> iterator;
		typedef _list_iterator<T, const T&, const T*> const_iterator;
		typedef reverse_iterator<const_iterator> const_reverse_iterator;
		typedef reverse_iterator<iterator> reverse_iterator;
	protected:
		link_type get_node() { return list_node_allocator::allocate(); }
		void put_node(link_type p) { list_node_allocator::deallocate(p); }

		link_type create_node(const T &x) {
			link_type p = get_node();
			try {
				construct(&p->data, x);//
			}
			catch (...) {				
				put_node(p);
				throw;
			}
			//cout << *p << endl;
			return p;
		}

		void destroy_node(link_type ptr){
			destroy(&ptr->data);
			put_node(ptr);
		}
	protected:
		link_type node;
	protected:
		void empty_initialize() {
			node = get_node();
			node->next = node;
			node->prev = node;
		}

		void fill_initialize(size_type n, const T &x) {
			empty_initialize();
			try {
				insert(begin(), n, x);
			}
			catch (...) {
				clear();
				put_node(node);
				throw;
			}
		}

		template<class InputIterator>
		void range_initialize(InputIterator first, InputIterator last) {
			empty_initialize();
			try {
				insert(begin(), first, last);
			}
			catch (...) {
				clear();
				put_node(node);
				throw;
			}
		}
	public:
		list() { empty_initialize(); }

		iterator begin() { return (link_type)((*node).next); }
		const_iterator begin()const { return (link_type)((*node).next); }
		const_iterator cbegin()const { return (link_type)((*node).next); }
		iterator end() { return node; }
		const_iterator end()const { return node; }
		const_iterator cend()const { return node; }
		reverse_iterator rbegin() { return reverse_iterator(end()); }
		const_reverse_iterator rbegin()const { return const_reverse_iterator(end()); }
		reverse_iterator rend() { return reverse_iterator(begin()); }
		const_reverse_iterator rend()const { return const_reverse_iterator(begin()); }

		bool empty()const { return node->next == node; }
		size_type size() {
			size_type result = 0;
			distance(begin(), end(), result);
			return result;
		}

		size_type max_size()const { return size_type(-1); }
		reference front() { return *begin(); }
		const_reference front()const { return *begin(); }
		reference back() { return *(--end()); }
		const_reference back()const { return *(--end()); }
		void swap(list<T, Alloc> &x) { My_STL::swap(node, x.node); }

		iterator insert(iterator pos, const T &val) {
			link_type tmp = create_node(val);
			//cout << *tmp << endl;
			tmp->next = pos.node;
			tmp->prev = pos.node->prev;
			(link_type(pos.node->prev))->next = tmp;
			pos.node->prev = tmp;
			return tmp;
		}
		iterator insert(iterator pos) { return insert(pos, T());}

		template<class InputIterator>
		void insert(iterator pos, InputIterator first, InputIterator last);

		void insert(iterator pos, size_type n, const T &val);
		void insert(iterator pos, int n, const T &val) {
			insert(pos, (size_type)n, val);
		}
		void insert(iterator pos, long n, const T &val) {
			insert(pos, (size_type)n, val);
		}

		void push_front(const T &val) { insert(begin(), val); }
		void push_back(const T &val) { insert(end(), val); }

		iterator erase(iterator pos) {
			link_type next_node = link_type(pos.node->next);
			link_type prev_node = link_type(pos.node->prev);
			prev_node->next = next_node;
			next_node->prev = prev_node;
	//		cout << "here" << endl;
			destroy_node(pos.node);
		//	cout << "hehe" << endl;
			return iterator(next_node);
		}
		iterator erase(iterator first, iterator last);

		void resize(size_type new_size, const T &val);
		void resize(size_type new_size) { resize(new_size, T()); }
		void clear();
		
		void pop_front() { erase(begin()); }
		void pop_back() {
			assert(node->next == node);
			iterator tmp = end();
			erase(--tmp);
		}

		list(size_type n, const T &val) { fill_initialize(n, val); }
		list(int n, const T &val) { fill_initialize(n, val); }
		list(long n, const T *val) { fill_initialize(n, val); }
		explicit list(size_type n) { fill_initialize(n, T()); }

		template<class InputIterator>
		list(InputIterator first, InputIterator last) { range_initialize(first, last); }
		list(std::initializer_list<value_type> il) {
			range_initialize(il.begin(), il.end());
		}
		list(const list<T, Alloc> &x) { range_initialize(x.begin(), x.end());}

		~list() { clear(); put_node(node); }
		
		list<T, Alloc>& operator=(const list<T, Alloc>&);
		protected:
			//将[first,last)节点接到pos之前。
			void transfer(iterator pos, iterator first, iterator last) {
				if (pos != last) {
					(*(link_type((*last.node).prev))).next = pos.node;
					(*(link_type((*first.node).prev))).next = last.node;
					(*(link_type((*pos.node).prev))).next = first.node;
					link_type tmp = link_type((*pos.node).prev);
					(*pos.node).prev = (*last.node).prev;
					(*last.node).prev = (*first.node).prev;
					(*first.node).prev = tmp;
				}
			}
		public:
			void splice(iterator pos, list &x) {
				if (!x.empty()) {
					transfer(pos, x.begin(), x.end());
				}
			}

			void splice(iterator pos, list &x, iterator i) {
				iterator j = i;
				++j;
				if (pos == j || pos == i)
					return;
				transfer(pos, i, j);
			}

			void splice(iterator pos, list &x, iterator first, iterator last) {
				if (first != last)
					transfer(pos, first, last);
			}
			
			void remove(const T &value);
			void unique();
			void merge(list &x);
			void reverse();
			void sort();

			//template<class Predicate> void remove_if(Predicate);
			//template<class BinaryPredicate> void unique(BinaryPredicate);
			//template<class StrictWeakerOrdering> void merge_if(list &, StrictWeakerOrdering);
			//template<class StrictWeakerOrdering> void sort_if(list&, StrictWeakerOrdering);
			
			friend bool operator==<T,Alloc>(const list<T,Alloc> &,const list<T,Alloc> &);
	};

	template<class T,class Alloc>
	inline bool operator==(const list<T, Alloc> &lhs, const list<T, Alloc> &rhs) {
		typedef typename list<T, Alloc>::link_type link_type;
		link_type end1 = lhs.node;
		link_type end2 = rhs.node;
		link_type begin1 = link_type(end1->next);
		link_type begin2 = link_type(end2->next);
		for (; begin1 != end1, begin2 != end2;begin1=link_type(begin1->next),
			begin2 = link_type(begin2->next)) {
			if(begin1->data!=begin2->data)
				return false;
		}
		return begin1==end1&&begin2==end2;
	}

	template<class T,class Alloc>
	inline bool operator <( const list<T, Alloc> &x, const list<T, Alloc> &y) {
		return lexicographical_compare(x.begin(),x.end(),y.begin(),y.end());
	}

	template<class T,class Alloc>
	inline void swap(list<T,Alloc> &x, list<T,Alloc> &y) {
		x.swap(y);
	}

	template<class T,class Alloc>
	template<class InputIterator>
	void list<T, Alloc>::insert(iterator pos, InputIterator first, InputIterator last) {
		for (; first != last; ++first)
			insert(pos, *first);
	}

	template<class T, class Alloc>
	void list<T, Alloc>::insert(iterator pos, size_type n, const T &val) {
		for (; n>0; --n)
			insert(pos, val);
	}

	template<typename T,typename Alloc>
	typename list<T,Alloc>::iterator 
		list<T, Alloc>::erase(iterator first, iterator last) {
		while (first != last)
			erase(first++);
		return last;
	}

	template<class T,class Alloc>
	void list<T,Alloc>::resize(size_type new_size, const T &val) {
		iterator i = begin();
		size_type len = 0;
		for (; i != end() && len < new_size; ++i, ++len);
		if (len == new_size)
			erase(i, end());
		else
			insert(end(), new_size - len, val);
	}

	template<class T, class Alloc>
	void list<T, Alloc>::clear() {
		link_type cur = (link_type)node->next;
		while (cur != node) {
			link_type tmp = cur;
			cur = (link_type)(cur->next);
			destroy_node(tmp);
		}
		node->next = node;
		node->prev = node;
	}

	template<class T,class Alloc>
	list<T, Alloc>& list<T, Alloc>::operator=(const list<T, Alloc>& rhs) {
		if (this != &rhs) {
			iterator f1 = begin();
			iterator e1 = end();
			const_iterator f2 = rhs.begin();
			const_iterator e2 = rhs.end();
			while (f1 != e1&&f2 != e2)
				*f1++ = *f2++;
			if (f2 == e2)
				erase(f1, e1);
			else
				insert(f1, f2, e2);
		}
		return *this;
	}

	template<class T, class Alloc>
	void list<T, Alloc>::remove(const T &value) {
		iterator first = begin();
		iterator last = end();
		while (first != last) {
			iterator next = first;
			++next;
			if (*first == value)
				erase(first);
			first = next;
		}
	}

	template<class T, class Alloc>
	void list<T, Alloc>::unique() {
		iterator first = begin();
		iterator last = end();
		if (first == last)
			return;
		iterator next = first;
		while (++next != last) {
			if (*next == *first)
				erase(next);
			else
				first = next;
			next = first;
		}
	}

	template<class T, class Alloc>
	void list<T,Alloc>::merge(list &x) {
		iterator first1 = begin();
		iterator last1 = end();
		iterator first2 = x.begin();
		iterator last2 = x.end();
		while (first1 != last1&&first2 != last2) {
			if (*first2 < *first1) {
				iterator next = first2;
				transfer(first1, first2, ++next);
				first2 = next;
			}
			else
				++first1;
		}
		if (first2 != last2)//
			transfer(last1, first2, last2);
	}

	template<class T, class Alloc>
	void list<T,Alloc>::reverse() {
		if (node->next == node || link_type(node->next)->next == node) return;
		iterator first = begin();
		++first;
		while (first != end()) {
			iterator tmp = first;
			++first;
			transfer(begin(), tmp, first);
		}
	}

	template<class T, class Alloc>
	void list<T, Alloc>::sort() {
		if (node->next == node || link_type(node->next)->next == node) return;
		list<T, Alloc> carry;
		list<T, Alloc> counter[64];
		int fill = 0;
		while (!empty()) {
			carry.splice(carry.begin(), *this, begin());
			int i = 0;
			while (i < fill && !counter[i].empty()) {
				counter[i].merge(carry);
				carry.swap(counter[i++]);
			}
			carry.swap(counter[i]);
			if (i == fill) ++fill;
		}
		for (int i = 1; i < fill; ++i) 
			counter[i].merge(counter[i - 1]);
			swap(counter[fill - 1]);
	}
}

#endif
