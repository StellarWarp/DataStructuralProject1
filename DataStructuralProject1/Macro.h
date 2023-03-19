#pragma once

#define PRIVATE_MACRO_VAR_ARGS_IMPL_COUNT(_1,_2,_3,_4,_5,_6,_7,_8,_9, N, ...) N
#define PRIVATE_MACRO_VAR_ARGS_IMPL(args)  PRIVATE_MACRO_VAR_ARGS_IMPL_COUNT args
#define COUNT_MACRO_VAR_ARGS(...)  PRIVATE_MACRO_VAR_ARGS_IMPL((__VA_ARGS__,9,8,7,6,5,4,3,2,1,0))

#define PRIVATE_MACRO_CAT_2__(x, y)  x##y
#define PRIVATE_MACRO_CAT_2_(x, y)  PRIVATE_MACRO_CAT_2__(x, y)
#define PRIVATE_MACRO_CAT_2(x, y)   PRIVATE_MACRO_CAT_2_(x, y)

#define PRIVATE_MACRO_CAT_3__(x, y, z)  x##y##z
#define PRIVATE_MACRO_CAT_3_(x, y, z)  PRIVATE_MACRO_CAT_3__(x, y, z)
#define PRIVATE_MACRO_CAT_3(x, y, z)   PRIVATE_MACRO_CAT_3_(x, y, z)

#define PRIVATE_MACRO_CAT_4__(x, y, z, w)  x##y##z##w
#define PRIVATE_MACRO_CAT_4_(x, y, z, w)  PRIVATE_MACRO_CAT_4__(x, y, z, w)
#define PRIVATE_MACRO_CAT_4(x, y, z, w)   PRIVATE_MACRO_CAT_4_(x, y, z, w)

#define PRIVATE_MACRO_CAT_5__(a, b, c, d, e)  a##b##c##d##e
#define PRIVATE_MACRO_CAT_5_(a, b, c, d, e)  PRIVATE_MACRO_CAT_5__(a, b, c, d, e)
#define PRIVATE_MACRO_CAT_5(a, b, c, d, e)   PRIVATE_MACRO_CAT_5_(a, b, c, d, e)

#define PRIVATE_MACRO_CAT_6__(a, b, c, d, e, f)  a##b##c##d##e##f
#define PRIVATE_MACRO_CAT_6_(a, b, c, d, e, f)  PRIVATE_MACRO_CAT_6__(a, b, c, d, e, f)
#define PRIVATE_MACRO_CAT_6(a, b, c, d, e, f)   PRIVATE_MACRO_CAT_6_(a, b, c, d, e, f)

#define PRIVATE_MACRO_CAT_7__(a, b, c, d, e, f, g)  a##b##c##d##e##f##g
#define PRIVATE_MACRO_CAT_7_(a, b, c, d, e, f, g)  PRIVATE_MACRO_CAT_7__(a, b, c, d, e, f, g)
#define PRIVATE_MACRO_CAT_7(a, b, c, d, e, f, g)   PRIVATE_MACRO_CAT_7_(a, b, c, d, e, f, g)

//用于连接多个宏
#define MACRO_CAT(...) PRIVATE_MACRO_CAT_2(PRIVATE_MACRO_CAT_,COUNT_MACRO_VAR_ARGS(__VA_ARGS__)(__VA_ARGS__))

//可变参数宏
//pre_name：宏前缀
//pre_name_x 为宏名 x为参数个数
#define VA_MACRO(pre_name, ...) PRIVATE_MACRO_CAT_4(pre_name,_,COUNT_MACRO_VAR_ARGS(__VA_ARGS__),(__VA_ARGS__))

//用于展开
#define EXPAND(...) __VA_ARGS__

#define STRING_OF(str) #str

//内存分配
extern char* g_malloc_ptr;
#if  defined(_DEBUG) || !defined(NDEBUG)
//不要尝试在隐式块语句中使用
#define MALLOC(type, size) (type*)(g_malloc_ptr =(char*)malloc(sizeof(type) * (size)));\
if(g_malloc_ptr==NULL) ExceptionCall(OverFlow,"Malloc Exception",__FUNCTION__,__FILE__,__LINE__);\
else memset(g_malloc_ptr, 0, sizeof(type)* (size));

#define REALLOC(type, ptr, size) (type*)(g_malloc_ptr =(char*)realloc(ptr, sizeof(type) * (size)));\
if (g_malloc_ptr == NULL) ExceptionCall(OverFlow,"Realloc Exception",__FUNCTION__,__FILE__,__LINE__);

#define PRIVATE_MACRO_NEW_3(type,size,init) (type*)(g_malloc_ptr = (char*)malloc(sizeof(type) * (size)));\
if (g_malloc_ptr)\
{\
	type val = { EXPAND init };\
	for (size_t i = 0; i < size; i++)\
		memcpy((void*)(g_malloc_ptr + (i * sizeof(type))), &val, sizeof(type));\
}\
else ExceptionCall(OverFlow,"Malloc Exception",__FUNCTION__,__FILE__,__LINE__);

#define PRIVATE_MACRO_NEW_2(type, init) (type*)(g_malloc_ptr = (char*)malloc(sizeof(type)));\
if (g_malloc_ptr)\
{\
	type val = { EXPAND init };\
	*((type*)g_malloc_ptr) = val;\
}\
else ExceptionCall(OverFlow,"Malloc Exception",__FUNCTION__,__FILE__,__LINE__);

#else
#define MALLOC(type, size) (type*)memset(malloc(sizeof(type) * (size)),0,sizeof(type) * (size))
#define REALLOC(type, ptr, size) (type*)realloc(ptr, sizeof(type) * (size));

#define PRIVATE_MACRO_NEW_3(type,size,init) (type*)(g_malloc_ptr = (char*)malloc(sizeof(type) * (size)));\
{\
	type val = { EXPAND init };\
	for (size_t i = 0; i < size; i++)\
		memcpy((void*)(g_malloc_ptr + (i * sizeof(type))), &val, sizeof(type));\
}

#define PRIVATE_MACRO_NEW_2(type, init) (type*)(g_malloc_ptr = (char*)malloc(sizeof(type)));\
{\
	type val = { EXPAND init };\
	*((type*)g_malloc_ptr) = val;\
}

#endif //  defined(_DEBUG)

//内存分配与初始化
// 使用一个初始化值
//不要尝试在隐式块语句中使用
//NEW( type , [optional] count, (single initialize value))
#define NEW(type,...) VA_MACRO(PRIVATE_MACRO_NEW, type, __VA_ARGS__)

