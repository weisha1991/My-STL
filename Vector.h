#pragma once
#ifndef VECTOR_H
#define VECTOR_H

#include "Alloc.h"
#include "Iterator1.h"
#include "ReverseIterator.h"
#include "Utility.h"
#include "uninitialized.h"
#include "Algobase.h"
#include <initializer_list>
#include <cassert>
namespace My_STL {

	template<class T,class Alloc=alloc>
	class vector {
	public:
		typedef T value_type;
		typedef value_type* pointer;
		typedef const value_type* const_pointer;
		typedef value_type* iterator;
		typedef const value_type* const_iterator;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef std::size_t size_type;
		typedef ptrdiff_t difference_type;

		typedef reverse_iterator<const_iterator> const_reverse_iterator;
		typedef reverse_iterator<iterator> reverse_iterator;
	protected:
		typedef simple_alloc<value_type, Alloc> data_allocator;
		iterator start;
		iterator finish;
		iterator end_of_storage;
		void insert_aux(iterator position, const T &x);
		void deallocate() {
			if (start)
				data_allocator::deallocate(start, end_of_storage-start);
		}

		void fill_initialize(size_type n, const T &value) {
			start = allocate_and_fill(n, value);
			finish = start + n;
			end_of_storage = finish;
		}

	public:
		iterator begin(){ return start; }
		const_iterator begin()const { return start; }

		iterator end() { return finish; }
		const_iterator end()const { return finish; }

		reverse_iterator rbegin() { return reverse_iterator(end()); }
		const_reverse_iterator rbegin()const { return const_reverse_iterator(end()); }

		reverse_iterator rend() { return reverse_iterator(begin()); }
		const_reverse_iterator rend()const { return const_reverse_iterator(begin()); }

		size_type size()const { return (size_type)(end()-begin()); }
		size_type max_size()const { return size_type(-1) / sizeof(T); }
		size_type capacity()const { return size_type(end_of_storage - begin()); }
		bool empty()const { return begin() == end(); }
		reference operator[](size_type n) { return *(begin()+n); }
		const_reference operator[](size_type n)const { return *(begin() + n); }

		vector():start(0),finish(0),end_of_storage(0){}
		vector(size_type n, const T &val) { fill_initialize(n, val); }
		vector(int n, const T &val) { fill_initialize(n, val); }
		vector(long n, const T &val) { fill_initialize(n, val); }
		explicit vector(size_type n) { fill_initialize(n, T()); }
		vector(std::initializer_list<value_type> il);


		vector(const vector &x) {
			start = allocate_and_copy(x.end() - x.begin(), x.begin(), x.end());
			finish = start + (x.end() - x.begin());
			end_of_storage = finish;
		}

		template<class InputIterator>
		vector(InputIterator first, InputIterator last):start(0),finish(0),end_of_storage(0)
		{
			range_initialize(first, last, iterator_category(first));
		}

		~vector() {
			destroy(start, finish);
			deallocate();
		}

		vector<T, Alloc>& operator=(const vector<T, Alloc>&);
		void reserve(size_type n) {
			if (capacity() < n) {
				const size_type old_size = size();
				iterator tmp = allocate_and_copy(n, start, finish);
				destroy(start, finish);
				deallocate();
				start = tmp;
				finish = start + old_size;
				end_of_storage= start + n;
			}
		}

		reference front(){ return *begin(); }
		const_reference front()const { return *begin(); }
		reference back() { return *(end()-1); }
		const_reference back()const { return (*end()-1); }
		
		void push_back(const T &val) {
			if (finish != end_of_storage) {
				construct(finish, val);
				++finish;
			}
			else
				insert_aux(end(), val);
		}

		void swap(vector<T, Alloc> &rhs) {
			My_STL::swap(start, rhs.start);
			My_STL::swap(finish, rhs.finish);
			My_STL::swap(end_of_storage, rhs.end_of_storage);
		}

		iterator insert(iterator position,const T &val){
			size_type n = position - begin();
			if (finish != end_of_storage&&position == end()) {
				construct(finish,val );
				++finish;
				//cout << "back insert" << endl;
			}
			else
				insert_aux(position, val);
			return begin() + n;
		}
		iterator insert(iterator position) { return insert(position, T()); }

