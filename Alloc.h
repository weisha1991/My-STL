#ifndef _ALLOC_H_
#define _ALLOC_H_

#include <cstdlib>

namespace My_STL{
	//template<int inst>sgistl源码中这个没用,关于线程安全也忽略了
	class alloc{
	private:
		enum EAlign{ ALIGN = 8};//小型区块的上调边界
		enum EMaxBytes{ MAXBYTES = 128};//小型区块的上限，超过的区块由malloc分配
		enum ENFreeLists{ NFREELISTS = (MAXBYTES / ALIGN)};//free-lists的个数
		enum ENObjs{ NOBJS = 20};//每次增加的节点数
	private:
		//free-lists的节点构造
		union obj{
			union obj *next;
			char client[1];//一个字节数据
		};

		static obj *free_list[NFREELISTS];
	private:
		static char *start_free;//内存池起始位置
		static char *end_free;//内存池结束位置
		static size_t heap_size;//
	private:
		//将bytes上调至8的倍数
		static size_t ROUND_UP(size_t bytes){
			return ((bytes + ALIGN - 1) & ~(ALIGN - 1));
		}
		//根据区块大小，决定使用第n号free-list，n从0开始计算
		static size_t FREELIST_INDEX(size_t bytes){
			return (((bytes)+ALIGN - 1) / ALIGN - 1);
		}
		//返回一个大小为n的对象，并可能加入大小为n的其他区块到free-list
		static void *refill(size_t n);
		//配置一大块空间，可容纳nobjs个大小为size的区块
		//如果配置nobjs个区块有所不便，nobjs可能会降低
		static char *chunk_alloc(size_t size, size_t& nobjs);

	public:
		static void *allocate(size_t bytes);
		static void deallocate(void *ptr, size_t bytes);
		static void *reallocate(void *ptr, size_t old_sz, size_t new_sz);
	};

	template<class T, class Alloc>
	class simple_alloc {
	public:
		static T *allocate(size_t n)
		{
			return 0 == n ? 0 : (T*)Alloc::allocate(n * sizeof(T));
		}
		static T *allocate(void)
		{
			return (T*)Alloc::allocate(sizeof(T));
		}
		static void deallocate(T *p, size_t n)
		{
			if (0 != n) Alloc::deallocate(p, n * sizeof(T));
		}
		static void deallocate(T *p)
		{
			Alloc::deallocate(p, sizeof(T));
		}
	};
}

#endif