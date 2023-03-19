#include "TypedDefineBegin.h"
#include "Common.h"
#include "Allocator.h"

#define _type_ T//别名

#define _class_ LinkedListNode(T)
#define LinkedListNode(T) _TypedVar_(LinkedListNode, T)
typedef struct _class_
{
	_type_ data;            //数据
	struct _class_* next;//下一节点
}*_class_;

#define _class_ LinkedList(T)//类名
#define LinkedList(T) _TypedVar_(LinkedList, T)//宏包装
typedef struct _class_
{
	LinkedListNode(T) head;     //头节点
	LinkedListNode(T) tail;        //尾节点
	UniformAllocator allocator;//内存分配器
	int length;                           //长度
}*_class_;

#define Template LinkedListInit
#define LinkedListInit(T, auto_release) _TypedVar_(LinkedListInit, T)(auto_release)
LinkedList(T) _template_(bool auto_release);

#define Template LinkedListRelease
#define LinkedListRelease(T, list) _TypedVar_(LinkedListRelease, T)(list)
void _template_(LinkedList(T) list);

#define Template LinkedListInsert
#define LinkedListInsert(T, list, prev_node, data) _TypedVar_(LinkedListInsert, T)(list, prev_node, data)
void _template_(LinkedList(T) list, LinkedListNode(T) prev_node, _type_ data);

#define Template LinkedListRemove
#define LinkedListRemove(T, list, prev_node) _TypedVar_(LinkedListRemove, T)(list, prev_node)
LinkedListNode(T) _template_(LinkedList(T) list, LinkedListNode(T) prev_node);

#define Template LinkedListClear
#define LinkedListClear(T, list) _TypedVar_(LinkedListClear, T)(list)
void _template_(LinkedList(T) list);

#define Template LinkedListPushFront
#define LinkedListPushFront(T, list, data) _TypedVar_(LinkedListPushFront, T)(list, data)
void _template_(LinkedList(T) list, _type_ data);

#define Template LinkedListPopFront
#define LinkedListPopFront(T, list) _TypedVar_(LinkedListPopFront, T)(list)
void _template_(LinkedList(T) list);

#define Template LinkedListPushBack
#define LinkedListPushBack(T, list, data) _TypedVar_(LinkedListPushBack, T)(list, data)
void _template_(LinkedList(T) list,  _type_ data);

#define Template LinkedListSize
#define LinkedListSize(T, list) _TypedVar_(LinkedListSize, T)(list)
inline size_t _template_(LinkedList(T) list) { return list->length; }

#define Template LinkedListEmpty
#define LinkedListEmpty(T, list) _TypedVar_(LinkedListEmpty, T)(list)
inline bool _template_(LinkedList(T) list) { return list->length == 0; }

/*
* 引用访问器
*/
#define Template LinkedListFront
#define LinkedListFront(T, list) *_TypedVar_(LinkedListFront, T)(list)//引用访问
inline _type_* _template_(LinkedList(T) list) { return &list->head->data; }

#define Template LinkedListBack
#define LinkedListBack(T, list) *_TypedVar_(LinkedListBack, T)(list)//引用访问
inline _type_* _template_(LinkedList(T) list) { return &list->tail->data; }

/*
* 迭代器接口
*/
#define Template LinkedListBegin
#define LinkedListBegin(T, list) _TypedVar_(LinkedListBegin, T)(list)
inline LinkedListNode(T) _template_(LinkedList(T) list)
{
	return list->head;
}

#define Template LinkedListEnd
#define LinkedListEnd(T, list) _TypedVar_(LinkedListEnd, T)(list)
inline LinkedListNode(T) _template_(LinkedList(T) list)
{
	return NULL;
}

#define Template LinkedListForward
#define LinkedListForward(T, list, node) _TypedVar_(LinkedListForward, T)(list, node)
inline LinkedListNode(T) _template_(LinkedList(T) list, LinkedListNode(T) node)
{
	return node->next;
}




