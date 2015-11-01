#pragma once
#ifndef SLIST_H
#define SLIST_H
#include "Iterator1.h"
#include <stddef.h>
#include "Alloc.h"
#include "Construct.h"
#include <initializer_list>
namespace My_STL {
	struct _slist_node_base {
		_slist_node_base *next;
	};

	//将new_node指向的node插入到previos的后面；
	inline _slist_node_base* _slist_make_link(_slist_node_base *previous_node,
		_slist_node_base *new_node) {
		new_node->next = previous_node->next;
		previous_node->next = new_node;
		return new_node;
	}

	//返回一个指向node之前的指针；
	inline _slist_node_base* _slist_previous(_slist_node_base *head,const _slist_node_base *node) {
		while (head&&head->next != node) {
			head = head->next;
		}
		return head;
	}

	inline const _slist_node_base* _slist_previous(const _slist_node_base *head,
		const _slist_node_base *node) {
		while (head&&head->next != node)
			head = head->next;
		return head;
	}
	/*
	把(before_first before_last]插入到pos后面
	(before_first before_last]在原有链表上移除
	*/
	inline void _slist_splice_after(_slist_node_base *pos, _slist_node_base *before_first,
		_slist_node_base *before_last) {
		if (pos != before_first&&pos != before_last) {
			_slist_node_base *first = before_first->next;
			_slist_node_base *after = pos->next;
			before_first->next = before_last->next;
			pos->next = first;
			before_last->next = after;
		}
	}
	/*
		从node开始（包括node）的节点反序；node成为尾节点；
		返回的result为头节点；
	*/
	inline _slist_node_base* _slist_reverse(_slist_node_base *node) {
		_slist_node_base *result = node;
		node = node->next;
		result->next = 0;
		while (node) {
			_slist_node_base *next = node->next;
			node->next = result;
			result = node;
			node = next;
		}
		return result;
	}

	template<class T>
	struct _slist_node :public _slist_node_base {
		T data;
	};

	struct _slist_iterator_base {
		typedef size_t size_type;
		typedef ptrdiff_t  difference_type;
		typedef forward_iterator_tag iterator_category;

		_slist_node_base *node;
		
		_slist_iterator_base(_slist_node_base *c):node(c){}
		void incr() { node = node->next; }
		bool operator==(const _slist_iterator_base &rhs)const {
			return node == rhs.node;
		}
		bool operator!=(const _slist_iterator_base &rhs) const{
			return node != rhs.node;
		}
	};

	template<class T,class Ref,class Ptr>
	struct _slist_iterator :public _slist_iterator_base {
		typedef _slist_iterator<T, T&, T*> iterator;
		typedef _slist_iterator<T, const T&, const T*> const_iterator;
		typedef _slist_iterator<T, Ref, Ptr> self;
		typedef T value_type;
		typedef Ptr pointer;
		typedef _slist_node<T> list_node;
		typedef Ref reference;

		_slist_iterator(list_node *x):_slist_iterator_base(x){}
		_slist_iterator():_slist_iterator_base(0){}
		_slist_iterator(const iterator &x):_slist_iterator_base(x.node){}

		self& operator++() {
			incr();
			return *this;
		}

		self operator++(int) {
			self tmp = *this;
			incr();
			return tmp;
		}

		reference operator*() {
			return (((list_node*)node)->data);
		}

		pointer operator&() {
			return &(operator*());
		}
	};

	inline size_t _slist_size(_slist_node_base *node) {
		size_t result = 0;
		for (; node != 0; node = node->next)
			++result;
		return result;
	}

	template<class T,class Alloc=alloc>
	class slist {
	public:
		typedef T value_type;
		typedef T* pointer;
		typedef const T* const_pointer;
		typedef T& reference;
		typedef const T& const_reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef _slist_iterator<T, T&, T*> iterator;
		typedef _slist_iterator<T, const T&, const T*> const_iterator;
	private:
		typedef _slist_node<T> list_node;
		typedef _slist_node_base list_node_base;
		typedef _slist_iterator_base iterator_base;
		typedef simple_alloc<list_node, Alloc> list_node_allocator;

		static list_node* create_node(const value_type &val) {
			list_node *node = list_node_allocator::allocate();
			try {
				construct(&node->data, val);
				node->next = 0;
			}
			catch (...) {
				list_node_allocator::deallocate(node);
				throw;
			}
			return node;
		}