		template<class InputIterator>
		void insert(iterator position, InputIterator first, InputIterator last) {
			range_insert(position, first, last,iterator_category(first));
		}

		void insert(iterator pos, size_type n, const T &val);
		void insert(iterator pos, int n, const T &val) {
			return insert(pos, (size_type)(n), val);
		}
		void insert(iterator pos, long n, const T &val) {
			return insert(pos, (size_type)(n), val);
		}
		
		void pop_back() {
			assert(finish != start);
			--finish;
			destroy(finish);
		}

		iterator erase(iterator pos) {
			if (pos + 1 != end())
				copy(pos + 1, finish, pos);
			--finish;
			destroy(finish);
			return pos;
		}
		iterator erase(iterator first, iterator last) {
			iterator i = copy(last, finish, first);
			destroy(i, finish);
			finish = finish - (last - first);
			return first;
		}

		void resize(size_type new_size, const T &val) {
			if (new_size < size())
				erase(begin() + new_size, end());
			else
				insert(end(),new_size-size(),val);
		}

		void resize(size_type new_size) { resize(new_size, T()); }
		void clear() { erase(begin(), end()); }
	protected:
		iterator allocate_and_fill(size_type n, const T &x) {
			iterator result = data_allocator::allocate(n);
			try {
				uninitialized_fill_n(result, n, x);
				return result;
			}
			catch (...) {
				data_allocator::deallocate(result, n);
				throw;
			}
		}

		template<class ForwardIterator>
		iterator allocate_and_copy(size_type n, ForwardIterator first, ForwardIterator last) {
			iterator result = data_allocator::allocate(n);
			try {
				uninitialized_copy(first, last, result);
				return result;
			}
			catch (...) {
				data_allocator::deallocate(result, n);
				throw;
			}
		}

		template<class InputIterator>
		void range_initialize(InputIterator first, InputIterator last,input_iterator_tag) {
			for (; first != last; ++first)
				push_back(*first);
		}

		template<class ForwardIterator>
		void range_initialize(ForwardIterator first, ForwardIterator last,
			forward_iterator_tag) {
			size_type n = 0;
			distance(first, last, n);
			start = allocate_and_copy(n, first, last);
			finish = start + n;
			end_of_storage = finish;
		}

		template<class InputIterator>
		void range_insert(iterator pos, InputIterator first, InputIterator last, input_iterator_tag);
		template<class ForwardIterator>
		void range_insert(iterator pos, ForwardIterator first, ForwardIterator last, forward_iterator_tag);
	};
	template<class T,class Alloc>
	vector<T, Alloc>::vector(std::initializer_list<value_type> il) {
		iterator tmp = data_allocator::allocate(il.size());
		start = tmp;
		for (auto iter = il.begin(); iter != il.end(); ++iter)
			construct(tmp++, *iter);
		finish = end_of_storage = start + il.size();
	}

