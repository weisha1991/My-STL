#ifndef HASHFUN_H
#define HASHFUN_H
#include <stddef.h>

namespace My_STL{
	template<typename Key> struct hash{};
	inline size_t _hash_string(const char *s){
		unsigned long h=0;
		for(;*s;++s){
			h=5*h+*s;
		}
		return h;
	}

	template<>
	struct hash<char*>{
		size_t operator()(const char *s){return _hash_string(s);}
	};
	
	template<> struct hash<const char*>{
		size_t operator()(const char *s){return _hash_string(s);}
	};

	template<>struct hash<char>{
		size_t operator()(char x){return x;}
	};
	template<> struct hash<unsigned char>
	{
		size_t operator()(unsigned char x){return x;}
	};

	template<> struct hash<signed char>{
		size_t operator()(signed char x){return x;}
	};

	template<> struct hash<short> {
	 	 size_t operator()(short x) const { return x; }
	};
	template<> struct hash<unsigned short> {
  	size_t operator()(unsigned short x) const { return x; }
	};

	template<> struct hash<int> {
	  size_t operator()(int x) const { return x; }
	};
	template<> struct hash<unsigned int> {
	  size_t operator()(unsigned int x) const { return x; }
	};
	template<> struct hash<long> {
	  size_t operator()(long x) const { return x; }
	};
	template<> struct hash<unsigned long> {
	  size_t operator()(unsigned long x) const { return x; }
	};

}
#endif