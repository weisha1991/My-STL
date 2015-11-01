#pragma once
#include "Alloc.h"
#include "Construct.h"
#include "Functional.h"
#include "Algobase.h"
#include "Iterator1.h"
#include "ReverseIterator.h"
#include <stddef.h>
#include "Algo.h"
#include "Utility.h"
#include "Queue.h"
#include <iostream>
namespace My_STL {
	typedef bool _rb_tree_color_type;
	const _rb_tree_color_type _rb_tree_red = false;
	const _rb_tree_color_type _rb_tree_black = true;

	struct _rb_tree_node_base {
		typedef _rb_tree_node_base* base_ptr;
		typedef _rb_tree_color_type color_type;
		color_type color;
		base_ptr parent;
		base_ptr left;
		base_ptr right;
		static base_ptr minimum(base_ptr x) {
			while (x->left != 0)
				x = x->left;
			return x;
		}
		static base_ptr maximum(base_ptr x) {
			while (x->right != 0)
				x = x->right;
			return x;
		}
	};

	template<class T>
	struct _rb_tree_node :public _rb_tree_node_base {
		typedef _rb_tree_node<T>* link_type;
		T value_field;
	};

	struct _rb_tree_iterator_base {
		typedef _rb_tree_node_base::base_ptr base_ptr;
		typedef bidirectional_iterator_tag iterator_category;
		typedef ptrdiff_t difference_type;
		base_ptr node;
		void increment() {
			if (node->right != 0) {
				node = node->right;
				while (node->left != 0)
					node = node->left;
			}
			else {
				base_ptr y = node->parent;
				while (node == y->right) {
					node = y;
					y = node->parent;
				}
				if (node->right != y)
					node = y;
			}
		}
		void decrement() {
			if (node->color == _rb_tree_red &&
				node->parent->parent == node)
				node = node->right;
			else if (node->left != 0) {
				base_ptr y = node->left;
				while (y->right != 0)
					y = y->right;
				node = y;
			}
			else {
				base_ptr y = node->parent;
				while (node == y->left) {
					node = y;
					y = y->parent;
				}
				node = y;
			}
		}
	
	};
	template<class T,class Ref,class Ptr>
	struct _rb_tree_iterator :public _rb_tree_iterator_base {
		typedef T value_type;
		typedef Ref reference;
		typedef Ptr pointer;
		typedef _rb_tree_iterator<T, T&, T*> iterator;
		typedef _rb_tree_iterator<T, const T&, const T*> const_iterator;
		typedef _rb_tree_iterator<T, Ref, Ptr> self;
		typedef _rb_tree_node<T>* link_type;
		
		_rb_tree_iterator(){}
		_rb_tree_iterator(link_type x) { node = x; }
		_rb_tree_iterator(const iterator &x) { node = x.node; }
		reference operator*() { return ((link_type)node)->value_field; }
		pointer operator->() { return &(operator*()); }

		self& operator++() { increment(); return *this; }
		self operator++(int) {
			self tmp = *this;
			increment();
			return tmp;
		}
		self& operator--() { decrement(); return *this; }
		self operator--(int) {
			self tmp = *this;
			decrement();
			return tmp;
		}
	};
	inline bool operator==(const _rb_tree_iterator_base &x,
		const _rb_tree_iterator_base &y) {
		return x.node == y.node;
	}
	inline bool operator!=(const _rb_tree_iterator_base &x, const
		_rb_tree_iterator_base &y) {
		return x.node != y.node;
	}
	
	inline void _rb_tree_rotate_left(_rb_tree_node_base *x, _rb_tree_node_base *&root) {
		_rb_tree_node_base *y = x->right;
		x->right = y->left;
		if (y->left != 0)
			y->left->parent = x;
		y->parent = x->parent;
		if (x == root)
			root = y;
		else if (x == x->parent->left)
			x->parent->left = y;
		else
			x->parent->right = y;
		y->left = x;
		x->parent = y;
	}

