#pragma once
#include "Common.h"

/*
* struct node
* {
*		T data;
*		node* next;
* }
*/
typedef void* node_ptr;
typedef struct UniformAllocator
{
	node_ptr data_array;
	node_ptr top;
	size_t type_size;
}*UniformAllocator;

UniformAllocator UniformAllocatorInit(size_t type_size);

void UniformAllocatorRelease(UniformAllocator allocator);

void* ualloc(UniformAllocator allocator);

void ufree(UniformAllocator allocator, node_ptr ptr);
