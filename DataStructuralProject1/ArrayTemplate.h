#include "TypedDefineBegin.h"
#define _type_ T//别名

#define _class_ Array(T)
#define Array(T) _TypedVar_(Array, T)

typedef _class_struct_
{
	T * m_data;            //数组指针
	T* m_data_end;    //容量域末（数组外）
	size_t m_size;       //数量
	size_t m_capacity;//容量
	T* m_end;            //数据域末（数据外）
}*_class_;

//public construct
#define Template ArrayInit
#define ArrayInit_(T, init_capacity, auto_release) _TypedVar_2(ArrayInit, T)(init_capacity, auto_release)
Array(T) _template_(size_t capacity, bool auto_release);

#define ArrayInit_2(T, auto_release) ArrayInit_(T, 4, auto_release)
#define ArrayInit_3(T, init_capacity, auto_release) ArrayInit_(T, init_capacity, auto_release)
#define ArrayInit(T,...) VA_MACRO(ArrayInit, T, __VA_ARGS__)

//public destruct
#define Template ArrayRelease
#define ArrayRelease(T, A) _TypedVar_(ArrayRelease, T)(A)
void _template_(Array(T) A);

//public
#define Template ArrayCapacitySet
#define ArrayCapacitySet(T, A, size) _TypedVar_(ArrayCapacitySet, T)(A, size)
void _template_(Array(T) A, size_t new_size);

/*
* 增删操作
*/
//public
#define Template ArrayPushBack
#define ArrayPushBack(T, A, val) _TypedVar_(ArrayPushBack, T)(A, val)
void _template_(Array(T) A, const T val);

//public
#define Template ArrayPopBack
#define ArrayPopBack(T, A) _TypedVar_(ArrayPopBack, T)(A)
void _template_(Array(T) A);

//public
#define Tempate ArrayEmplaceBack
#define ArrayEmplaceBack(T, A, ...) _TypedVar_(ArrayEmplaceBack, T)(A, __VA_ARGS__)
void _template_(Array(T) A, ...);

//public
#define Template ArrayResize
#define ArrayResize(T, A, new_size) _TypedVar_(ArrayResize, T)(A, new_size)
void _template_(Array(T) A, size_t new_size);

#define Template ArrayClear
#define ArrayClear(T, A) _TypedVar_(ArrayClear, T)(A)
void _template_(Array(T) A);

//public
#define Template ArraySize
#define ArraySize(T, A) _TypedVar_(ArraySize, T)(A)
inline size_t _template_(Array(T) A) { return A->m_size; }

//public
#define Template ArrayEmpty
#define ArrayEmpty(T, A) _TypedVar_(ArrayEmpty, T)(A)
inline bool _template_(Array(T) A) { return A->m_size == 0; }

//public
#define Template ArrayForEach
#define ArrayForEach(T, A, func) _TypedVar_(ArrayForEach, T)(A, func)
void _template_(Array(T) A, Status visit(T* val));

/*
* 访问器
* 返回引用类型
*/
//public
#define Template ArrayGet
#define ArrayGet(T, A, index) (*_TypedVar_(ArrayGet, T)(A, index))
inline T* _template_(Array(T) A, uint64_t index)
{
	CHECK(if (index >= A->m_size)
	{
		ET(OutOfRange, "index out of range", NULL);
	});
	return &A->m_data[index];
}

//public
#define Template ArrayBack
#define ArrayBack(T, A) (*_TypedVar_(ArrayBack, T)(A))
inline T* _template_(Array(T) A) { return &A->m_data[A->m_size - 1]; }

//public
#define Template ArrayFront
#define ArrayFront(T, A) (*_TypedVar_(ArrayFront, T)(A))
inline T* _template_(Array(T) A) { return &A->m_data[0]; }

//迭代器接口
#define Template ArrayBegin
#define ArrayBegin(T, A) _TypedVar_(ArrayBegin, T)(A)
inline T* _template_(Array(T) A) { return A->m_data; }

#define Template ArrayEnd
#define ArrayEnd(T, A) _TypedVar_(ArrayEnd, T)(A)
inline T* _template_(Array(T) A) { return A->m_end; }

#define Template ArrayForward
#define ArrayForward(T, A, it) _TypedVar_(ArrayForward, T)(A, it)
inline T* _template_(Array(T) A, T* it)
{
	CHECK(if (it >= A->m_end)
	{
		ET(OutOfRange, "iterator out of range", NULL);
	});
	return it + 1;
}

#define _class_ LoopArray(T)
#define LoopArray(T) _TypedVar_(LoopArray, T)
typedef _class_struct_
{
	T * m_data;//数组首地址
	T* m_data_end;//数组边界 不在数组内
	size_t m_size;//长度
	size_t m_capacity;//容量 实际大小
	T* m_begin;//数据起始地址
	T* m_end;//数据终止地址//不包括数据
}*_class_;