	inline void _rb_tree_rotate_right(_rb_tree_node_base *x, _rb_tree_node_base *&root) {
		_rb_tree_node_base *y = x->left;
		x->left = y->right;
		if (y->right != 0)
			y->right->parent = x;
		y->parent = x->parent;
		if (x == root)
			root = y;
		else if (x == x->parent->right)
			x->parent->right = y;
		else
			x->parent->left = y;
		y->right = x;
		x->parent = y;
	}
	
	inline void _rb_tree_reballance(_rb_tree_node_base *x, _rb_tree_node_base *&root) {
		//static int reballace_cnt = 0;
		//std::cout << ++reballace_cnt << std::endl;
		x->color = _rb_tree_red;
		while (x != root&&x->parent->color == _rb_tree_red) {
			if (x->parent == x->parent->parent->left) {
				_rb_tree_node_base *y = x->parent->parent->right;
				if (y&&y->color == _rb_tree_red) {
					x->parent->color = _rb_tree_black;
					y->color = _rb_tree_black;
					x->parent->parent ->color= _rb_tree_red;
					x = x->parent->parent;
				}
				else {
					if (x == x->parent->right) {
						x = x->parent;
						_rb_tree_rotate_left(x, root);
					}
					x->parent->color = _rb_tree_black;
					x->parent->parent->color = _rb_tree_red;
					_rb_tree_rotate_right(x->parent->parent, root);
				}
			}
			else {
				_rb_tree_node_base *y = x->parent->parent->left;
				if (y&&y->color == _rb_tree_red) {
					x->parent->color = _rb_tree_black;
					y->color = _rb_tree_black;
					x->parent->parent->color = _rb_tree_red;
					x = x->parent->parent;
				}
				else {
					if (x->parent->left == x) {
						x = x->parent;
						_rb_tree_rotate_right(x, root);
					}
					x->parent->color = _rb_tree_black;
					x->parent->parent->color = _rb_tree_red;
					_rb_tree_rotate_left(x->parent->parent, root);
				}
			}
		}
		root->color = _rb_tree_black;
	}
	
