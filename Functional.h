#ifndef FUNCTIONAL_H
#define FUNCTIONAL_H

namespace My_STL {

	template<class Arg, class Result>
	struct unary_function{
		typedef Arg argmument_type;
		typedef Result result_type;
	}; 
	
	template<class Arg1,class Arg2,class Result>
	struct binary_function{
		typedef Arg1 first_argument_type;
		typedef Arg2 second_argument_type;
		typedef Result result_type;
	};

	template<class T>
	struct plus :public binary_function<T,T,T>{
		T operator()(const T &x,const T &y)const{
			return x + y;
		}
	};

	template<class T>
	struct minus :public binary_function<T, T, T>{
		T operator()(const T &x, const T &y)const{
			return x - y;
		}
	};

	template<class T>
	struct multiplies :public binary_function<T, T, T>{
		T operator()(const T &x, const T &y)const{
			return x*y;
		}
	};

	template<typename T>
	struct divides :public binary_function<T, T, T>{
		T operator()(const T &x, const T&y)const{
			return x / y;
		}
	};

	template<typename T> inline T identity_element(plus<T>){ return T(0); }
	template<typename T> inline T identity_element(multiplies<T>){ return T(1); }

	template<typename T>
	struct modulus :public binary_function<T, T, T>{
		T operator()(const T &x, const T &y)const{
			return x%y;
		}
	};

	template<typename T>
	struct negate :public unary_function<T, T>{
		T operator()(const T &x)const{
			return -x;
		}
	};

	template<typename T>
	struct equal_to :public binary_function<T, T, bool>{
		bool operator()(const T &x, const T &y)const{
			return x == y;
		}
	};

	template<typename T>
	struct not_equal_to :public binary_function<T, T, bool>{
		T  operator()(const T &x, const T &y)const{
			return x != y;
		}
	};

	template <class T>
	struct greater : public binary_function<T, T, bool> {
		bool operator()(const T& x, const T& y) const { return x > y; }
	};

	template <class T>
	struct less : public binary_function<T, T, bool> {
		bool operator()(const T& x, const T& y) const { return x < y; }
	};

	template <class T>
	struct greater_equal : public binary_function<T, T, bool> {
		bool operator()(const T& x, const T& y) const { return x >= y; }
	};

	template <class T>
	struct less_equal : public binary_function<T, T, bool> {
		bool operator()(const T& x, const T& y) const { return x <= y; }
	};

	template <class T>
	struct logical_and : public binary_function<T, T, bool> {
		bool operator()(const T& x, const T& y) const { return x && y; }
	};

	template <class T>
	struct logical_or : public binary_function<T, T, bool> {
		bool operator()(const T& x, const T& y) const { return x || y; }
	};

	template <class T>
	struct logical_not : public unary_function<T, bool> {
		bool operator()(const T& x) const { return !x; }
	};
	
	template<class Predicate>
	class unary_negate :public unary_function<typename Predicate::argument_type, bool>{
	protected:
		Predicate pred;
	public:
		explicit unary_negate(const Predicate &x) :pred(x){}
		bool operator()(const typename Predicate::argument_type &x)const{
			return !pred(x);
		}
	};

	template<class Predicate>
	inline unary_negate<Predicate> not1(const Predicate &pred){
		return unary_negate<Predicate>(pred);
	}

	template<class Predicate>
	class binary_negate :public binary_function<typename Predicate::first_argument_type, 
								typename Predicate::second_argument_type,bool >{
	protected:
		Predicate pred;
	public:
		explicit binary_negate(const Predicate &x) :pred(x){}
		bool operator()(const typename Predicate::first_argument_type &x,
			const typename Predicate::second_argument_type &y)
		{
			return !pred(x, y);
		}
	};

	template<class Predicate>
	inline binary_negate<Predicate> not2(const Predicate &pred){
		return binary_negate<Predicate>(pred);
	}