//public construct fucntion
#define Template LoopArrayInit
#define LoopArrayInit(T, init_capacity, auto_release) _TypedVar_(LoopArrayInit, T)(init_capacity, auto_release)
LoopArray(T) _template_(size_t capacity, bool auto_release);

//public destruct function
#define Template LoopArrayRelease
#define LoopArrayRelease(T, LA) _TypedVar_(LoopArrayRelease, T)(LA)
void _template_(_class_ self);

/*
* 迭代器接口
*/
//public
#define Template LoopArrayBegin
#define LoopArrayBegin(T, LA) _TypedVar_(LoopArrayBegin, T)(LA)
inline T* _template_(_class_ self) { return self->m_begin; }

//public
#define Template LoopArrayEnd
#define LoopArrayEnd(T, LA) _TypedVar_(LoopArrayEnd, T)(LA)
inline T* _template_(_class_ self) { return self->m_end; }

//public
#define Template LoopArrayForward
#define LoopArrayForward(T, LA, ptr) _TypedVar_(LoopArrayForward, T)(LA, ptr)
T* _template_(_class_ self, T* ptr);

//public
#define Template LoopArrayBackward
#define LoopArrayBackward(T, LA, ptr) _TypedVar_(LoopArrayBackward, T)(LA, ptr)
T* _template_(_class_ self, T* ptr);

/*
* 指针控制器
*/
//public
//++ptr
#define Template LoopArrayIncrease
#define LoopArrayIncrease(T, LA, ptr) _TypedVar_(LoopArrayIncrease, T)(LA, &ptr)
void _template_(_class_ self, T** ptr);

//public
//--ptr
#define Template LoopArrayDecrease
#define LoopArrayDecrease(T, LA, ptr) _TypedVar_(LoopArrayDecrease, T)(LA, &ptr)
void _template_(_class_ self, T** ptr);

/*
* 增删操作
*/

//public function
#define Template LoopArrayPushBack
#define LoopArrayPushBack(T, LA, val) _TypedVar_(LoopArrayPushBack, T)(LA, val)
void _template_(_class_ self, T val);

//public
#define Template LoopArrayPopBack
#define LoopArrayPopBack(T, LA) _TypedVar_(LoopArrayPopBack, T)(LA)
void _template_(_class_ self);

//public
#define Template LoopArrayPushFront
#define LoopArrayPushFront(T, LA, val) _TypedVar_(LoopArrayPushFront, T)(LA, val)
void _template_(_class_ self, T val);

//public
#define Template LoopArrayPopFront
#define LoopArrayPopFront(T, LA) _TypedVar_(LoopArrayPopFront, T)(LA)
void _template_(_class_ self);

/*
* 访问器
* 返回引用类型
*/

//public
#define Template LoopArrayGet
#define LoopArrayGet(T, LA, index) (*_TypedVar_(LoopArrayGet, T)(LA, index))
inline T* _template_(_class_ self, uint64_t index)
{
	CHECK(if (index >= self->m_size)
		ET(OutOfRange, "index out of range", NULL););
	index = (index + (self->m_begin - self->m_data)) % self->m_capacity;
	return &self->m_data[index];
}

//public
#define Template LoopArrayFront
#define LoopArrayFront(T, LA) (*_TypedVar_(LoopArrayFront, T)(LA))
inline T* _template_(_class_ self)
{
	CHECK(if (self->m_size == 0)
		ET(OutOfRange, "empty array", NULL););
	return self->m_begin;
}

//public
#define Template LoopArrayBack
#define LoopArrayBack(T, LA) (*_TypedVar_(LoopArrayBack, T)(LA))
inline T* _template_(_class_ self)
{
	CHECK(if (self->m_size == 0)
		ET(OutOfRange, "empty array", NULL););
	return LoopArrayBackward(T, self, self->m_end);
}

/*
* 遍历器接口
*/
//public
#define Template LoopArrayForeach
#define LoopArrayForeach(T, LA, visit) _TypedVar_(LoopArrayForeach, T)(LA, visit)
inline void _template_(_class_ self, Status visit(T* val))
{
	T* it = self->m_begin;
	for (int i = 0; i < self->m_size; i++)
	{
		if (visit(it) == Break) break;
		LoopArrayIncrease(T, self, it);
	}
}

//public
#define Template LoopArrayEmpty
#define LoopArrayEmpty(T, LA) _TypedVar_(LoopArrayEmpty, T)(LA)
inline bool _template_(_class_ self)
{
	return self->m_size == 0;
}

#define Template LoopArraySize
#define LoopArraySize(T, LA) _TypedVar_(LoopArraySize, T)(LA)
inline size_t _template_(_class_ self)
{
	return self->m_size;
}

//public 
#define Template LoopArrayClear
#define LoopArrayClear(T, LA) _TypedVar_(LoopArrayClear, T)(LA)
inline void _template_(_class_ self)
{
	self->m_size = 0;
	self->m_begin = self->m_data;
	self->m_end = self->m_data;
}
#include "TypedDefineEnd.h"
