#ifndef UNINITIALIZED_H
#define UNINITIALIZED_H

#include "Algobase.h"
#include "Construct.h"
#include "TypeTraits.h"
#include "Iterator1.h"
#include "Utility.h"

namespace My_STL{
	
	/*template<class InputIterator,class ForwardIterator>
	ForwardIterator _uninitialized_copy_aux(InputIterator first, InputIterator last,
		ForwardIterator result, _true_type);

	template<class InputIterator, class ForwardIterator>
	ForwardIterator _uninitialized_copy_aux(InputIterator first, InputIterator last,
		ForwardIterator result, _false_type);

	template<class InputIterator, class ForwardIterator>
	ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result){
		typedef typename _type_traits<iterator_traits<InputIterator>::value_type>::is_POD_type is_POD_type;
		return _uninitialized_copy_aux(first, last, result, is_POD_type());
	}
	template<>
	inline char* uninitialized_copy(const char *first, const char *last, char *result)
	{
		memmove(result, first, last - first);
		return result + (last - first);
	}

	template<>
	inline wchar_t* uninitialized_copy(const wchar_t *first, const wchar_t *last,wchar_t *result){
		memmove(result, first, last - first);
		return result + (last - first);
	}

	template<class InputIterator, class ForwardIterator>
	ForwardIterator _uninitialized_copy_aux(InputIterator first, InputIterator last,
		ForwardIterator result, _true_type){
		memcpy(result, first, (last - first)*sizeof(*first));
		return result + (last - first);
	}

	template<class InputIterator, class ForwardIterator>
	ForwardIterator _uninitialized_copy_aux(InputIterator first, InputIterator last,
		ForwardIterator result, _false_type){
		int i = 0;
		for (; first != last; ++first, ++i){
			construct((result + i), *first);
		}
		return result + i;
	}

	template<class ForwardIterator,class T>
	void _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T &value,
		_true_type);
	template<class ForwardIterator,class T>
	void _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T &value,
		_false_type);

	template<class ForwardIterator,class T>
	void uninitialiez_fill(ForwardIterator first, ForwardIterator last, const T&value)
	{
		typedef typename _type_traits<T>::is_POD_type is_POD_type;
		_uninitialized_fill_aux(first, last, value, is_POD_type());
	}

	template<class ForwardIterator,class T>
	void _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T &value,
		_true_type){
		fill(first, last, value);
	}

	template<class ForwardIterator, class T>
	void _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T &value,
		_false_type){
		int i = 0;
		for (; first != last; ++first, ++i){
			construct(first, value);
		}
	}

	template<class ForwardIterator,class Size,class T>
	ForwardIterator _uninitialized_fill_n_aux(ForwardIterator first, Size n,
		const T&x, _true_type);

	template<class ForwardIterator, class Size, class T>
	ForwardIterator _uninitialized_fill_n_aux(ForwardIterator first, Size n,
		const T&x, _false_type);

	template<class ForwardIterator, class Size, class T>
	inline ForwardIterator uninitialiez_fill_n(ForwardIterator first, Size n, const T&value){
		typedef typename _type_traits<T>::is_POD_type is_POD_type;
		return _uninitialized_fill_n_aux(first, n, value, is_POD_type());
	}

	template<class ForwardIterator, class Size, class T>
	ForwardIterator _uninitialized_fill_n_aux(ForwardIterator first, Size n,
		const T&x, _true_type){
		return fill_n(first, n, x);
	}

	template<class ForwardIterator, class Size, class T>
	ForwardIterator _uninitialized_fill_n_aux(ForwardIterator first, Size n,
		const T&x, _false_type){
		int i = 0;
		for (; i != n; ++i)
			construct((T*)(first + i), value);
		return first + i;
	}*/
	template<class InputIterator,class ForwardIterator>
	inline ForwardIterator
		_uninitialized_copy_aux(InputIterator first, InputIterator last,
			ForwardIterator result,
			_true_type) {
		return copy(first, last, result);
	}

	template <class InputIterator, class ForwardIterator>
	ForwardIterator _uninitialized_copy_aux(InputIterator first, InputIterator last,
			ForwardIterator result,_false_type) {
		ForwardIterator cur = result;
		try{
			for (; first != last; ++first, ++cur)
			construct(&*cur, *first);
		return cur;
		}
		catch (...) { destroy(result, cur); throw; }
	}

	template <class InputIterator, class ForwardIterator, class T>
	inline ForwardIterator
		_uninitialized_copy(InputIterator first, InputIterator last,
			ForwardIterator result, T*) {
		typedef typename _type_traits<T>::is_POD_type is_POD;
		return _uninitialized_copy_aux(first, last, result, is_POD());
	}

	template <class InputIterator, class ForwardIterator>
		inline ForwardIterator
		uninitialized_copy(InputIterator first, InputIterator last,
			ForwardIterator result) {
		return _uninitialized_copy(first, last, result, value_type(result));
	}

