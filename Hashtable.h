#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "Utility.h"
#include "Algo.h"
#include "Functional.h"
//#include "Hashfun.h"
#include "Alloc.h"
#include "Iterator1.h"
#include "Vector.h"
#include <iostream>
namespace My_STL{
	using std::ostream;
	using std::endl;
	template<class Value>
	struct _hash_table_node{
		_hash_table_node *next;
		Value val;
	};

	template<class Value,class Key,class Hashfun,class ExtractKey,class EqualKey,class Alloc=alloc>
	class hashtable;

	template<class Value,class Key,class Hashfun,class ExtractKey,class EqualKey,class Alloc>
	struct _hashtable_iterator;

	template<class Value,class Key,class Hashfun,class ExtractKey,class EqualKey,class Alloc>
	struct _hashtable_const_iterator;


	template<class Value,class Key,class Hashfun,class ExtractKey,class EqualKey,class Alloc>
	struct  _hashtable_iterator{
		typedef hashtable<Value,Key,Hashfun,ExtractKey,EqualKey,Alloc> hashtable;
		typedef _hashtable_iterator<Value,Key,Hashfun,ExtractKey,EqualKey,Alloc> iterator;
		typedef _hashtable_const_iterator<Value,Key,Hashfun,ExtractKey,EqualKey,Alloc> const_iterator;
		typedef _hash_table_node<Value> node;
		typedef forward_iterator_tag iterator_categoty;
		typedef Value value_type;
		typedef ptrdiff_t difference_type;
		typedef size_t size_type;
		typedef Value& reference;
		typedef Value* pointer;

		node *cur;
		hashtable *ht;
		_hashtable_iterator(node *n,hashtable *tab):cur(n),ht(tab){}
		_hashtable_iterator(){}
		reference operator*()const{return cur->val;}
		pointer operator->()const{return &(operator*());}
		iterator& operator++();
		iterator operator++(int);
		bool operator==(const iterator &rhs)const{return cur==rhs.cur;}
		bool operator !=(const iterator &rhs)const{return cur!=rhs.cur;}
	};

	template<class Value,class Key,class Hashfun,class ExtractKey,class EqualKey,class Alloc>
	struct _hashtable_const_iterator {
	  typedef hashtable<Value, Key, Hashfun, ExtractKey, EqualKey, Alloc>  hashtable;
	  typedef _hashtable_iterator<Value, Key, Hashfun,ExtractKey, EqualKey, Alloc> iterator;
	  typedef _hashtable_const_iterator<Value, Key, Hashfun,ExtractKey, EqualKey, Alloc>  const_iterator;
	   typedef _hash_table_node<Value> node;

	  typedef forward_iterator_tag iterator_category;
	  typedef Value value_type;
	  typedef ptrdiff_t difference_type;
	  typedef size_t size_type;
	  typedef const Value& reference;
	  typedef const Value* pointer;

	  const node* cur;
	  const hashtable* ht;
	  _hashtable_const_iterator(const node* n, const hashtable* tab)
	    : cur(n), ht(tab) {}
	  _hashtable_const_iterator() {}
	  _hashtable_const_iterator(const iterator& it) : cur(it.cur), ht(it.ht) {}

	  reference operator*() const { return cur->val; }
	  pointer operator->() const { return &(operator*()); }
	  const_iterator& operator++();
	  const_iterator operator++(int);
	  bool operator==(const const_iterator& it) const { return cur == it.cur; }
	  bool operator!=(const const_iterator& it) const { return cur != it.cur; }
	};

	/*
	*/
	static const int _num_primes=28;
	static const unsigned long _prime_list[_num_primes]={
	53,         97,           193,         389,       769,
  	1543,       3079,         6151,        12289,     24593,
  	49157,      98317,        196613,      393241,    786433,
  	1572869,    3145739,      6291469,     12582917,  25165843,
  	50331653,   100663319,    201326611,   402653189, 805306457,
  	1610612741, 3221225473ul, 4294967291ul
	};

	inline unsigned long _next_prime(unsigned long n){
		const unsigned long *first=_prime_list;
		const unsigned long *last=_prime_list+_num_primes;
		const unsigned long *pos=lower_bound(first,last,n);
		return pos==last?*(last-1):*pos;
	}

	template<class Value,class Key,class Hashfun,class ExtractKey,class EqualKey,class Alloc>
	bool operator ==(const hashtable<Value,Key,Hashfun,ExtractKey,EqualKey,Alloc> &lhs,
			const hashtable<Value,Key,Hashfun,ExtractKey,EqualKey,Alloc> &rhs);

