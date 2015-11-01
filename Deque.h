#pragma once
#ifndef DEQUE_H
#define DEQUE_H
#include "Iterator1.h"
#include "TypeTraits.h"
#include "ReverseIterator.h"
#include "Algobase.h"
#include "Alloc.h"
#include "uninitialized.h"
#include <initializer_list>
namespace My_STL {
	//返回每个缓冲区容纳的元素大小为sz的个数，若n==0，并且每个元素大小小于512，
	//默认缓冲区为512,返回512/sz个元素，若元素大小大于512,则返回1；
	//若n不为0，则返回n；
	inline size_t _deque_buf_size(size_t n, size_t sz) {
		return n != 0 ? n : (sz < 512 ? size_t(512 / sz) : size_t(1));
	}

	template<class T,class Ref,class Ptr,size_t Bufsize>
	struct _deque_iterator {
		typedef _deque_iterator<T, T&, T*, Bufsize> iterator;
		typedef _deque_iterator<T, const T&, const T*, Bufsize> const_iterator;
		static size_t buffer_size() { return _deque_buf_size(Bufsize, sizeof(T)); }
		
		typedef random_access_iterator_tag iterator_category;
		typedef T value_type;
		typedef Ptr pointer;
		typedef Ref reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef T** map_pointer;
		typedef _deque_iterator self;

		T *curr;
		T *first;
		T *last;
		map_pointer node;

		_deque_iterator(T *f,map_pointer n):curr(f),first(*n),last(*n+buffer_size()),
			node(n){}
		_deque_iterator():curr(0),first(0),last(0),node(0){}
		_deque_iterator(const iterator &rhs):curr(rhs.curr),first(rhs.first),
			last(rhs.last),node(rhs.node){}
		reference operator*() { return *curr; }
		pointer operator->() { return &(operator*()); }

		difference_type operator -(const _deque_iterator<T,Ref,Ptr,Bufsize> &x)const {//两个迭代器之间的元素个数
			return difference_type(buffer_size())*(node - x.node - 1) +
				(curr - first) + (x.last - x.curr);
		}

		void set_node(map_pointer new_node) {
			node = new_node;
			first = *new_node;
			last = first + difference_type(buffer_size());
		}

		self& operator++() {
			++curr;
			if (curr == last) {
				set_node(node + 1);
				curr = first;
			}
			return *this;
		}

		self operator++(int) {
			self tmp = *this;
			++*this;
			return tmp;
		}

		self& operator--() {
			if (curr == first) {
				set_node(node - 1);
				curr = last;
			}
			--curr;
			return *this;
		}

		self operator--(int) {
			self tmp = *this;
			--*this;
			return tmp;
		}

		self& operator+=(difference_type n) {
			difference_type offset = n + (curr - first);
			if (offset >= 0 && offset < difference_type(buffer_size()))
				curr += n;
			else {
				difference_type node_offset = offset > 0 ? (offset / (difference_type(buffer_size()))) :
					-difference_type((-offset - 1) / buffer_size()) - 1;
				set_node(node + node_offset);
				curr = first + (offset - node_offset*difference_type(buffer_size()));
			}
			return *this;
		}

		self operator+(difference_type n)const {
			self tmp = *this;
			return tmp += n;
		}

		self& operator-=(difference_type n) {
			return *this += -n;
		}

		self operator-(difference_type n)const {
			self tmp = *this;
			return tmp -= n;
		}

		reference operator[](difference_type n) const{ return *(*this + n); }
		bool operator==(const self &rhs) const{ return curr == rhs.curr; }
		bool operator!=(const self &rhs)const{return !(*this==rhs); }
		bool operator<(const self &rhs)const {
			return node == rhs.node ? curr < rhs.curr : (node < rhs.node);
		}		
	};