	//z是要删除的节点
	inline _rb_tree_node_base* _rb_tree_erase_reballance(_rb_tree_node_base *z,
		_rb_tree_node_base *&root, _rb_tree_node_base *leftmost,
		_rb_tree_node_base *rightmost) {
		_rb_tree_node_base* y = z;
		_rb_tree_node_base* x = 0;
		_rb_tree_node_base* x_parent = 0;
		if (y->left == 0)             // z has at most one non-null child. y == z.
			x = y->right;               // x might be null.
		else
			if (y->right == 0)          // z has exactly one non-null child.  y == z.
				x = y->left;              // x is not null.
			else {                      // z has two non-null children.  Set y to
				y = y->right;             //   z's successor.  x might be null.
				while (y->left != 0)
					y = y->left;
				x = y->right;
			}
			if (y != z) {                 // relink y in place of z.  y is z's successor
				z->left->parent = y;
				y->left = z->left;
				if (y != z->right) {
					x_parent = y->parent;
					if (x) x->parent = y->parent;
					y->parent->left = x;      // y must be a left child
					y->right = z->right;
					z->right->parent = y;
				}
				else
					x_parent = y;
				if (root == z)
					root = y;
				else if (z->parent->left == z)
					z->parent->left = y;
				else
					z->parent->right = y;
				y->parent = z->parent;
				My_STL::swap(y->color, z->color);
				y = z;
				// y now points to node to be actually deleted
			}
			else {                        // y == z
				x_parent = y->parent;
				if (x) x->parent = y->parent;
				if (root == z)
					root = x;
				else
					if (z->parent->left == z)
						z->parent->left = x;
					else
						z->parent->right = x;
				if (leftmost == z)
					if (z->right == 0)        // z->left must be null also
						leftmost = z->parent;
				// makes leftmost == header if z == root
					else
						leftmost = _rb_tree_node_base::minimum(x);
				if (rightmost == z)
					if (z->left == 0)         // z->right must be null also
						rightmost = z->parent;
				// makes rightmost == header if z == root
					else                      // x == z->left
						rightmost = _rb_tree_node_base::maximum(x);
			}
			if (y->color != _rb_tree_red) {
				while (x != root && (x == 0 || x->color == _rb_tree_black))
					if (x == x_parent->left) {
						_rb_tree_node_base* w = x_parent->right;
						if (w->color == _rb_tree_red) {
							w->color = _rb_tree_black;
							x_parent->color = _rb_tree_red;
							_rb_tree_rotate_left(x_parent, root);
							w = x_parent->right;
						}
						if ((w->left == 0 || w->left->color == _rb_tree_black) &&
							(w->right == 0 || w->right->color == _rb_tree_black)) {
							w->color = _rb_tree_red;
							x = x_parent;
							x_parent = x_parent->parent;
						}
						else {
							if (w->right == 0 || w->right->color == _rb_tree_black) {
								if (w->left) w->left->color = _rb_tree_black;
								w->color = _rb_tree_red;
								_rb_tree_rotate_right(w, root);
								w = x_parent->right;
							}
							w->color = x_parent->color;
							x_parent->color = _rb_tree_black;
							if (w->right) w->right->color = _rb_tree_black;
							_rb_tree_rotate_left(x_parent, root);
							break;
						}
					}
					else {                  // same as above, with right <-> left.
						_rb_tree_node_base* w = x_parent->left;
						if (w->color == _rb_tree_red) {
							w->color = _rb_tree_black;
							x_parent->color = _rb_tree_red;
							_rb_tree_rotate_right(x_parent, root);
							w = x_parent->left;
						}
						if ((w->right == 0 || w->right->color == _rb_tree_black) &&
							(w->left == 0 || w->left->color == _rb_tree_black)) {
							w->color = _rb_tree_red;
							x = x_parent;
							x_parent = x_parent->parent;
						}
						else {
							if (w->left == 0 || w->left->color == _rb_tree_black) {
								if (w->right) w->right->color = _rb_tree_black;
								w->color = _rb_tree_red;
								_rb_tree_rotate_left(w, root);
								w = x_parent->left;
							}
							w->color = x_parent->color;
							x_parent->color = _rb_tree_black;
							if (w->left) w->left->color = _rb_tree_black;
							_rb_tree_rotate_right(x_parent, root);
							break;
						}
					}
					if (x) x->color = _rb_tree_black;
			}
			return y;
	}

	template<class Key,class Value,class KeyofValue,class Compare,class Alloc=alloc>
	class rb_tree {
	protected:
		typedef void* void_pointer;
		typedef _rb_tree_node_base* base_ptr;
		typedef _rb_tree_node<Value> rb_tree_node;
		typedef simple_alloc<rb_tree_node, Alloc> rb_tree_node_allocator;
		typedef _rb_tree_color_type color_type;
	public:
		typedef Key key_type;
		typedef Value value_type;
		typedef Value* pointer;
		typedef const Value* const_pointer;
		typedef Value& reference;
		typedef const Value& const_reference;
		typedef rb_tree_node* link_type;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
	protected:
		link_type get_node() { return rb_tree_node_allocator::allocate(); }
		void put_node(link_type p) { rb_tree_node_allocator::deallocate(p); }
		link_type create_node(const value_type &val) {
			link_type ptr = get_node();
			try {
				construct(&ptr->value_field, val);
			}
			catch (...) {
				put_node(ptr); throw;
			}
			return ptr;
		}

		link_type clone_node(link_type x) {
			link_type cp = create_node(x->value_field);
			cp->color = x->color;
			cp->left = 0;
			cp->right = 0;
			return cp;
		}
		void destroy_node(link_type ptr) {
			destroy(&ptr->value_field);
			put_node(ptr);
		}
	protected:
		size_type node_cnt;
		link_type header;
		Compare key_compare;
		
		link_type& root()const { return (link_type&)header->parent; }
		link_type& leftmost()const { return(link_type&)header->left; }
		link_type& rightmost()const { return (link_type&)header->right; }

		static link_type& left(link_type x) { return (link_type&)(x->left); }
		static link_type& right(link_type x) { return(link_type&)(x->right); }
		static link_type& parent(link_type x) { return(link_type&)(x->parent); }
		static reference value(link_type x) { return x->value_field; }
		static const Key key(link_type x) { return KeyofValue()(value(x)); }
		static color_type& color(link_type x) { return (color_type&)(x->color); }