	/*��һ����������*/
	template<class Operation>
	class binder1st :
		public unary_function <typename Operation::second_argument_type, typename Operation::result_type>{
	protected:
		Operation op;
		typename Operation::first_argument_type value;
	public:
		binder1st(const Operation &o, const typename Operation::first_argument_type &val) :op(o), value(val){}
		typename Operation::result_type 
			operator()(const typename Operation::second_argument_type &x)const{
				return op(value, x);
			}
	};

	template<class Operation,class T>
	inline binder1st<Operation>
		bind1st(const Operation &op, const T &x){
			typedef typename Operation::first_argument_type arg1_type;
			return binder1st<Operation>(op, arg1_type(x));
		}

	template<class Operation>
	class binder2nd :
		public unary_function<typename Operation::first_argument_type, typename Operation::result_type>{
	protected:
		Operation op;
		typename Operation::second_argument_type value;
	public:
		binder2nd(const Operation &o, const typename Operation::second_argument_type &x) :op(o), value(x){}
		typename Operation::result_type 
			operator()(const typename Operation::first_argument_type &x)const{
				return op(x, value);
			}
	};

	template<class Operation,class T>
	inline binder2nd<Operation>
		bind2nd(const Operation &op, const T &x){
			typedef typename Operation::second_argument_type arg2_type;
			return binder2nd<Operation>(op, arg2_type(x));
		}

	
	//template<class Operation1,class Operation2>
	//class unary_compose:public unary_function<typename Operation2::argument_type, typename Operation1::result_type>{
	//protected:
	//	Operaiton1 op1;
	//	Operation2 op2;
	//public:
	//	unary_compose(const Operation1 &o1, const Operation2 &o2) :op1(o1), op2(o2){}
	//	typename Operation1::result_type
	//		operator()(const typename Operation2::argument_type &x)const
	//	{
	//			return op1(op2(x));
	//	}
	//};

	///*���غ������*/
	//template<class Operation1,class Operation2,class T>
	//inline unary_compose<Operation1,Operation2>
	//	compose1(const Operation1 &op1, const Operation2 &op2){
	//		return unary_compose<Operation1, Operation2>(op1, op2);
	//	}

	template <class Operation1, class Operation2, class Operation3>
	class binary_compose
		: public unary_function<typename Operation2::argument_type,
		typename Operation1::result_type> {
	protected:
		Operation1 op1;
		Operation2 op2;
		Operation3 op3;
	public:
		binary_compose(const Operation1& x, const Operation2& y,
			const Operation3& z) : op1(x), op2(y), op3(z) { }
		typename Operation1::result_type
			operator()(const typename Operation2::argument_type& x) const {
				return op1(op2(x), op3(x));
			}
	};

	template <class Operation1, class Operation2, class Operation3>
	inline binary_compose<Operation1, Operation2, Operation3>
		compose2(const Operation1& op1, const Operation2& op2, const Operation3& op3) {
			return binary_compose<Operation1, Operation2, Operation3>(op1, op2, op3);
		}

	template<class Arg,class Result>
	class pointer_to_unary_function :public unary_function<Arg,Result>{
	protected:
		Result(*ptr)(Arg);
	public:
		pointer_to_unary_function(){}
		explicit pointer_to_unary_function(Result(*x)(Arg)) :ptr(x){}
		Result operator()(const Arg &x)const{
			return ptr(x);
		}
	};

	template<class Arg,class Result>
	inline pointer_to_unary_function<Arg,Result>
		ptr_fun(Result(*x)(Arg)){
			return pointer_to_unary_function<Arg, Result>(x);
		}

	template<class Arg1,class Arg2,class Result>
	class pointer_binary_function :public binary_function<Arg1, Arg2, Result>{
	protected:
		Result(*ptr)(Arg1, Arg2);
	public:
		pointer_binary_function(){}
		explicit pointer_binary_function(Result(*x)(Arg1, Arg2)) :ptr(x){}
		Result operator()(const Arg1 &x, const Arg2 &y)const{
			return ptr(x, y);
		}
	};

