#pragma once
#include <stdint.h>
#include "Common.h"

void AutoPtrInit();

void AutoPtrReleaseAll();

void* AutoMalloc(size_t size, void internal_release(void*));
#define PRIVATE_MACRO_AUTO_MALLOC_3(type, count, release_func) (type*)AutoMalloc((count)*sizeof(type),release_func)
#define PRIVATE_MACRO_AUTO_MALLOC_2(type, count) (type*)AutoMalloc((count)*sizeof(type),free)
//AUTO_MALLOC(type, count, release_func[default = free])
#define AUTO_MALLOC(type, count, ...) VA_MACRO(PRIVATE_MACRO_AUTO_MALLOC,type, count,__VA_ARGS__)

void AutoRelease(void* ptr, void internal_release(void*), bool ex_free);

void AutoMallocBegin();

void AutoMallocEnd();

struct SharedPtr_Base
{
	void* ptr;//指针
	uint32_t count;//计数值
	void (*internal_release)(void*);//释放函数
};

typedef struct SharedPtr_Base* SharedPtr;

void SharedPtrRelease(SharedPtr ptr);

SharedPtr SharedPtrInit(void* ptr, void release(void*), bool auto_release);

SharedPtr SharedPtrPass(SharedPtr ptr, bool auto_release);

#define ptr_cast(shared_ptr , type) ((type*)shared_ptr->ptr)