	template<class T,class Alloc=alloc,size_t Bufsize=0>
	class deque {
	public:
		typedef T value_type;
		typedef value_type* pointer;
		typedef const value_type* const_pointer;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
	public:
		typedef _deque_iterator<T, T&, T*, Bufsize> iterator;
		typedef _deque_iterator<T, const T&, const T*, Bufsize> const_iterator;
		typedef reverse_iterator<const_iterator> const_reverse_iterator;
		typedef reverse_iterator<iterator>  reverse_iterator;
	protected:
		typedef pointer* map_pointer;
		typedef simple_alloc<pointer, Alloc> map_alloctor;
		typedef simple_alloc<value_type, Alloc> data_allocator;
		static size_type buffer_size() {
			return _deque_buf_size(Bufsize, sizeof(value_type));
		}
		static size_type initial_map_size() { return 8; }
	protected:
		iterator start;
		iterator finish;
		map_pointer map;
		size_type map_size;
	public:	
		iterator begin() { return start; }
		const_iterator begin()const { return start; }
		const_iterator cbegin()const { return start; }
		iterator end() { return finish; }
		const_iterator end()const { return finish; }
		const_iterator cend()const { return finish; }
		reverse_iterator rbegin() { return reverse_iterator(finish); }
		const_reverse_iterator rbegin()const { return const_reverse_iterator(finish); }
		reverse_iterator rend() { return reverse_iterator(start); }
		const_reverse_iterator rend()const { return const_reverse_iterator(start); }

		reference operator[](size_type n) { return start[difference_type(n)]; }
		const_reference operator[](size_type n)const { return start[difference_type(n)]; }

		reference front() { return *start; }
		const_reference front()const { return *start; }
		reference back(){
			iterator tmp = finish;
			--tmp;
			return *tmp;
		}
		const_reference back()const {
			const_iterator tmp = finish;
			--tmp;
			return *tmp;
		}

		size_type size()const { return finish - start;}
		size_type max_size()const { return size_type(-1); }
		bool empty()const { return start == finish; }
	public:
		deque():start(),finish(),map(0),map_size(0){
			create_map_and_nodes(0);
	//		cout << "done" << endl;
		}

		deque(const deque &rhs) :start(), finish(), map(0), map_size(0) {
			create_map_and_nodes(rhs.size());
			try {
				uninitialized_copy(rhs.begin(), rhs.end(), start);
			}
			catch (...) {
				destroy_map_and_nodes();
				throw;
			}
		}

		deque(size_type n, const value_type &val):start(),finish(),map(0),map_size(0) 
		{ fill_initialize(n, val); }
		deque(int n, const value_type &val):start(),finish(),map(0),map_size(0) {
			fill_initialize(n, val); 
		}
		deque(long n, const value_type &val) :start(), finish(), map(0), map_size(0) {
			fill_initialize(n, val); 
		}
		template<class InputIterator>
		deque(InputIterator first,InputIterator last):start(),finish(),map(0),map_size(0){
			range_initialize(first, last, iterator_category(first));
		}
		deque(std::initializer_list<value_type> il) {
			range_initialize(il.begin(), il.end(), input_iterator_tag());
		}
		~deque() {
			destroy(start, finish);
			destroy_map_and_nodes();
		}
	public:
		void push_back(const value_type &val){
			if (finish.curr != finish.last - 1) {
				construct(finish.curr, val);
				++finish.curr;
			}
			else
				push_back_aux(val);
		}
		void push_front(const value_type &val) {
			if (start.curr != start.first) {
				construct(start.curr - 1, val);
				--start.curr;
			}
			else
				push_front_aux(val);
		}

		void pop_back() {
			if (finish.curr != finish.first) {
				--finish.curr;
				destroy(finish.curr);
			}
			else
				pop_back_aux();
		}
		void pop_front() {
			if (start.curr+1 != start.last) {
				destroy(start.curr);
				++start.curr;
			}
			else
				pop_front_aux();
		}

	public:
		iterator insert(iterator pos, const value_type &val) {
			if (pos.curr == start.curr) {
				push_front(val);
				return start;
			}
			else if (pos.curr == finish.curr) {
				push_back(val);
				iterator tmp = finish;
				--tmp;
				return tmp;
			}
			else
				return insert_aux(pos, val);
		}

	protected:
		iterator insert_aux(iterator pos, const value_type &val);
	protected:
		void push_back_aux(const value_type&);
		void push_front_aux(const value_type&);
		void pop_back_aux();
		void pop_front_aux();
	public:
		iterator erase(iterator pos);
		iterator erase(iterator first, iterator last);
		void clear();
	protected:
		void create_map_and_nodes(size_type num_elements);
		void destroy_map_and_nodes();
		void fill_initialize(size_type, const value_type&);
		template<class InputIterator>
		void range_initialize(InputIterator first, InputIterator last, input_iterator_tag);
		template<class ForwardIterator>
		void range_initialize(ForwardIterator first, ForwardIterator last, forward_iterator_tag);