	template<class Arg1,class Arg2,class Result>
	inline pointer_binary_function<Arg1,Arg2,Result>
		ptr_func(Result(*func)(Arg1, Arg2)){
			return pointer_binary_function<Arg1, Arg2, Result>(func);
		}

	template<class T>
	struct identity :public unary_function<T, T>{
		const T& operator()(const T &x)const{ return x; }
	};

	template<class Pair>
	struct select1st :public unary_function<Pair, typename Pair::first_type>{
		const typename Pair::first_type operator()(const Pair &x)const{
			return x.first;
		}
	};

	template<class Pair>
	struct select2nd :public unary_function<Pair, typename Pair::second_type>{
		const typename Pair::second_type operator()(const Pair &x)const{
			return x.second;
		}
	};

	template<class Arg1,class Arg2>
	struct project1st:public binary_function<Arg1,Arg2,Arg2>{
		Arg1 operator()(const Arg1 &x, const Arg2&)const{
			return x;
		}
	};

	template<class Arg1,class Arg2>
	struct project2nd :public binary_function<Arg1, Arg2, Arg2>{
		Arg2 operator()(const Arg1&, const Arg2 &x)const{
			return x;
		}
	};

	template<class S,class T>
	class mem_fun_t :public unary_function<T*, S>{
	public:
		explicit mem_fun_t(S (T::*pf)()) :f(pf){}
		S operator()(T *p)const{
			return (p->*f)();
		}
	private:
		S (T::*f)();
	};

	template <class S, class T>
	class const_mem_fun_t : public unary_function<const T*, S> {
	public:
		explicit const_mem_fun_t(S(T::*pf)() const) : f(pf) {}
		S operator()(const T* p) const { return (p->*f)(); }
	private:
		S(T::*f)() const;
	};


	template <class S, class T>
	class mem_fun_ref_t : public unary_function<T, S> {
	public:
		explicit mem_fun_ref_t(S(T::*pf)()) : f(pf) {}
		S operator()(T& r) const { return (r.*f)(); }
	private:
		S(T::*f)();
	};

	template <class S, class T>
	class const_mem_fun_ref_t : public unary_function<T, S> {
	public:
		explicit const_mem_fun_ref_t(S(T::*pf)() const) : f(pf) {}
		S operator()(const T& r) const { return (r.*f)(); }
	private:
		S(T::*f)() const;
	};

	template <class S, class T, class A>
	class mem_fun1_t : public binary_function<T*, A, S> {
	public:
		explicit mem_fun1_t(S(T::*pf)(A)) : f(pf) {}
		S operator()(T* p, A x) const { return (p->*f)(x); }
	private:
		S(T::*f)(A);
	};

	template <class S, class T, class A>
	class const_mem_fun1_t : public binary_function<const T*, A, S> {
	public:
		explicit const_mem_fun1_t(S(T::*pf)(A) const) : f(pf) {}
		S operator()(const T* p, A x) const { return (p->*f)(x); }
	private:
		S(T::*f)(A) const;
	};

	template <class S, class T, class A>
	class mem_fun1_ref_t : public binary_function<T, A, S> {
	public:
		explicit mem_fun1_ref_t(S(T::*pf)(A)) : f(pf) {}
		S operator()(T& r, A x) const { return (r.*f)(x); }
	private:
		S(T::*f)(A);
	};

	template <class S, class T, class A>
	class const_mem_fun1_ref_t : public binary_function<T, A, S> {
	public:
		explicit const_mem_fun1_ref_t(S(T::*pf)(A) const) : f(pf) {}
		S operator()(const T& r, A x) const { return (r.*f)(x); }
	private:
		S(T::*f)(A) const;
	};


	template <class T>
	class mem_fun_t<void, T> : public unary_function<T*, void>{
	public:
		explicit mem_fun_t(void (T::*pf)()) : f(pf) {}
		void operator()(T* p) const { (p->*f)(); }
	private:
		void (T::*f)();
	};

