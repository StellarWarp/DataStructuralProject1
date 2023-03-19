#pragma once
#include "TypedDefineBegin.h"

#define _type_ T

#define Template up_pow2
T _template_(T n)
{
	if (n == 0) return 1;
	--n;
	n |= n >> 1;
	n |= n >> 2;
	n |= n >> 4;
	n |= n >> 8;
	n |= n >> 16;
	n |= n >> 32;
	return ++n;
}

#define Template BinarySearch
int _template_(T* arr, size_t len, T val)
{
	if (len == 0) return 0;
	if (val > arr[len - 1]) return len;//超出范围返回最大索引+1
	if (val <= arr[0]) return 0;
	int mid;//搜索位置
	T mid_val;

	int a = 0;
	int b = len - 1;

	while (1)
	{
		mid = (a + b) / 2;
		mid_val = arr[mid];

		if (a == mid) return mid + 1;
		else if (mid_val < val) a = mid;
		else if (mid_val >= val) b = mid;
	}
}

#include "TypedDefineEnd.h"