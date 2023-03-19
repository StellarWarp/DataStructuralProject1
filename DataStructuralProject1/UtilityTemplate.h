#pragma once
#include "TypedDefineBegin.h"
#define _type_ T

#define Template up_pow2
#define up_pow2(type ,x) _TypedVar_(up_pow2, type)(x)
T _template_(T n);


#define Template BinarySearch//模板名称
//如果不存在，返回最接近但大于val的值
//超出范围返回最大索引+1
#define BinarySearch(type ,arr, len, val) _TypedVar_(BinarySearch, type)(arr, len, val)//宏包装
int _template_(T* arr, size_t len, T val);//函数声明

#include "TypedDefineEnd.h"