	template<class Value,class Key,class Hashfun,class ExtractKey,class EqualKey,class Alloc>
	class hashtable{
	public:
		typedef Key key_type;
		typedef Value value_type;
		typedef Hashfun hasher;
		typedef EqualKey key_equal;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef value_type* pointer;
		typedef const value_type* const_pointer;
		typedef value_type& reference;
		typedef const value_type & const_reference;
		hasher hash_func()const{return hash;}
		key_equal key_eq()const{return equals;}
	private:
		hasher hash;
		key_equal equals;
		ExtractKey get_key;
		typedef _hash_table_node<Value> node;
		typedef simple_alloc<node,Alloc> node_allocator;
	vector<node*,Alloc> buckets;
	size_type  num_elements;
	public:
		typedef _hashtable_iterator<Value,Key,Hashfun,ExtractKey,EqualKey,Alloc> iterator;
		typedef _hashtable_const_iterator<Value,Key,Hashfun,ExtractKey,EqualKey,Alloc> const_iterator;
		friend struct _hashtable_iterator<Value,Key,Hashfun,ExtractKey,EqualKey,Alloc>;
		friend struct _hashtable_const_iterator<Value,Key,Hashfun,ExtractKey,EqualKey,Alloc>;
	public:
		hashtable(size_type n,const Hashfun &hf,const EqualKey &eq,const ExtractKey &ek):hash(hf),equals(eq),get_key(ek),num_elements(0){
				initialize_buckets(n);
			}
		hashtable(size_type n,const Hashfun &hf,const EqualKey &eq):hash(hf),equals(eq),get_key(ExtractKey()),
			num_elements(0){
				initialize_buckets(n);
		}

		hashtable(const hashtable &rhs):hash(rhs.hash),equals(rhs.equals),get_key(rhs.get_key),num_elements(0){
			copy_from(rhs);
		}

		hashtable& operator=(const hashtable &rhs){
			if(this!=&rhs){
				clear();
				hash=rhs.hash;
				equals=rhs.equals;
				get_key=rhs.get_key;
				copy_from(rhs);
			}
			return *this;
		}
		~hashtable(){clear();}
		size_type size()const{return num_elements;}
		size_type max_size()const{return size_type(-1);}
		bool empty()const{return size()==0;}
		void swap(hashtable &rhs){
			My_STL::swap(hash,rhs.hash);
			My_STL::swap(equals,rhs.equals);
			My_STL::swap(get_key,rhs.get_key);
			buckets.swap(rhs.buckets);
			My_STL::swap(num_elements,rhs.num_elements);
		}

		iterator begin(){
			for(size_type n=0;n<buckets.size();++n)
				if(buckets[n])
					return iterator(buckets[n],this);
			return end();
		}
		iterator end(){return iterator(0,this);}
		const_iterator begin()const{
			for(size_type n=0;n<buckets.size();++n)
				if(buckets[n])
					return const_iterator(buckets[n],this);
			return end();
		}
		const_iterator end()const{return const_iterator(0,this);}

		friend bool operator== <Value,Key,Hashfun,ExtractKey,EqualKey,Alloc> (const hashtable<Value,Key,Hashfun,ExtractKey,EqualKey,Alloc>&,
			const hashtable<Value,Key,Hashfun,ExtractKey,EqualKey,Alloc>&);

		// friend bool operator==  (const hashtable&,const hashtable&);
	public:
		void print_inside(ostream &os);
		size_type bucket_count()const{return buckets.size();}
		size_type max_bucket_count()const{return _prime_list[_num_primes-1];}
		size_type elems_in_bucket(size_type bucket)const{
			size_type result=0;
			for(node *cur=buckets[bucket];cur;cur=cur->next)
				result+=1;
			return result;
		}

		pair<iterator,bool> insert_unique(const value_type &obj){
			resize(num_elements+1);
			return insert_unique_noresize(obj);
		}
		iterator insert_equal(const value_type &obj){
			resize(num_elements+1);
			return insert_equal_noresize(obj);
		}
		pair<iterator,bool> insert_unique_noresize(const value_type&);
		iterator insert_equal_noresize(const value_type&);

