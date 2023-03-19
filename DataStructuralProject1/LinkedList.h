#pragma once

#define T int
#include "LinkedListTemplate.h"

#undef T




//#define _type_ int
//typedef struct LinkedListNode
//{
//	_type_ data;
//	struct LinkedListNode* next;
//}*LinkedListNode;
//
//typedef struct LinkedList
//{
//	LinkedListNode head;
//	LinkedListNode tail;
//	UniformAllocator allocator;
//	int length;
//}*LinkedList;
//
//LinkedList LinkedListInit(bool auto_release);
//
//void LinkedListRelease(LinkedList list);
//
//void LinkedListInsert(LinkedList list, LinkedListNode prev_node, _type_ data);
//
//void LinkedListRemove(LinkedList list, LinkedListNode prev_node);
//
//void LinkedListPushFront(LinkedList list, _type_ data);
//
//void LinkedListPopFront(LinkedList list);
//
//void LinkedListPopBack(LinkedList list);
//
///*
//* 迭代器接口
//*/
//LinkedListNode LinkedListBegin(LinkedList list);
//LinkedListNode LinkedListEnd(LinkedList list);
//LinkedListNode LinkedListForward(LinkedList list,LinkedListNode node);

