#include "AutoPtr.h"
#include "TypedDefineBegin.h"
#define _type_ T//别名

#define Template LinkedListInit
LinkedList(T) _template_(bool auto_release)
{
	LinkedList(T) list = (LinkedList(T))malloc(sizeof(struct LinkedList(T)));
	list->head = NULL;
	list->tail = NULL;
	list->length = 0;
	if (auto_release)
	{
		AutoRelease(list, _TypedVar_(LinkedListRelease,T), false);
	}
	list->allocator = UniformAllocatorInit(sizeof(struct LinkedListNode(T)));
	return list;
}

#define Template LinkedListRelease
void _template_(LinkedList(T) list)
{
	LinkedListNode(T) node = list->head;
	while (node != NULL)
	{
		LinkedListNode(T) next = node->next;
		ufree(list->allocator, node);
		node = next;
	}
	UniformAllocatorRelease(list->allocator);
	free(list);
}

#define Template LinkedListInsert
void _template_(LinkedList(T) list, LinkedListNode(T) prev_node, _type_ data)
{
	LinkedListNode(T) node = ualloc(list->allocator);
	node->data = data;
	if (prev_node == NULL)
	{
		node->next = list->head;
		list->head = node;
	}
	else
	{
		node->next = prev_node->next;
		prev_node->next = node;
	}
	if (node->next == NULL)
	{
		list->tail = node;
	}
	list->length++;
}

#define Template LinkedListRemove
LinkedListNode(T) _template_(LinkedList(T) list, LinkedListNode(T) prev_node)
{
	LinkedListNode(T) node = prev_node == NULL ? list->head : prev_node->next;
	if (node == NULL) return NULL;
	if (prev_node == NULL)
	{
		list->head = node->next;
	}
	else
	{
		prev_node->next = node->next;
	}
	if (node->next == NULL)
	{
		list->tail = prev_node;
	}
	ufree(list->allocator, node);
	list->length--;
	CHECK(
		if (list->length < 0)
		{
			ET(OutOfRange, "list length < 0", NULL);
			exit(-1);
		});
	return prev_node? prev_node->next : NULL;//new iterator
}

#define Template LinkedListClear
void _template_(LinkedList(T) list)
{
	LinkedListNode(T) node = list->head;
	while (node != NULL)
	{
		LinkedListNode(T) next = node->next;
		ufree(list->allocator, node);
		node = next;
	}
	list->head = NULL;
	list->tail = NULL;
	list->length = 0;
}

#define Template LinkedListPushFront
void _template_(LinkedList(T) list, _type_ data)
{
	LinkedListNode(T) node = ualloc(list->allocator);
	node->data = data;
	node->next = list->head;
	list->head = node;
	if (list->tail == NULL)
	{
		list->tail = node;
	}
	list->length++;
}

#define Template LinkedListPopFront
void _template_(LinkedList(T) list)
{
	LinkedListNode(T) node = list->head;
	if (node == NULL) return;
	list->head = node->next;
	if (node->next == NULL)
	{
		list->tail = NULL;
	}
	ufree(list->allocator, node);
	list->length--;
}

#define Template LinkedListPushBack
void _template_(LinkedList(T) list, _type_ data)
{
	LinkedListNode(T) node = ualloc(list->allocator);
	node->data = data;
	node->next = NULL;
	if (list->tail == NULL)
	{
		list->head = node;
	}
	else
	{
		list->tail->next = node;
	}
	list->tail = node;
	list->length++;
}



#include "TypedDefineEnd.h"