	template <class T>
	class const_mem_fun_t<void, T> : public unary_function<const T*, void>{
	public:
		explicit const_mem_fun_t(void (T::*pf)() const) : f(pf) {}
		void operator()(const T* p) const { (p->*f)(); }
	private:
		void (T::*f)() const;
	};

	template <class T>
	class mem_fun_ref_t<void, T> : public unary_function<T, void>{
	public:
		explicit mem_fun_ref_t(void (T::*pf)()) : f(pf) {}
		void operator()(T& r) const { (r.*f)(); }
	private:
		void (T::*f)();
	};

	template <class T>
	class const_mem_fun_ref_t<void, T> : public unary_function<T, void>{
	public:
		explicit const_mem_fun_ref_t(void (T::*pf)() const) : f(pf) {}
		void operator()(const T& r) const { (r.*f)(); }
	private:
		void (T::*f)() const;
	};

	template <class T, class A>
	class mem_fun1_t<void, T, A> : public binary_function<T*, A, void>{
	public:
		explicit mem_fun1_t(void (T::*pf)(A)) : f(pf) {}
		void operator()(T* p, A x) const { (p->*f)(x); }
	private:
		void (T::*f)(A);
	};

	template <class T, class A>
	class const_mem_fun1_t<void, T, A> : public binary_function<const T*, A, void>{
	public:
		explicit const_mem_fun1_t(void (T::*pf)(A) const) : f(pf) {}
		void operator()(const T* p, A x) const { (p->*f)(x); }
	private:
		void (T::*f)(A) const;
	};

	template <class T, class A>
	class mem_fun1_ref_t<void, T, A> : public binary_function<T, A, void>{
	public:
		explicit mem_fun1_ref_t(void (T::*pf)(A)) : f(pf) {}
		void operator()(T& r, A x) const { (r.*f)(x); }
	private:
		void (T::*f)(A);
	};

	template <class T, class A>
	class const_mem_fun1_ref_t<void, T, A> : public binary_function<T, A, void>{
	public:
		explicit const_mem_fun1_ref_t(void (T::*pf)(A) const) : f(pf) {}
		void operator()(const T& r, A x) const { (r.*f)(x); }
	private:
		void (T::*f)(A) const;
	};


	template <class S, class T>
	inline mem_fun_t<S, T> mem_fun(S(T::*f)()) {
		return mem_fun_t<S, T>(f);
	}

	template <class S, class T>
	inline const_mem_fun_t<S, T> mem_fun(S(T::*f)() const) {
		return const_mem_fun_t<S, T>(f);
	}

	template <class S, class T>
	inline mem_fun_ref_t<S, T> mem_fun_ref(S(T::*f)()) {
		return mem_fun_ref_t<S, T>(f);
	}

	template <class S, class T>
	inline const_mem_fun_ref_t<S, T> mem_fun_ref(S(T::*f)() const) {
		return const_mem_fun_ref_t<S, T>(f);
	}

	template <class S, class T, class A>
	inline mem_fun1_t<S, T, A> mem_fun1(S(T::*f)(A)) {
		return mem_fun1_t<S, T, A>(f);
	}

	template <class S, class T, class A>
	inline const_mem_fun1_t<S, T, A> mem_fun1(S(T::*f)(A) const) {
		return const_mem_fun1_t<S, T, A>(f);
	}

	template <class S, class T, class A>
	inline mem_fun1_ref_t<S, T, A> mem_fun1_ref(S(T::*f)(A)) {
		return mem_fun1_ref_t<S, T, A>(f);
	}

	template <class S, class T, class A>
	inline const_mem_fun1_ref_t<S, T, A> mem_fun1_ref(S(T::*f)(A) const) {
		return const_mem_fun1_ref_t<S, T, A>(f);
	}
}

#endif