		static link_type& left(base_ptr x) { return (link_type&)(x->left); }
		static link_type& right(base_ptr x) { return (link_type&)(x->right); }
		static link_type& parent(base_ptr x) { return (link_type&)(x->parent); }
		static reference value(base_ptr x) { return ((link_type)x)->value_field; }
		static const Key key(base_ptr x) { return KeyofValue()(value(link_type(x))); }//需要注意不要返回局部变量的
		//的引用；
		static color_type& color(base_ptr x) { return (color_type&)(link_type(x)->color); }

		static link_type minimum(link_type x) {
			return (link_type)_rb_tree_node_base::minimum(x);
		}
		static link_type maximum(link_type x) {
			return (link_type)_rb_tree_node_base::maximum(x);
		}
	public:
		typedef _rb_tree_iterator<value_type, reference, pointer> iterator;
		typedef _rb_tree_iterator<value_type, const_reference, const_pointer> const_iterator;
		typedef reverse_iterator<const_iterator> const_reverse_iterator;
		typedef reverse_iterator<iterator> reverse_iterator;
	private:
		iterator _insert(base_ptr _x, base_ptr _y, const value_type &val);
		link_type _copy(link_type p, link_type q);
		void _erase(link_type p);
		void init() {
			header = get_node();
			color(header) = _rb_tree_red;
			root() = 0;
			leftmost() = header;
			rightmost() = header;
		}
	public:
		rb_tree() :node_cnt(0) { init(); }
		rb_tree(const Compare &comp) :node_cnt(0),key_compare(comp) { init(); }
		rb_tree(const rb_tree &rhs) :node_cnt(0),key_compare(rhs.key_compare){
			header = get_node();
			color(header) = _rb_tree_red;
			if (rhs.root() == 0) {
				root() = 0;
				leftmost() = 0;
			}
			else {
				try {
					root() = _copy(rhs.root(), header);
				}
				catch (...) {
					put_node(header); throw;
				}
				leftmost() = minimum(root());
				rightmost() = maximum(root());
			}
			node_cnt = rhs.node_cnt;
		}
		~rb_tree() {
			clear();
			put_node(header);
		}
		rb_tree& operator=(const rb_tree &rhs);
	public:
		Compare key_comp()const { return key_compare; }
		iterator begin() { return leftmost(); }
		const_iterator begin()const { return leftmost(); }
		iterator end() { return header; }
		const_iterator end()const { return header; }
		reverse_iterator rbegin() { return reverse_iterator(end()); }
		const_reverse_iterator rbegin() const{ return const_reverse_iterator(end()); }
		reverse_iterator rend() { return reverse_iterator(begin()); }
		const_reverse_iterator rend()const { return const_reverse_iterator(begin()); }

		bool empty()const { return node_cnt == 0; }
		size_type size()const { return node_cnt; }
		size_type max_size()const { return size_type(-1); }
		void swap(rb_tree &rhs) {
			My_STL::swap(header, rhs.header);
			My_STL::swap(node_cnt, rhs.node_cnt);
			My_STL::swap(key_compare, rhs.key_compare);
		}
		void print_tree(std::ostream &os);
	public:
		pair<iterator, bool> insert_unique(const value_type&);
		iterator insert_equal(const value_type &val);
		iterator insert_unique(iterator pos, const value_type &val);
		iterator insert_equal(iterator pos, const value_type &val);
		template<class InputIter>
		void insert_unique(InputIter first, InputIter last);
		template<class InputIter>
		void insert_equal(InputIter first, InputIter last);
		
		void erase(iterator pos);
		size_type erase(const key_type &x);
		void erase(iterator first, iterator last);
		void erase( Key first,  Key last);