		static void destroy_node(list_node *ptr) {
			destroy(&ptr->data);
			list_node_allocator::deallocate(ptr);
		}

		void fill_initialize(size_type n, const value_type &val) {
			head.next = 0;
			try {
				_insert_after_fill(&head,n , val);
			}
			catch (...) {
				clear();
				throw;
			}
		}
		template<class InputIterator>
		void range_initialize(InputIterator first, InputIterator last) {
			head.next = 0;
			try {
				_insert_range_after(&head, first, last);
			}
			catch (...) {
				clear(); throw;
			}
		}
	public:
		slist() { head.next = 0; }
		slist(size_type n, const value_type &val) { fill_initialize(n, val); }
		slist(int n, const value_type &val) { fill_initialize(n, val); }
		slist(long n, const value_type &val) { fill_initialize(n, val); }
		
		template<class InputIterator>
		slist(InputIterator first, InputIterator last) { range_initialize(first, last); }
		slist(std::initializer_list<value_type>il) { range_initialize(il.begin(), il.end()); }
		slist(const slist&rhs) { range_initialize(rhs.begin(), rhs.end()); }
		slist& operator=(const slist&);
		~slist() { clear(); }
	private:
		list_node_base head;
	public:
		iterator begin() { return iterator((list_node*)head.next); }
		const_iterator begin() const{ return const_iterator((list_node*)head.next); }
		const_iterator cbegin() const { return const_iterator((list_node*)head.next); }
		iterator end() { return iterator(0); }
		const_iterator end()const { return const_iterator(0); }
		const_iterator cend()const { return const_iterator(0); }
		size_type size()const { return _slist_size(head.next); }
		size_type max_size()const { return size_type(-1); }
		bool empty()const { return head.next == 0; }
		void swap(slist &rhs) {
			list_node_base *tmp = head.next;
			head.next = rhs.head.next;
			rhs.head.next = tmp;
		}
	public:
		reference front() { return ((list_node*)head.next)->data; }
		const_reference front() const{ return ((list_node*)head.next)->data; }
		void push_front(const value_type &val) {
			_slist_make_link(&head, create_node(val));
		}
		void pop_front() {
			list_node *next = (list_node*)head.next;
			head.next = next->next;
			destroy_node(next);
		}

	private:
		//insert new node after the pos;
		list_node* _insert_after(list_node_base *pos, const value_type &val) {
			return (list_node*)(_slist_make_link(pos, create_node(val)));
		}
		//insert n vals after the pos``ition pos;
		void _insert_after_fill(list_node_base *pos, size_type n, const value_type &val) {
			for (size_type i = 0; i < n; ++i)
				pos = _slist_make_link(pos, create_node(val));
		}

		template<class InputIterator>
		void _insert_range_after(list_node_base *pos, InputIterator first, InputIterator last) {
			while (first != last) {
				pos = _slist_make_link(pos, create_node(*first));
				++first;
			}
		}

		list_node_base* erase_after(list_node_base *pos) {
			list_node *next = (list_node*)(pos->next);
			list_node_base *next_n = next->next;
			pos->next = next_n;
			destroy_node(next);
			return next_n;
		}

		list_node_base* erase_after(list_node_base *before_first, list_node_base *last) {
			list_node *curr = (list_node*)(before_first->next);
			while (curr != last) {
				list_node *tmp = curr;
				curr = (list_node*)(curr->next);
				destroy_node(tmp);
			}
			before_first->next = last;
			return last;
		}
	public:
		iterator insert_after(iterator pos, const value_type &val) {
			return iterator(_insert_after(pos.node, val));
		}
		iterator insert_after(iterator pos) {
			return insert_after(pos, value_type());
		}
		void insert_after(iterator pos, size_type n, const value_type &val) {
			_insert_after_fill(pos.node, n, val);
		}
		template<class InItr>
		void insert_after(iterator pos, InItr first, InItr last) {
			_insert_range_after(pos.node, first, last);
		}

		iterator insert(iterator pos, const value_type& x) {
			return iterator(_insert_after(_slist_previous(&head, pos.node), x));
		}
		iterator insert(iterator pos) {
			return iterator(_insert_after(_slist_previous(&head, pos.node),
				value_type()));
		}
		void insert(iterator pos, size_type n, const value_type& x) {
			_insert_after_fill(_slist_previous(&head, pos.node), n, x);
		}
		template <class InIter>
		void insert(iterator pos, InIter first, InIter last) {
			_insert_range_after(_slist_previous(&head, pos.node), first, last);
		}
	public:
		iterator erase_after(iterator pos) {
			return iterator((list_node*)erase_after(pos.node));
		}
		iterator erase_after(iterator before_first, iterator last) {
			return iterator((list_node*)erase_after(before_first.node, last.node));
		}