	protected:
		void reallocate_map_front(size_type nodes_to_add=1) {
			if (nodes_to_add+map > start.node )
				reallocate_map(nodes_to_add, true);
		}
		void reallocate_map_back(size_type nodes_to_add=1) {
			if (nodes_to_add + 1 > map_size - (finish.node - map))
				reallocate_map(nodes_to_add, false);
		}

		void reallocate_map(size_type nodes_to_add, bool add_front);
		pointer allocate_node() { return data_allocator::allocate(buffer_size()); }
		void deallocate_node(pointer ptr) { 
			//cout << "hehe"<<endl;
		return data_allocator::deallocate(ptr, buffer_size()); }
	};

	template<class T,class Alloc,size_t Bufsize>
	typename deque<T,Alloc,Bufsize>::iterator 
		deque<T, Alloc, Bufsize>::erase(iterator pos) {
		iterator next = pos;
		++next;
		difference_type index = pos - start;
		if (index < (size() >> 1)) {
			copy_backward(start, pos, next);
			pop_front();
		}
		else {
			copy(next, finish, pos);
			pop_back();
		}
		return start+index;
	}

	template<class T,class Alloc,size_t Bufsize>
	typename deque<T,Alloc,Bufsize>::iterator
		deque<T,Alloc,Bufsize>::erase(iterator first, iterator last) {
		if (first == start&&finish == last) {
			clear();
			return finish;
		}
		else {
			difference_type n = last - first;
			difference_type elems_before = first - start;
			if (elems_before < (size() - n) / 2) {
				copy_backward(start, first, last);
				iterator new_start = start + n;
				destroy(start, new_start);
				for (map_pointer curr_node = start.node; curr_node < new_start.node; ++curr_node)
					data_allocator::deallocate(*curr_node, buffer_size());
				start = new_start;
			}
			else {
				copy(last, finish, first);
				iterator new_finish = finish - n;
				destroy(new_finish, finish);
				for (map_pointer cur_node = new_finish.node + 1; cur_node <= finish.node; ++cur_node)
					data_allocator::deallocate(*cur_node,buffer_size());
				finish = new_finish;
			}
			return start + elems_before;
		}
	}

	template<class T, class Alloc, size_t Bufsize>
	void deque<T, Alloc, Bufsize>::clear() {
		for (map_pointer cur_node = start.node + 1; cur_node < finish.node; ++cur_node)
		{
			destroy(*cur_node, *cur_node + buffer_size());
			data_allocator::deallocate(*cur_node, buffer_size());
		}
		if (start.node != finish.node) {
			destroy(start.curr, start.last);
			destroy(finish.first, finish.curr);
			data_allocator::deallocate(finish.first, buffer_size());
		}
		else
			destroy(start.curr, start.last);
		finish = start;
		
	}

	template<class T,class Alloc,size_t Bufsize>
	void deque<T, Alloc, Bufsize>::create_map_and_nodes(size_type num_elements) {
		size_type num_nodes = num_elements/buffer_size() + 1;
		map_size = max(initial_map_size(), num_nodes + 2);
		map = map_alloctor::allocate(map_size);
		map_pointer nstart = map + (map_size - num_nodes) / 2;
		map_pointer nfinish = nstart + num_nodes - 1;
		map_pointer curr;
		try {
			for (curr = nstart; curr <=nfinish; ++curr)
				*curr = allocate_node();
		}
		catch (...) {
			for (map_pointer n = nstart; n < curr; ++n)
				deallocate_node(*n);
			map_alloctor::deallocate(map,map_size);
			throw;
		}
		start.set_node(nstart);
		finish.set_node(nfinish);
		start.curr = start.first;
		finish.curr = finish.first + num_elements%(buffer_size());
	}

	template<class T,class Alloc,size_t Bufsize>
	void deque<T, Alloc, Bufsize>::destroy_map_and_nodes() {
		for (map_pointer curr = start.node; curr <= finish.node; ++curr)
			deallocate_node(*curr);
		map_alloctor::deallocate(map,map_size);
	}

	template<class T, class Alloc, size_t Bufsize>
	void deque<T,Alloc,Bufsize>::fill_initialize(size_type n, const value_type &val) {
		create_map_and_nodes(n);
		map_pointer curr;
		try {
			for (curr = start.node; curr <finish.node; ++curr)
				uninitialized_fill(*curr, *curr + buffer_size(), val);
			uninitialized_fill(finish.first, finish.curr, val);
		}
		catch (...) {
			for (map_pointer ptr = start.node; ptr < curr; ++ptr)
				destroy(*ptr, *ptr + buffer_size());
			destroy_map_and_nodes();
		}
	}

