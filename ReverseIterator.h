#ifndef REVERSEITERATOR_H
#define REVERSEITERATOR_H
#include "Iterator1.h"

namespace My_STL{

	template<class Iterator>
	class reverse_iterator{
	public:
		typedef typename iterator_traits<Iterator>::iterator_category
			iterator_category;
		typedef typename iterator_traits<Iterator>::value_type
			value_type;
		typedef typename iterator_traits<Iterator>::difference_type
			difference_type;
		typedef typename iterator_traits<Iterator>::pointer
			pointer;
		typedef typename iterator_traits<Iterator>::reference
			reference;

		typedef Iterator iterator_type;
		typedef reverse_iterator<Iterator> self;

	protected:
		Iterator current;
	public:
		reverse_iterator(){}
		explicit reverse_iterator(iterator_type x) :current(x){}
		reverse_iterator(const self &x) :current(x.current){}

		template<class Iter>
		reverse_iterator(const reverse_iterator<Iter> &x) : current(x.current){}

		iterator_type base()const{ return current; }

		reference operator*()const{
			Iterator tmp = current;
			return *--tmp;
		}

		pointer operator()()const{ return &(this->operator*()); }

		self& operator++(){
			--current;
			return *this;
		}

		self& operator++(int){
			self tmp = *this;
			--current;
			return tmp;
		}

		self& operator--(){
			++current;
			return *this;
		}

		self& operator--(int){
			self tmp = *this;
			++current;
			return tmp;
		}

		self operator+(difference_type n)const{
			return self(current - n);
		}
		self& operator+=(difference_type n){
			current -= n;
			return *this;
		}

		self operator-(difference_type n)const{
			return self(current + n);
		}

		self operator-=(difference_type n){
			current += n;
			return *this;
		}

		reference operator[](difference_type n)const{
		return *(*this + n);
		}
	};

	template<typename Iterator>
	inline bool operator==(const reverse_iterator<Iterator> &x,
		const reverse_iterator<Iterator> &y){
		return x.base() == y.base();
	}

	template<typename Iterator>
	inline bool operator!=(const reverse_iterator<Iterator> &x,
		const reverse_iterator<Iterator> &y) {
		return x.base()!=y.base();
	}


	template<typename Iterator>
	inline bool operator <(const reverse_iterator<Iterator> &x,
		const reverse_iterator<Iterator> &y){
		return x.base() < y.base();
	}

	template<typename Iterator>
	inline typename reverse_iterator<Iterator>::difference_type
		operator-(const reverse_iterator<Iterator> &x,
		const reverse_iterator<Iterator> &y){
			return y.base() - x.base();
		}

	template<typename Iterator>
	inline  reverse_iterator<Iterator>
		operator+(typename reverse_iterator<Iterator>::difference_type n, const reverse_iterator<Iterator> &x){
			return reverse_iterator<Iterator>(x.base() - n);
		}
}

#endif