		void clear() {
			if (node_cnt != 0) {
				_erase(root());
				leftmost ()= 0;
				rightmost()= 0;
				root() = 0;
				node_cnt = 0;
			}
		}
	public:
		iterator find(const Key &x);
		const_iterator find(const Key &x)const;
		size_type count(const Key &x)const;
		iterator lower_bound(const Key &x);
		const_iterator lower_bound(const Key &x)const;
		iterator upper_bound(const Key &x);
		const_iterator upper_bound(const Key &x)const;
		pair<iterator, iterator> equal_range(const Key &x);
		pair<const_iterator, const_iterator> equal_range(const Key &x)const;
	public:
		bool _rb_tree_verify()const;
	};

	//x is the new added node and y is the parent of x;val is the value to add.
	template<class Key,class Value,class KeyofValue,class Compare,class Alloc>
	typename rb_tree<Key,Value,KeyofValue,Compare,Alloc>::iterator 
		rb_tree<Key, Value, KeyofValue, Compare, Alloc>::_insert(base_ptr _x, base_ptr _y, 
			const value_type &val) {
		link_type x = (link_type)_x;
		link_type y = (link_type)_y;
		link_type z;
		if (y == header || x != 0 || key_compare(KeyofValue()(val), key(y))) {
			z = create_node(val);
			left(y) = z;
			if (y == header) {
				root() = z;
				rightmost() = z;
			}
			else if(y==leftmost()) {
				leftmost() = z;
			}
		}
		else {
			z = create_node(val);
			right(y) = z;
			if (y == rightmost())
				rightmost() = z;
		}
		parent(z) = y;
		left(z) = 0;
		right(z) = 0;
		_rb_tree_reballance(z, header->parent);
		++node_cnt;
		return iterator(z);
	}
	//将以x为根的子树复制到p节点之下；尾递归操作。
	template<class Key, class Value, class KeyofValue, class Compare, class Alloc>
	typename rb_tree<Key, Value, KeyofValue, Compare, Alloc>::link_type
		rb_tree<Key, Value, KeyofValue, Compare, Alloc>::_copy(link_type x, link_type p) {
		link_type top = clone_node(x);
		top->parent = p;
		try {
			if (x->right)
				top->right = _copy(right(x), top);
			p = top;
			x = left(x);
			while (x != 0) {
				link_type y = clone_node(x);
				p->left = y;
				y->parent = p;
				if (x->right)
					y->right = _copy(right(x), y);
				p = y;
				x = left(x);
			}
		}
		catch (...) {
			_erase(top); throw;
		}
		return top;
	}

	//尾递归，将以p的右子树节点删除，再删除p节点，最后删除左子树
	template<class Key, class Value, class KeyofValue, class Compare, class Alloc>
	void rb_tree<Key,Value,KeyofValue,Compare,Alloc>::_erase(link_type p) {
		while (p != 0) {
			_erase(right(p));
			link_type y = left(p);
			destroy_node(p);
			p = y;
		}
	}

	template<class Key, class Value, class KeyofValue, class Compare, class Alloc>
	void rb_tree<Key,Value,KeyofValue,Compare,Alloc>::print_tree(std::ostream &os) {
		link_type p;
		queue<link_type> Q;
		Q.push(root());
		while (!Q.empty()) {
			p = Q.front();
			Q.pop();
			std::cout <<( (p==root())?"root node:":"node:") <<value(p)<< ",color:" 
				<< ((color(p)==true)?"black":"red") << "\t";
			if (left(p)) {
				std::cout << "left: " << value(left(p)) << ",color:" <<(color(left(p))==true?"black":"red")
					<< "\t";
				Q.push(left(p));
			}
			if(right(p)){
				std::cout << "right:" << value(right(p)) << ",color:" << (color(right(p)) == true ? "black" : "red");
				Q.push(right(p));
			}
			cout << endl;
		}
	}

	template<class Key, class Value, class KeyofValue, class Compare, class Alloc>
	typename rb_tree<Key, Value, KeyofValue, Compare, Alloc>::iterator
		rb_tree<Key, Value, KeyofValue, Compare, Alloc>::insert_equal(const value_type &val) {
		link_type y = header;
		link_type x = root();
		while (x != 0) {
			y = x;
			x = (key_compare(KeyofValue()(val), key(x)) )?  left(x) : right(x);
		}
		return _insert(x, y, val);
	}

