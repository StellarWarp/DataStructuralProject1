#include "Array.h"
#include "AutoPtr.h"

// private field

typedef struct UniquePtr
{
	void* ptr;//指针
	void (*internal_release)(void*);//销毁函数
	bool ex_free;//是否需要额外调用一次free
}UniquePtr;

#define T UniquePtr
#include "ArrayTemplate.h"
#include "ArrayCore.h"

Stack(UniquePtr) g_PtrStack;
Stack(size_t) g_CounterStack;

//public field

void AutoPtrInit()
{
	g_PtrStack = StackInit(UniquePtr, 1024, false);
	g_CounterStack = StackInit(size_t, 1024, false);
}

void AutoPtrReleaseAll()
{
	for (int i = 0; i < g_PtrStack->m_size; i++)
	{
		UniquePtr* ph = &g_PtrStack->m_data[i];
		ph->internal_release(ph->ptr);
	}
	StackRelease(size_t, g_CounterStack);
	StackRelease(UniquePtr, g_PtrStack);
}

void* AutoMalloc(size_t size, void internal_release(void*))
{
	CHECK(
		if (StackEmpty(size_t, g_CounterStack))ET(Error, "Have not call AutoMallocBegin", NULL);
	);
	void* p = malloc(size);
	memset(p, 0, size);
	if (internal_release == NULL) internal_release = free;
	UniquePtr h = { p,internal_release,false };
	StackPush(UniquePtr, g_PtrStack, h);
	return p;
}

void AutoRelease(void* ptr, void internal_release(void*), bool ex_free)
{
	CHECK(if (internal_release == NULL) ET(Error, "No Release Function"););
	UniquePtr p = { ptr , internal_release ,ex_free };
	StackPush(UniquePtr, g_PtrStack, p);
}

void AutoMallocBegin()
{
	StackPush(size_t, g_CounterStack, g_PtrStack->m_size);
}

void AutoMallocEnd()
{
	size_t begin = StackTop(size_t, g_CounterStack);
	StackPop(size_t, g_CounterStack);
	while (g_PtrStack->m_size > begin)
	{
		UniquePtr* ph = &StackTop(UniquePtr, g_PtrStack);
		StackPop(UniquePtr, g_PtrStack);
		ph->internal_release(ph->ptr);
		if (ph->ex_free) free(ph);
	}
	g_PtrStack->m_size = begin;
}

void SharedPtrRelease(SharedPtr ptr)
{
	ptr->count--;
	if (ptr->count == 0)
	{
		ptr->internal_release(ptr->ptr);
		if (ptr->internal_release != free) free(ptr->ptr);
		free(ptr);
	}
}

SharedPtr SharedPtrInit(void* ptr, void internal_release(void*), bool auto_release)
{
	struct SharedPtr_Base* base = (struct SharedPtr_Base*)malloc(sizeof(struct SharedPtr_Base));
	base->ptr = ptr;
	base->count = 1;
	base->internal_release = internal_release;
	if (auto_release) AutoRelease(base, SharedPtrRelease, true);
	return base;
}

SharedPtr SharedPtrPass(SharedPtr ptr, bool auto_release)
{
	if (auto_release) AutoRelease(ptr, SharedPtrRelease, true);
	ptr->count++;
	return ptr;
}