		template<class InputIter>
		void insert_unique(InputIter first,InputIter last){
			insert_unique(first,last,iterator_category(first));
		}
		template<class InputIter>
		void insert_unique(InputIter first,InputIter last,input_iterator_tag){
			for(;first!=last;first++)
				insert_unique(*first);
		} 
		template<class ForwardIter>
		void insert_unique(ForwardIter f,ForwardIter l,forward_iterator_tag){
			size_type n=0;
			distance(f,l,n);
			resize(num_elements+n);
			for(;n>0;--n,++f)
				insert_unique_noresize(*f);
		}

		template<class InputIter>
		void insert_equal(InputIter first,InputIter last){
			insert_equal(first,last,iterator_category(first));
		}
		template<class InputIter>
		void insert_equal(InputIter first,InputIter last,input_iterator_tag){
			for(;first!=last;first++)
				insert_equal(*first);
		}
		template<class ForwardIter>
		void insert_equal(ForwardIter f,ForwardIter l,forward_iterator_tag){
			size_type n=0;
			distance(f,l,n);
			resize(num_elements+n);
			for(;n>0;--n,++f)
				insert_equal_noresize(*f);
		}
		reference find_or_insert(const value_type&);

		iterator find(const key_type &k){
			size_type n=bkt_num_key(k);
			node *first;
			for(first=buckets[n];first&&!equals(get_key(first->val),k);first=first->next)
				;
			return iterator(first,this);
		}
		const_iterator find(const key_type &k)const{
			size_type n=bkt_num_key(k);
			const node *first;
			for(first=buckets[n];first&&!equals(get_key(first->val),k);first=first->next)
				;
			return const_iterator(first,this);
		}

		size_type count(const key_type &k)const{
			const size_type n=bkt_num_key(k);
			size_type result=0;
			for(const node *first=buckets[n];first;first=first->next)
				if(equals(get_key(first->val),k))
					++result;
			return result;
		}

		pair<iterator,iterator> equal_range(const key_type &);
		pair<const_iterator,const_iterator> equal_range(const key_type &)const;

		size_type erase(const key_type &);
		void erase(const iterator &pos);
		void erase(iterator first,iterator last);

		void erase(const const_iterator &pos);
		void erase(const const_iterator first,const const_iterator last);
		void resize(size_type);
		void clear();
	private:
		size_type next_size(size_type n)const{return _next_prime(n);}
		void initialize_buckets(size_type n){
			const size_type n_buckets=next_size(n);
			buckets.reserve(n_buckets);
			buckets.insert(buckets.end(),n_buckets,(node*)0);
			num_elements=0;
		}

		  size_type bkt_num_key( key_type key) const
		  {
		    return bkt_num_key(key, buckets.size());
		  }

		  size_type bkt_num( value_type obj) const
		  {
		    return bkt_num_key(get_key(obj));
		  }

		  size_type bkt_num_key( key_type key, size_t n) const
		  {
		    return hash(key) % n;
		  }

		  size_type bkt_num( value_type obj, size_t n) const
		  {
		    return bkt_num_key(get_key(obj), n);
		  }

		node* new_node(const value_type &obj){
			node *n=node_allocator::allocate();
			n->next=0;
			try{
				construct(&n->val,obj);
				return n;
			}catch(...){
				node_allocator::deallocate(n);throw;
			}
		}

		void delete_node(node *n){
			destroy(&n->val);
			node_allocator::deallocate(n);
		}

		void erase_bucket(size_type ,node *,node*);
		void erase_bucket(size_type,node*);
 		void copy_from(const hashtable &);
	};

	template<class Value,class Key,class Hashfun,class ExtractKey,class EqualKey,class Alloc>
	_hashtable_iterator<Value,  Key,  Hashfun,  ExtractKey,  EqualKey,  Alloc>&
		_hashtable_iterator<Value,  Key,  Hashfun,  ExtractKey,  EqualKey,  Alloc>::operator++(){
			const node* old=cur;
			cur=cur->next;
			if(!cur){
				size_type bucket=ht->bkt_num(old->val);
				while(!cur&&++bucket<ht->buckets.size())
					cur=ht->buckets[bucket];
			}
			return *this;
		}

	template<class Value,class Key,class Hashfun,class ExtractKey,class EqualKey,class Alloc>
	_hashtable_iterator<Value,  Key,  Hashfun,  ExtractKey,  EqualKey,  Alloc>
		_hashtable_iterator<Value,  Key,  Hashfun,  ExtractKey,  EqualKey,  Alloc>::operator++(int){
			iterator tmp=*this;
			++*this;
			return tmp;
		}