	template<class Key, class Value, class KeyofValue, class Compare, class Alloc>
	pair<typename rb_tree<Key, Value, KeyofValue, Compare, Alloc>::iterator, bool>
		rb_tree<Key, Value, KeyofValue, Compare, Alloc>::insert_unique(const value_type &val) {
		link_type y = header;
		link_type x = root();
		bool comp = true;
		while (x != 0) {
			y = x;
			comp = key_compare(KeyofValue()(val), key(x));
			x = comp ? left(x) : right(x);
		}

		iterator j = iterator(y);
		if (comp) {
			if (j == begin())
				return pair<iterator, bool>(_insert(x, y, val), true);
			else
				--j;
		}
		if (key_compare(key(j.node), KeyofValue()(val))) {
			return pair<iterator, bool>(_insert(x, y, val), true);
		}
		return pair<iterator, bool>(j, false);
	}

	template<class Key,class Value,class KeyofValue,class Compare,class Alloc>
	typename rb_tree<Key,Value,KeyofValue,Compare,Alloc>::iterator
		rb_tree<Key, Value, KeyofValue, Compare, Alloc>::insert_unique(iterator pos, const value_type &val) {
		if (pos.node == header->left) {
			if (size() > 0 && key_compare(KeyofValue()(val), key(pos.node)))
				return _insert(pos.node, pos.node, val);
			else
				return (insert_unique(val).first);
		}
		else if (pos.node == header) {
			if (key_compare(key(rightmost()), KeyofValue()(val)))
				return _insert(0, rightmost(), val);
			else
				return insert_unique(val).first;
		}
		else {
			iterator before = pos;
			--pos;
			if (key_compare(key(before.node), KeyofValue()(val)) &&
				key_compare(KeyofValue()(val), key(pos.node)))
				if (right(before.node) == 0)
					return _insert(0, before.node, val);
				else
					return _insert(pos.node, pos.node, val);
			else
				return insert_unique(val).first;
		}
	}

	template <class Key, class Val, class KeyOfValue, class Compare, class Alloc>
	typename rb_tree<Key, Val, KeyOfValue, Compare, Alloc>::iterator
		rb_tree<Key, Val, KeyOfValue, Compare, Alloc>::insert_equal(iterator position,
			const value_type& v) {
		if (position.node == header->left) // begin()
			if (size() > 0 && key_compare(KeyOfValue()(v), key(position.node)))
				return _insert(position.node, position.node, v);
		// first argument just needs to be non-null 
			else
				return insert_equal(v);
		else if (position.node == header) // end()
			if (!key_compare(KeyOfValue()(v), key(rightmost())))
				return _insert(0, rightmost(), v);
			else
				return insert_equal(v);
		else {
			iterator before = position;
			--before;
			if (!key_compare(KeyOfValue()(v), key(before.node))
				&& !key_compare(key(position.node), KeyOfValue()(v)))
				if (right(before.node) == 0)
					return _insert(0, before.node, v);
				else
					return _insert(position.node, position.node, v);
			// first argument just needs to be non-null 
			else
				return insert_equal(v);
		}
	}

	template <class Key, class Val, class KeyOfValue, class Compare, class Alloc>
	template<class InputIter>
	void rb_tree<Key, Val, KeyOfValue, Compare, Alloc>::insert_unique(InputIter first,
		InputIter last) {
		for (; first != last; ++first)
			insert_unique(*first);

	}

	template <class Key, class Val, class KeyOfValue, class Compare, class Alloc>
	template<class InputIter>
	void rb_tree<Key, Val, KeyOfValue, Compare, Alloc>::insert_equal(InputIter first,
		InputIter last) {
		for (; first != last; ++first)
			insert_equal(*first);
	}


	template <class Key, class Val, class KeyOfValue, class Compare, class Alloc>
	void rb_tree<Key, Val, KeyOfValue, Compare, Alloc>::erase(iterator pos) {
		//std::cout << "ccc" << std::endl;
		link_type y = (link_type)_rb_tree_erase_reballance(pos.node, header->parent,
			header->left, header->right);
		destroy_node(y);
		--node_cnt;
	}