	inline char* uninitialized_copy(const char* first, const char* last,
		char* result) {
		memmove(result, first, last - first);
		return result + (last - first);
	}

	inline wchar_t* uninitialized_copy(const wchar_t* first, const wchar_t* last,
		wchar_t* result) {
		memmove(result, first, sizeof(wchar_t) * (last - first));
		return result + (last - first);
	}
	
	template <class InputIterator, class Size, class ForwardIterator>
	pair<InputIterator, ForwardIterator>
		_uninitialized_copy_n(InputIterator first, Size count,ForwardIterator result,input_iterator_tag) {
		ForwardIterator cur = result;
		try{
			for (; count > 0; --count, ++first, ++cur)
				construct(&*cur, *first);
		return pair<InputIterator, ForwardIterator>(first, cur);
		}
		catch (...) { 
			destroy(result, cur); 
			throw;
		}
	}

	template <class RandomAccessIterator, class Size, class ForwardIterator>
	inline pair<RandomAccessIterator, ForwardIterator>
		_uninitialized_copy_n(RandomAccessIterator first, Size count,
			ForwardIterator result,random_access_iterator_tag) {
		RandomAccessIterator last = first + count;
		return make_pair(last, uninitialized_copy(first, last, result));
	}

	template <class InputIterator, class Size, class ForwardIterator>
	inline pair<InputIterator, ForwardIterator>
		uninitialized_copy_n(InputIterator first, Size count,ForwardIterator result) {
		return _uninitialized_copy_n(first, count, result,
			iterator_category(first));
	}

	template <class ForwardIterator, class T>
	inline void
		_uninitialized_fill_aux(ForwardIterator first, ForwardIterator last,const T& x, _true_type)
	{
		fill(first, last, x);
	}

	template <class ForwardIterator, class T>
	void _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last,const T& x, _false_type)
	{
		ForwardIterator cur = first;
		try{
			for (; cur != last; ++cur)
			construct(&*cur, x);
		}
		catch (...) { destroy(first, cur); throw; }
	}

	template <class ForwardIterator, class T, class T1>
	inline void _uninitialized_fill(ForwardIterator first, ForwardIterator last,
		const T& x, T1*) {
		typedef typename _type_traits<T1>::is_POD_type is_POD;
		_uninitialized_fill_aux(first, last, x, is_POD());

	}

	template <class ForwardIterator, class T>
	inline void uninitialized_fill(ForwardIterator first, ForwardIterator last,
		const T& x) {
		_uninitialized_fill(first, last, x, value_type(first));
	}

	template <class ForwardIterator, class Size, class T>
	inline ForwardIterator
		_uninitialized_fill_n_aux(ForwardIterator first, Size n,
			const T& x, _true_type) {
		return fill_n(first, n, x);
	}

	template <class ForwardIterator, class Size, class T>
	ForwardIterator
		_uninitialized_fill_n_aux(ForwardIterator first, Size n,
			const T& x, _false_type) {
		ForwardIterator cur = first;
		try{
			for (; n > 0; --n, ++cur)
				construct(&*cur, x);
			return cur;
		}
		catch (...) { destroy(first, cur); throw; }
	}

	template <class ForwardIterator, class Size, class T, class T1>
	inline ForwardIterator _uninitialized_fill_n(ForwardIterator first, Size n,
		const T& x, T1*) {
		typedef typename _type_traits<T1>::is_POD_type is_POD;
		return _uninitialized_fill_n_aux(first, n, x, is_POD());

	}

	template <class ForwardIterator, class Size, class T>
	inline ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n,
		const T& x) {
		return _uninitialized_fill_n(first, n, x, value_type(first));
	}


	template <class InputIterator1, class InputIterator2, class ForwardIterator>
	inline ForwardIterator
		_uninitialized_copy_copy(InputIterator1 first1, InputIterator1 last1,
			InputIterator2 first2, InputIterator2 last2,
			ForwardIterator result) {
		ForwardIterator mid = uninitialized_copy(first1, last1, result);
		try{
			return uninitialized_copy(first2, last2, mid);
		}
		catch (...) { destroy(result, mid); throw; }
	}

	template <class ForwardIterator, class T, class InputIterator>
	inline ForwardIterator
		_uninitialized_fill_copy(ForwardIterator result, ForwardIterator mid,
			const T& x,InputIterator first, InputIterator last) {
		uninitialized_fill(result, mid, x);
		try{
			return uninitialized_copy(first, last, mid);
		}
		catch (...) {
			destroy(result, mid);
			throw;
		}
	}

	template <class InputIterator, class ForwardIterator, class T>
	inline void
		_uninitialized_copy_fill(InputIterator first1, InputIterator last1,
			ForwardIterator first2, ForwardIterator last2,const T& x) {
		ForwardIterator mid2 = uninitialized_copy(first1, last1, first2);
		try{
			uninitialized_fill(mid2, last2, x);
		}
		catch (...) { 
			destroy(first2, mid2); 
			throw;
		}
	}

}


#endif