	template<class T,class Alloc,size_t Bufsize>
	template<class InputIterator>
	void deque<T, Alloc, Bufsize>::range_initialize(InputIterator first, InputIterator last,
		input_iterator_tag) {
		create_map_and_nodes(0);
		for (; first != last; ++first)
			push_back(*first);
	}

	template<class T, class Alloc, size_t Bufsize>
	template<class ForwardIterator>
	void deque<T, Alloc, Bufsize>::range_initialize(ForwardIterator first, ForwardIterator last,
		forward_iterator_tag) {
		size_type n = 0;
		distance(first, last, n);
		create_map_and_nodes(n);
		try {
			uninitialized_copy(first, last, start);
		}
		catch (...) {
			destroy_map_and_nodes();
		}
	}
	
	template<class T,class Alloc,size_t Bufsize>
	void deque<T, Alloc, Bufsize>::reallocate_map(size_type nodes_to_add, bool add_front) {
		size_type old_num_nodes = finish.node - start.node + 1;
		size_type new_num_nodes = old_num_nodes + nodes_to_add;
		map_pointer new_start;
		if (map_size > 2 * new_num_nodes) {
			new_start = map + (map_size - new_num_nodes) / 2 + (add_front ? nodes_to_add : 0);
			if (new_start < start.node)
				copy(start.node, finish.node + 1, new_start);
			else
				copy_backward(start.node, finish.node + 1, new_start + old_num_nodes);
		}
		else {
			size_type new_map_size = map_size + max(map_size, nodes_to_add) + 2;
			map_pointer new_map = map_alloctor::allocate(new_map_size);
			new_start = new_map + (new_map_size - new_num_nodes) / 2 + (add_front ? nodes_to_add : 0);
			copy(start.node, finish.node + 1, new_start);
			map_alloctor::deallocate(map, map_size);
			map = new_map;
			map_size = new_map_size;
		}
		start.set_node(new_start);
		finish.set_node(start.node + old_num_nodes - 1);
	}

	template<class T,class Alloc,size_t Bufsize>
	typename deque<T, Alloc, Bufsize>::iterator
		deque<T, Alloc, Bufsize>::insert_aux(iterator pos, const value_type &val) {
		difference_type index = pos - start;
		value_type val_copy = val;
		if (index < size() / 2) {
			push_front(front());
			iterator front1 = start;
			++front1;
			iterator front2 = front1;
			++front2;
			pos = start + index;
			iterator pos1 = pos;
			++pos1;
			copy(front1, pos1, front2);
		}
		else {
			push_back(back());
			iterator back1 = finish;
			--back1;
			iterator back2 = back1;
			--back2;
			pos = start + index;
			copy_backward(pos, back2, back1);
		}
		*pos = val_copy;
		return pos;
	}

	template<class T,class Alloc,size_t Bufsize>
	void deque<T, Alloc, Bufsize>::push_back_aux(const value_type &val) {	
		value_type val_copy = val;
		reallocate_map_back();
		*(finish.node + 1) = allocate_node();
		try {
			construct(finish.curr, val_copy);
			finish.set_node(finish.node + 1);
			finish.curr = finish.first;
		}
		catch (...) {
			deallocate_node(*(finish.node + 1));
			throw;
		}
	}

	template<class T, class Alloc, size_t Bufsize>
	void deque<T, Alloc, Bufsize>::push_front_aux(const value_type &val) {
		value_type val_copy = val;
		reallocate_map_front();
		*(start.node - 1) = allocate_node();
		try {
			start.set_node(start.node - 1);
			start.curr = start.last - 1;
			construct(start.curr, val);
		}
		catch (...) {
			start.set_node(start.node + 1);
			start.curr = start.first;
			deallocate_node(*(start.node - 1));
			throw;
		}
	}

	template<class T,class Alloc,size_t Bufsize>
	void deque<T,Alloc,Bufsize>::pop_back_aux() {
		deallocate_node(finish.first);
		finish.set_node(finish.node - 1);
		finish.curr = finish.last - 1;
		destroy(finish.curr);
	}

	template<class T, class Alloc, size_t Bufsize>
	void deque<T, Alloc, Bufsize>::pop_front_aux() {
		destroy(start.curr);
		deallocate_node(start.first);
		start.set_node(start.node + 1);
		start.curr = start.first;		
	}

}

#endif