		iterator erase(iterator pos) {
			return iterator((list_node*)erase_after(_slist_previous(&head,pos.node)));
		}
		iterator erase(iterator first, iterator last) {
			return iterator((list_node*)erase_after(_slist_previous(&head, first.node), last.node));
		}

		void clear() {
			erase_after(&head,0);
		}
	public:
		
		//将(first,last]内的节点，拼接到pos之后
		void splice_after(iterator pos, iterator before_first, iterator before_last) {

			if (before_first != before_last)
				_slist_splice_after(pos.node, before_first.node, before_last.node);
		}
		void splice_after(iterator pos, iterator prev) {
			_slist_splice_after(pos.node, prev.node, prev.node->next);
		}
		
		void splice(iterator pos, slist &sl) {
			if (sl.head.next)
				_slist_splice_after(_slist_previous(&head, pos.node),
					&sl.head, _slist_previous(&sl.head, 0));
		}
		void splice(iterator pos, slist &sl, iterator i) {
			_slist_splice_after(_slist_previous(&head, pos.node),
				_slist_previous(&sl.head, i.node),
				i.node);
		}
		void splice(iterator pos, slist &sl, iterator first, iterator last) {
			if (first != last) {
				_slist_splice_after(_slist_previous(&head, pos.node),
					_slist_previous(&sl.head, first.node),
					_slist_previous(first.node, last.node));
			}
		}
	public:
		void reverse() { if (head.next) head.next = _slist_reverse(head.next); }
		void remove(const T& val);
		void unique();
		void merge(slist& L);
		void sort();

	};

	template<class T,class Alloc>
	slist<T,Alloc>& slist<T, Alloc>::operator=(const slist<T,Alloc> &rhs) {
		if(this!=&rhs){
			list_node_base* p1 = &head;
			list_node* n1 = (list_node*)head.next;
			const list_node* n2 = (const list_node*)rhs.head.next;
			while (n1 && n2) {
				n1->data = n2->data;
				p1 = n1;
				n1 = (list_node*)n1->next;
				n2 = (const list_node*)n2->next;
			}
			if (n2 == 0)
				erase_after(p1, 0);
			else
				_insert_after_range(p1,
					const_iterator((list_node*)n2), const_iterator(0));
		}
		return *this;
	}
	
	template<class T,class Alloc>
	void slist<T, Alloc>::remove(const value_type &val) {
		list_node_base *cur = &head;
		while (cur&&cur->next) {
			if (((list_node*)cur->next)->data == val)
				erase_after(cur);
			else
				cur = cur->next;
		}
	}

	template<class T,class Alloc>
	void slist<T, Alloc>::unique() {
		list_node_base *cur = head.next;
		if (cur) {
			while (cur->next) {
				if ((((list_node*)cur)->data) == (((list_node*)cur->next)->data))
					erase_after(cur);
				else
					cur = cur->next;
			}
		}
	}

	template<class T,class Alloc>
	void slist<T, Alloc>::merge(slist &x) {
		list_node_base *n1 = &head;
		while (n1->next&&x.head.next) {
			if ((((list_node*)x.head.next)->data) < (((list_node*)n1->next)->data))
				_slist_splice_after(n1, &x.head, x.head.next);
			n1 = n1->next;
		}
		if (x.head.next)
		{
			n1->next = x.head.next;
			x.head.next = 0;
		}
	}

	template <class T, class Alloc>
	void slist<T, Alloc>::sort()
	{
		if (head.next && head.next->next) {
			slist carry;
			slist counter[64];
			int fill = 0;
			while (!empty()) {
				_slist_splice_after(&carry.head, &head, head.next);
				int i = 0;
				while (i < fill && !counter[i].empty()) {
					counter[i].merge(carry);
					carry.swap(counter[i]);
					++i;
				}
				carry.swap(counter[i]);
				if (i == fill)
					++fill;
			}
			for (int i = 1; i < fill; ++i)
				counter[i].merge(counter[i - 1]);
			this->swap(counter[fill - 1]);
		}
	}

}


#endif
