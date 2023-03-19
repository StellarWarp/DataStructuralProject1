#include "Allocator.h"

//private function
#define node_next(p_node) *(node_ptr*)(((char*)p_node) + allocator->type_size)
inline size_t allocate_node_size(size_t type_size)
{
	return type_size + sizeof(void*);
}
inline node_ptr get_node(const UniformAllocator allocator, uint64_t index)
{
	return (char*)(allocator->data_array) + 
		allocate_node_size(allocator->type_size) * index;
}

const int UniformAllocatorChunkCapacity = 1024*4;

UniformAllocator UniformAllocatorInit(size_t type_size)
{
	UniformAllocator allocator = MALLOC(struct UniformAllocator, 1);
	allocator->data_array = malloc(allocate_node_size(type_size) 
		* UniformAllocatorChunkCapacity);
	allocator->type_size = type_size;
	node_ptr* next = NULL;
	for (size_t i = 0; i <UniformAllocatorChunkCapacity; i++)
	{
		void* current_node = get_node(allocator, i);
		node_next(current_node) = next;
		next = current_node;
	}
	allocator->top = next;
	return allocator;
}
void UniformAllocatorRelease(UniformAllocator allocator)
{
	free(allocator->data_array);
	free(allocator);
}

void* ualloc(UniformAllocator allocator)
{
	if (allocator->top == NULL) return NULL;
	void* ret = allocator->top;
	allocator->top = node_next(allocator->top);
	return ret;
}

void ufree(UniformAllocator allocator, node_ptr ptr)
{
	node_next(ptr) = allocator->top;
	allocator->top = ptr;
}