	template<class Value,class Key,class Hashfun,class ExtractKey,class EqualKey,class Alloc=alloc>
		bool operator ==(const hashtable<Value,Key,ExtractKey,EqualKey,Alloc> &lhs,
			const hashtable<Value,Key,ExtractKey,EqualKey,Alloc> &rhs){
		typedef typename hashtable<Value,Key,Hashfun,ExtractKey,EqualKey,Alloc>::node node;
		if(lhs.buckets.size()!=rhs.buckets.size())
				return false;
		for(int n=0;n<lhs.buckets.size();++n){
			node* cur1=lhs.buckets[n];
			node *cur2=rhs.buckets[n];
			for(;cur1&&cur2&&cur1->val==cur2->val;cur1=cur1->next,cur2=cur2->next);
			if(cur1||cur2)
				return false;
		}
		return true;
		}

	template<class Value,class Key,class Hashfun,class ExtractKey,class EqualKey,class Alloc>
		void swap(hashtable<Value, Key,  Hashfun,  ExtractKey,  EqualKey> &lhs,
			hashtable<Value, Key,  Hashfun,  ExtractKey,  EqualKey> &rhs){
			lhs.swap(rhs);
		}

	template<class Value,class Key,class Hashfun,class ExtractKey,class EqualKey,class Alloc>
		pair<typename hashtable<Value,Key,Hashfun,ExtractKey,EqualKey,Alloc>::iterator,bool>
		hashtable<Value,Key,Hashfun,ExtractKey,EqualKey,Alloc>::insert_unique_noresize(const value_type &obj){
			const size_type n=bkt_num(obj);
			node *first=buckets[n];
			for(node *curr=first;curr;curr=curr->next)
				if(equals(get_key(obj),get_key(curr->val)))
					return pair<iterator,bool>(iterator(curr,this),false);
			node *tmp=new_node(obj);
			tmp->next=first;
			buckets[n]=tmp;
			++num_elements;
			return pair<iterator,bool>(iterator(tmp,this),true);
		}

	template<class Value,class Key,class Hashfun,class ExtractKey,class EqualKey,class Alloc>
	typename hashtable<Value,Key,Hashfun,ExtractKey,EqualKey,Alloc>::iterator
		hashtable<Value,Key,Hashfun,ExtractKey,EqualKey,Alloc>::insert_equal_noresize(const value_type &obj){
			const size_type n=bkt_num(obj);
			node *first=buckets[n];
			for(node *curr=first;curr;curr=curr->next){
				if(equals(get_key(obj),get_key(curr->val))){
					node *tmp=new_node(obj);
					tmp->next=curr->next;
					curr->next=tmp;
					++num_elements;
					return iterator(tmp,this);
				}
			}
			node *tmp=new_node(obj);
			tmp->next=first;
			buckets[n]=tmp;
			++num_elements;
			return iterator(tmp,this);
		}

	template<class Value,class Key,class Hashfun,class ExtractKey,class EqualKey,class Alloc>
	typename hashtable<Value,Key,Hashfun,ExtractKey,EqualKey,Alloc>::reference
		hashtable<Value,Key,Hashfun,ExtractKey,EqualKey,Alloc>::find_or_insert(const value_type &obj){
			const size_type n=bkt_num(obj);
			node *first=buckets[n];
			for(node *curr=first;curr;curr=curr->next){
				if(equals(get_key(curr->val),get_key(obj)))
					return curr->val;
			}
			node *tmp=new_node(obj);
			tmp->next=first;
			buckets[n]=tmp;
			++num_elements;
			return tmp->val;
		}
	

	template<class Value,class Key,class Hashfun,class ExtractKey,class EqualKey,class Alloc>
	pair<typename hashtable<Value,Key,Hashfun,ExtractKey,EqualKey,Alloc>::iterator,
		typename hashtable<Value,Key,Hashfun,ExtractKey,EqualKey,Alloc>::iterator>	
	hashtable<Value,Key,Hashfun,ExtractKey,EqualKey,Alloc>::equal_range(const key_type &k){
		typedef pair<iterator,iterator> pii;
		const size_type n=bkt_num_key(k);
		for(node *first=buckets[n];first;first=first->next){
			if(equals(k,get_key(first->val))){
				for(node *curr=first->next;curr;curr=curr->next)
					if(!equals(get_key(curr->val),k))
						return pii(iterator(first,this),iterator(curr,this));
				for(size_type m=n+1;m<buckets.size();++m)
					if(buckets[m])
						return pii(iterator(first,this),iterator(buckets[m],this));
				return pii(iterator(first,this),end());
			}
		}
		return pii(end(),end());
	}