	template <class Key, class Val, class KeyOfValue, class Compare, class Alloc>
	typename rb_tree<Key, Val, KeyOfValue, Compare, Alloc>::size_type
		rb_tree<Key, Val, KeyOfValue, Compare, Alloc>::erase(const key_type &k) {
		pair<iterator, iterator> p = equal_range(k);
		size_type n = 0;
		distance(p.first, p.second, n);
		erase(p.first, p.second);
		return n;
	}

	template <class Key, class Val, class KeyOfValue, class Compare, class Alloc>
	void rb_tree<Key, Val, KeyOfValue, Compare, Alloc>::erase(iterator first, iterator last) {
		if (first == begin() && last == end())
			clear();
		else
			while (first != last)
				erase(first++);
	}

	template <class Key, class Val, class KeyOfValue, class Compare, class Alloc>
	void rb_tree<Key, Val, KeyOfValue, Compare, Alloc>::erase( Key first,
		 Key last) {
		while (first != last) {
			{	erase(first);
				++first;
			}
		}
	}

	template <class Key, class Val, class KeyOfValue, class Compare, class Alloc>
	typename rb_tree<Key,Val, KeyOfValue,Compare,Alloc>::iterator 
		rb_tree<Key, Val, KeyOfValue, Compare, Alloc>::find(const Key &k) {
		link_type y = header;
		link_type x = root();
		while (x != 0) {
			if (!key_compare(key(x), k))
				y = x, x = left(x);
			else
				x = right(x);
		}
		iterator j = iterator(y);
		return ((j == end() || key_compare(k, key(j.node)))) ? end() : j;
	}

	template <class Key, class Val, class KeyOfValue, class Compare, class Alloc>
	typename rb_tree<Key, Val, KeyOfValue, Compare, Alloc>::const_iterator
		rb_tree<Key, Val, KeyOfValue, Compare, Alloc>::find(const Key &k)const {
		link_type y = header;
		link_type x = root();
		while (x != 0) {
			if (!key_compare(key(x), k))
				y = x, x = left(x);
			else
				x = right(x);
		}
		const_iterator j = const_iterator(y);
		return ((j == end() || key_compare(k, key(j.node))) )? end() : j;

	}
	template <class Key, class Val, class KeyOfValue, class Compare, class Alloc>
	typename rb_tree<Key,Val,KeyOfValue,Compare,Alloc>::size_type 
		rb_tree<Key, Val, KeyOfValue, Compare, Alloc>::count(const Key &k)const {
		pair<const_iterator, const_iterator> p = equal_range(k);		
		size_type n=0;
		distance(p.first, p.second, n);
		cout <<"inside"<< n << endl;
		return n;
	}

	template <class Key, class Val, class KeyOfValue, class Compare, class Alloc>
	typename rb_tree<Key, Val, KeyOfValue, Compare, Alloc>::iterator
		rb_tree<Key, Val, KeyOfValue, Compare, Alloc>::lower_bound(const Key &k) {
		link_type y = header;
		link_type x = root();
		while (x != 0) {
			if (!key_compare(key(x), k))
				y = x, x = left(x);
			else
				x = right(x);
		}
		return iterator(y);
	}

	template <class Key, class Val, class KeyOfValue, class Compare, class Alloc>
	typename rb_tree<Key, Val, KeyOfValue, Compare, Alloc>::const_iterator
		rb_tree<Key, Val, KeyOfValue, Compare, Alloc>::lower_bound(const Key &k)const {
		link_type y = header;
		link_type x = root();
		while (x != 0) {
			if (!key_compare(key(x), k))
				y = x, x = left(x);
			else
				x = right(x);
		}
		return const_iterator(y);

	}

	template <class Key, class Val, class KeyOfValue, class Compare, class Alloc>
	typename rb_tree<Key, Val, KeyOfValue, Compare, Alloc>::iterator
		rb_tree<Key, Val, KeyOfValue, Compare, Alloc>::upper_bound(const Key &k) {
		link_type y = header;
		link_type x = root();
		while (x != 0) {
			if (key_compare(k, key(x)))
				y = x, x = left(x);
			else
				x = right(x);
		}
		return iterator(y);
	}

