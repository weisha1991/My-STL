#pragma once
#include "RBTree.h"
#include "Utility.h"
#include "Functional.h"
#include "Alloc.h"
#include "ReverseIterator.h"

namespace My_STL {
	
	template<class Key, class T, class Compare = less<Key>, class Alloc = alloc>
	class map;

	template<class Key, class T, class Compare, class Alloc>
	bool operator==(const map<Key, T, Compare, Alloc> &, const map<Key, T, Compare, Alloc>&);

	template<class Key, class T, class Compare , class Alloc >
	bool operator<(const map<Key, T, Compare, Alloc> &, const map<Key, T, Compare, Alloc>&);


	template<class Key,class T,class Compare,class Alloc>
	class map {
	public:
		typedef Key key_type;
		typedef T data_type;
		typedef T mapped_type;
		typedef pair<const Key, T> value_type;
		typedef Compare key_compare;

		class value_compare :public binary_function<value_type, value_type, bool> {
			friend class map<Key, T, Compare, Alloc>;
		protected:
			Compare comp;
			value_compare(Compare c):comp(c){}
		public:
			bool operator()(const value_type &lhs, const value_type &rhs) {
				return comp(lhs.first, rhs.first);
			}
		};
	private:
		typedef rb_tree<key_type,value_type, select1st<value_type>, key_compare, Alloc> rep_type;
		rep_type t;
	public:
		typedef typename rep_type::pointer pointer;
		typedef typename rep_type::const_pointer const_pointer;
		typedef typename rep_type::reference reference;
		typedef typename rep_type::const_reference const_reference;
		typedef typename rep_type::iterator iterator;
		typedef typename rep_type::const_iterator const_iterator;
		typedef typename rep_type::size_type size_type;
		typedef typename rep_type::difference_type difference_type;
		typedef typename rep_type::reverse_iterator reverse_iterator;
		typedef typename rep_type::const_reverse_iterator const_reverse_iterator;

		map():t(Compare()){}
		explicit map(const Compare &x):t(x){}
		template<class InputIter>
		map(InputIter first, InputIter last) : t(Compare()) { t.insert_unique(first, last); }

		template<class InputIter>
		map(InputIter first, InputIter last, const Compare &x) : t(x) { t.insert_unique(first, last); }

		map(const map &rhs):t(rhs.t){}
		map& operator=(const map &rhs) {
			t = rhs.t;
			return *this;
		}

		key_compare key_comp()const { return t.key_comp(); }
		value_compare value_comp()const { return value_compare(t.key_comp()); }
		iterator begin() { return t.begin(); }
		const_iterator begin() const { return t.begin(); }
		iterator end() { return t.end(); }
		const_iterator end() const { return t.end(); }
		reverse_iterator rbegin() { return t.rbegin(); }
		const_reverse_iterator rbegin() const { return t.rbegin(); }
		reverse_iterator rend() { return t.rend(); }
		const_reverse_iterator rend() const { return t.rend(); }
		bool empty() const { return t.empty(); }
		size_type size() const { return t.size(); }
		size_type max_size() const { return t.max_size(); }
		T& operator[](const key_type& k) {
			return (*((insert(value_type(k, T()))).first)).second;
		}
		void swap(map<Key, T, Compare, Alloc>& x) { t.swap(x.t); }

		pair<iterator, bool> insert(const value_type &x) {
			return t.insert_unique(x);
		}
		iterator insert(iterator pos, const value_type &x) {
			return t.insert_unique(pos, x);
		}

		template<typename InputIter>
		void insert(InputIter first, InputIter last) {
			t.insert_unique(first, last);
		}

		void erase(iterator pos) {
			t.erase(pos);
		}
		size_type erase(const key_type &k) {
			return t.erase(k);
		}
		void erase(iterator first, iterator last) { t.erase(first,last); }
		void clear() { t.clear(); }

		iterator find(const key_type &k) { return t.find(k);}
		const_iterator find(const key_type &k)const { return t.find(k); }
		size_type count(const key_type &k) { return t.count(k); }
		iterator lower_bound(const key_type &k) { return t.lower_bound(k); }
		const_iterator lower_bound(const key_type &k) const{ return t.lower_bound(k); }
		iterator upper_bound(const key_type &k) { return t.upper_bound(k); }
		const_iterator upper_bound(const key_type &k) const{ return t.upper_bound(k); }

		pair<iterator, iterator> equal_range(const key_type &k) { return t.equal_range(k); }
		pair<const_iterator, const_iterator> equal_range(const key_type &k) const{ return t.equal_range(k); }
	public:
		template<class K1,class T1,class C1,class A1>
		friend bool operator==(const map<K1, T1, C1, A1> &lhs, const map<K1, T1, C1, A1> &rhs);

		//template<class K1, class T1, class C1, class A1>
		//friend bool operator<(const map<K1, T1, C1, A1> &lhs, const map<K1, T1, C1, A1> &rhs);
	};

	template<class Key, class T, class Compare, class Alloc>
	inline bool operator==(const map<Key, T, Compare, Alloc> &lhs, const map<Key, T, Compare, Alloc> &rhs) {
		return rhs.t == lhs.t;
	}

	//template<class Key, class T, class Compare, class Alloc >
	//inline bool operator<(const map<Key, T, Compare, Alloc> &lhs, const map<Key, T, Compare, Alloc> &rhs) {
	//	return lhs.t < rhs.t;
	//}

	//template<class Key, class T, class Compare, class Alloc >
	//inline bool operator==(const map<Key, T, Compare, Alloc> &lhs, const map<Key, T, Compare, Alloc> &rhs) {
	//	return !(lhs == rhs);
	//}

	//template<class Key, class T, class Compare, class Alloc >
	//inline bool operator>=(const map<Key, T, Compare, Alloc> &lhs, const map<Key, T, Compare, Alloc> &rhs) {
	//	return !(lhs < rhs);
	//}



}