	template<class Value,class Key,class Hashfun,class ExtractKey,class EqualKey,class Alloc>
	pair<typename hashtable<Value,Key,Hashfun,ExtractKey,EqualKey,Alloc>::const_iterator,
		typename hashtable<Value,Key,Hashfun,ExtractKey,EqualKey,Alloc>::const_iterator>	
	hashtable<Value,Key,Hashfun,ExtractKey,EqualKey,Alloc>::equal_range(const key_type &k) const{
		typedef pair<const_iterator,const_iterator> pii;
		const size_type n=bkt_num_key(k);
		for(node *first=buckets[n];first;first=first->next){
			if(equals(k,get_key(first->val))){
				for(node *curr=first->next;curr;curr=curr->next)
					if(!equals(get_key(curr->val),k))
						return pii(const_iterator(first,this),const_iterator(curr,this));
				for(size_type m=n+1;m<buckets.size();++m)
					if(buckets[m])
						return pii(const_iterator(first,this),const_iterator(buckets[m],this));
				return pii(const_iterator(first,this),end());
			}
		}
		return pii(end(),end());
	}


	template<class Value,class Key,class Hashfun,class ExtractKey,class EqualKey,class Alloc>
		void hashtable<Value,Key,Hashfun,ExtractKey,EqualKey,Alloc>::resize(size_type num_elems_hint){
			const size_type old_n=buckets.size();
			if(num_elems_hint>old_n){
				const size_type n=next_size(num_elems_hint);
				if(n>old_n){
					vector<node*,Alloc>tempbuf(n,(node*)0);
					try{
						for(size_type bucket=0;bucket<old_n;++bucket){
							node *first=buckets[bucket];
							while(first){
								size_type new_bucket=bkt_num(first->val,n);
								buckets[bucket]=first->next;
								first->next=tempbuf[new_bucket];
								tempbuf[new_bucket]=first;
								first=buckets[bucket];
							}
						}
						buckets.swap(tempbuf);
					}
					catch(...){
						for(size_type bucket;bucket<tempbuf.size();++bucket){
							while(tempbuf[bucket]){
								node *next=tempbuf[bucket]->next;
								delete_node(tempbuf[bucket]);
								tempbuf[bucket]=next;
							}
						}
						throw;
					}
				}
			}
		}

	template<class Value,class Key,class Hashfun,class ExtractKey,class EqualKey,class Alloc>
	void hashtable<Value,Key,Hashfun,ExtractKey,EqualKey,Alloc>::erase_bucket(size_type n,node *last){
		node *curr=buckets[n];
		while(curr!=last){
			node *next=curr->next;
			delete_node(curr);
			curr=next;
			buckets[n]=curr;
			--num_elements;
		}
	}

	template<class Value,class Key,class Hashfun,class ExtractKey,class EqualKey,class Alloc>
	void hashtable<Value,Key,Hashfun,ExtractKey,EqualKey,Alloc>::erase_bucket(size_type n,node *first,node *last){
		node *curr=buckets[n];
		if(curr==first)
			erase(n,last);
		else{
			node *next;
			for(next=curr->next;next!=first;curr=next,next=next->next);
			while(next){
				curr->next=next->next;
				delete_node(next);
				next=curr->next;
				--num_elements;
			}

		}
	}

	template<class Value,class Key,class Hashfun,class ExtractKey,class EqualKey,class Alloc>
	typename hashtable<Value,Key,Hashfun,ExtractKey,EqualKey,Alloc>::size_type
		hashtable<Value,Key,Hashfun,ExtractKey,EqualKey,Alloc>::erase(const key_type &k){
		const size_type n = bkt_num_key(k);
		node* first = buckets[n];
		size_type erased = 0;
		if (first) {
			    node* cur = first;
			    node* next = cur->next;
			    while (next) {
			      if (equals(get_key(next->val), k)) {
			        cur->next = next->next;
			        delete_node(next);
			        next = cur->next;
			        ++erased;
			        --num_elements;
			 }
			else {
			        cur = next;
			        next = cur->next;
			}
		    }
		    if (equals(get_key(first->val), k)) {
		      buckets[n] = first->next;
		      delete_node(first);
		      ++erased;
		      --num_elements;
		    }
		  }
		  return erased;
	}
	