	template <class Key, class Val, class KeyOfValue, class Compare, class Alloc>
	typename rb_tree<Key, Val, KeyOfValue, Compare, Alloc>::const_iterator
		rb_tree<Key, Val, KeyOfValue, Compare, Alloc>::upper_bound(const Key &k)const {
		link_type y = header;
		link_type x = root();
		while (x != 0) {
			if (key_compare(k, key(x)))
				y = x, x = left(x);
			else
				x = right(x);
		}
		return const_iterator(y);
	}

	template <class Key, class Val, class KeyOfValue, class Compare, class Alloc>
	pair<typename rb_tree<Key, Val, KeyOfValue, Compare, Alloc>::iterator,
		typename rb_tree<Key, Val, KeyOfValue, Compare, Alloc>::iterator>
		rb_tree<Key, Val, KeyOfValue, Compare, Alloc>::equal_range(const Key &k) {
		return pair<iterator, iterator>(lower_bound(k), upper_bound(k));
	}

	template <class Key, class Val, class KeyOfValue, class Compare, class Alloc>
	pair<typename rb_tree<Key,Val,KeyOfValue,Compare,Alloc>::const_iterator, 
		typename rb_tree<Key, Val, KeyOfValue, Compare, Alloc>::const_iterator>
		rb_tree<Key, Val, KeyOfValue, Compare, Alloc>::equal_range(const Key &x)const {
		return pair<const_iterator, const_iterator>(lower_bound(x), upper_bound(x));
	}

	template <class Key, class Val, class KeyOfValue, class Compare, class Alloc>
	rb_tree<Key,Val,KeyOfValue,Compare,Alloc>& 
		rb_tree<Key, Val, KeyOfValue, Compare, Alloc>::operator=(const rb_tree &rhs) {
		if (this != &rhs) {
			clear();
			node_cnt = 0;
			key_compare = rhs.key_compare;
			if (rhs.root() == 0) {
				root() = 0;
				leftmost() = 0;
				rightmost() = 0;
			}
			else {
				root() = _copy(rhs.root(), header);
				leftmost() = minimum(root());
				rightmost() = maximum(root());
				node_cnt = rhs.node_cnt;
			}
		}
		return *this;
	}
	
	/*
		从node节点开始上溯到根节点的路径上所含有的黑节点数目
	*/
	inline int _black_cnt(_rb_tree_node_base *node,_rb_tree_node_base *root) {
		if (node == 0)
			return 0;
		else {
			int bc = (node->color == _rb_tree_black) ? 1 : 0;
			if (node == root)
				return bc;
			else
				return bc + _black_cnt(node->parent, root);
		}
	}

	/*
		验证rb_tree的合法性；空的rb_tree也是红黑树；

	*/
	template <class Key, class Val, class KeyOfValue, class Compare, class Alloc>
	bool rb_tree<Key, Val, KeyOfValue, Compare, Alloc>::_rb_tree_verify()const {
		if (node_cnt == 0 && begin() == end()) {
			return (node_cnt == 0) && begin() == end() &&
				header->left == header&&header->right == header;
		}
		int len = _black_cnt(leftmost(), root());
		for (const_iterator iter = begin(); iter != end(); ++iter) {//中序遍历
			link_type x = (link_type)iter.node;
			link_type l = left(x);
			link_type r = right(x);
			
			if (color(x) == _rb_tree_red)//检验是否有连续的红色父子节点
				if ((l&&color(l) == _rb_tree_red) || (r&&color(r) == _rb_tree_red))
					return false;
			if (l&&key_compare(key(x), key(l)))//二叉树性质检测
				return false;
			if (r&&key_compare(key(r), key(x)))//同上
				return false;
			if (!l&&!r&&_black_cnt(x, root()) != len)//遇到叶子节点，计算到根的黑色节点数
				return false;
		}
		if (leftmost() != _rb_tree_node_base::minimum(root()))
			return false;
		if (rightmost() != _rb_tree_node_base::maximum(root()))
			return false;
		return true;
	}

}