	template<class T,class Alloc>
	inline bool operator==(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs) {
		return (lhs.size() == rhs.size()) && equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	template<class T,class Alloc>
	inline bool operator<(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs) {
		return lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	template<class T,class Alloc>
	void swap(vector<T, Alloc> &x, vector<T, Alloc> &y) {
		x.swap(y);
	}

	template<class T,class Alloc>
	vector<T, Alloc>& vector<T, Alloc>::operator=(const vector<T, Alloc> &rhs) {
		if (&rhs != this) {
			if (rhs.size()>capacity()) {
				iterator tmp = allocate_and_copy(rhs.end() - rhs.begin(), rhs.begin(), rhs.end());
				destroy(start, finish);
				deallocate();
				start = tmp;
				end_of_storage = start + (rhs.end() - rhs.begin());
			}
			else if (size()>=rhs.size()) {
				iterator tmp = copy(rhs.begin(), rhs.end(), begin());
				destroy(tmp, finish);
			}
			else {
				copy(rhs.begin(), rhs.begin() + size(), start);
				uninitialized_copy(rhs.begin() + size(), rhs.end(), finish);
			}
			finish = start + rhs.size();
		}
		return *this;
	}

	template<class T,class Alloc>
	void vector<T, Alloc>::insert_aux(iterator pos, const T &val) {
		if (finish != end_of_storage) {//判断是否剩余闲置空间
			construct(finish, *(finish - 1));
			++finish;
			T val_copy = val;
			copy_backward(pos, finish - 2, finish - 1);
			*pos = val_copy;
		}
		else {
			const size_type old_size = size();
			const size_type len = (old_size != 0) ? 2 * old_size : 1;
			iterator new_start = data_allocator::allocate(len);
			iterator new_finish = new_start;
			try {
				new_finish = uninitialized_copy(start, pos, new_start);
				construct(new_finish, val);
				++new_finish;
				new_finish = uninitialized_copy(pos, finish, new_finish);
			}
			catch(...) {
				destroy(new_start, new_finish);
				data_allocator::deallocate(new_start, len);
				throw;
			}
			destroy(begin(), end());
			deallocate();
			start = new_start;
			finish = new_finish;
			end_of_storage = new_start + len;
		}
	}

	template<class T,class Alloc>
	void vector<T, Alloc>::insert(iterator pos, size_type n, const T &val) {
		if (n != 0) {
			if (size_type(end_of_storage - finish) > n) {
				T val_copy = val;
				const size_type elems_after = finish - pos;
				iterator old_finish = finish;
				if (elems_after > n) {
					uninitialized_copy(finish - n, finish, finish);
					finish += n;
					copy_backward(pos, old_finish - n, old_finish);
					fill(pos, pos + n, val_copy);
				}
				else {
					uninitialized_fill_n(finish, n - elems_after, val_copy);
					finish += n - elems_after;
					uninitialized_copy(pos, old_finish, finish);
					finish += elems_after;
					fill(pos, old_finish, val_copy);
				}
			}
			else {
				//cout << "out of memory" << endl;
				const size_type old_size = size();
				const size_type len = old_size + max(old_size, n);//多分配备份空间
				iterator new_start = data_allocator::allocate(len);
				iterator new_finish = new_start;
				try {
					new_finish = uninitialized_copy(start, pos, new_start);
					new_finish = uninitialized_fill_n(new_finish, n, val);
					new_finish = uninitialized_copy(pos, finish, new_finish);
				}
				catch (...) {
					destroy(new_start, new_finish);
					data_allocator::deallocate(new_start, len);
					throw;
				}
				destroy(start, finish);
				deallocate();
				start = new_start;
				finish = new_finish;
				end_of_storage = start + len;
			}
		}
	}

	template<class T,class Alloc>
	template<class InputIterator>
	void vector<T, Alloc>::range_insert(iterator pos, InputIterator first, InputIterator last,
		input_iterator_tag) {
		for (; first != last; ++first) {
			pos = insert(pos, *first);
			++pos;
		}
	}

	template<class T,class Alloc>
	template<class ForwardIterator>
	void vector<T, Alloc>::range_insert(iterator pos, ForwardIterator first, ForwardIterator last,
		forward_iterator_tag) {
		if (first != last) {
			size_type n = 0;
			distance(first, last, n);
			if (size_type(end_of_storage - finish) >= n) {
				const size_type elems_after = finish - pos;
				iterator old_finish = finish;
				if (elems_after > n) {
					uninitialized_copy(finish - n, finish, finish);
					finish += n;
					copy_backward(pos, old_finish - n, old_finish);
					copy(first, last, pos);
				}
				else {
					ForwardIterator mid = first;
					advance(mid, elems_after);
					uninitialized_copy(mid, last, finish);
					finish += n - elems_after;
					uninitialized_copy(pos, old_finish, finish);
					finish += elems_after;
					copy(first, mid, pos);
				}
			}
			else {
				const size_type old_size = size();
				const size_type len = old_size + max(old_size, n);
				iterator new_start = data_allocator::allocate(len);
				iterator new_finish = new_start;
				try {
					new_finish = uninitialized_copy(start, pos, new_finish);
					new_finish = uninitialized_copy(first, last, new_finish);
					new_finish = uninitialized_copy(pos, finish, new_finish);
				}
				catch (...) {
					destroy(new_start, new_finish);
					data_allocator::deallocate(new_start, len);
					throw;
				}
				destroy(start, finish);
				deallocate();
				start = new_start;
				finish = new_finish;
				end_of_storage = new_start + len;
			}
		}
	}
}








#endif