	template<class Value,class Key,class Hashfun,class ExtractKey,class EqualKey,class Alloc>
	void hashtable<Value,Key,Hashfun,ExtractKey,EqualKey,Alloc>::erase(const iterator &it){
		if(node *const p=it.cur){
			const size_type n=bkt_num(p->val);
			node *cur=buckets[n];
			if(cur==p){
				buckets[n]=cur->next;
				delete_node(cur);
				--num_elements;
			}
			else{
				node *next=cur->next;
				while(next){
					if(next==p){
						cur->next=next->next;
						delete_node(next);
						--num_elements;
						break;
					}else{
						cur=next;
						next=cur->next;
					}
				}
			}
		}
	}

	template<class Value,class Key,class Hashfun,class ExtractKey,class EqualKey,class Alloc>
	void hashtable<Value,Key,Hashfun,ExtractKey,EqualKey,Alloc>::erase(iterator first,iterator last){
		size_type f_bucket=first.cur?bkt_num(first.cur->val):buckets.size();
		size_type l_bucket=last.cur?bkt_num(last.cur->val):buckets.size();
		if(first.cur==last.cur)
			return;
		else if(f_bucket==l_bucket)
			erase_bucket(f_bucket,first.cur,last.cur);
		else{
			erase_bucket(f_bucket,first.cur,0);
			for(size_type n=f_bucket+1;n<l_bucket;++n){
				erase_bucket(n,0);
			}
			if(l_bucket!=buckets.size())
				erase_bucket(l_bucket,last.cur);
		}
	}

	template<class Value,class Key,class Hashfun,class ExtractKey,class EqualKey,class Alloc>
	inline void hashtable<Value,Key,Hashfun,ExtractKey,EqualKey,Alloc>::erase(const_iterator first,const_iterator last){
		erase(iterator(const_cast<node*>(first.cur),const_cast<hashtable*>(first.ht)),
			iterator(const_cast<node*>(last.cur),const_cast<hashtable*>(last.ht)));
	}

	template<class Value,class Key,class Hashfun,class ExtractKey,class EqualKey,class Alloc>
	inline void hashtable<Value,Key,Hashfun,ExtractKey,EqualKey,Alloc>::erase(const const_iterator &it){
		erase(iterator(const_cast<node*>(it.cur),const_cast<hashtable*>(it.ht)));
	}
	template<class Value,class Key,class Hashfun,class ExtractKey,class EqualKey,class Alloc>
	void hashtable<Value,Key,Hashfun,ExtractKey,EqualKey,Alloc>::clear(){
		for(size_type i=0;i<buckets.size();++i){
			node *curr=buckets[i];
			while(curr!=0){
				node *nn=curr->next;
				delete_node(curr);
				curr=nn;
			}
			buckets[i]=0;
		}
		num_elements=0;
	}

	template<class Value,class Key,class Hashfun,class ExtractKey,class EqualKey,class Alloc>
	void hashtable<Value,Key,Hashfun,ExtractKey,EqualKey,Alloc>::copy_from(const hashtable &rhs){
		buckets.clear();
		buckets.reserve(rhs.buckets.size());
		buckets.insert(buckets.end(),rhs.buckets.size(),(node*)0);
		try{
			for(size_type i=0;i<rhs.buckets.size();++i){
				if(node const*cur=rhs.buckets[i]){
					node *copy=new_node(cur->val);
					buckets[i]=copy;
					for(node *nn=cur->next;nn;cur=nn,nn=cur->next){
						copy->next=new_node(nn->val);
						copy=copy->next;
					}
				}
			}
			num_elements=rhs.num_elements;
		}catch(...){
			clear();throw;
		}
	}

	template<class Value,class Key,class Hashfun,class ExtractKey,class EqualKey,class Alloc>
	void hashtable<Value,Key,Hashfun,ExtractKey,EqualKey,Alloc>::print_inside(ostream &os){
		for(size_type n=0;n<buckets.size();++n){
			if(buckets[n])
				os<<"bucket "<<n<<": ";
			// else 
			// 	os<<"bucket "<<n<<" empty";
			if(buckets[n]){
				for(node *cur=buckets[n];cur;cur=cur->next){
					os<<cur->val<<" ";
				}
				os<<endl;
			}
		}
	}
}

#endif