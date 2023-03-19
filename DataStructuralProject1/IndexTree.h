#pragma once
#include "Console.h"
#include "String.h"
#include "Array.h"
//#include "TypedDefineBegin.h"
//
//const int M = 10;
//static const int min_key = (M % 2) ? M / 2 + 1 : M / 2;//m/2向上取整
//static const int max_key = M - 1;
//static const int min_i = min_key - 1;
//static const int max_i = max_key - 1;
//
//#define T1 size_t
////key type
//#define KT T1
//#define T2 size_t
////value type
//#define VT T2
//#include "IndexTreeTemplate.h"
//
//
//#include "TypedDefineEnd.h"

/*
* B+树
*/
typedef struct PathInfo
{
	struct LayerN* Node;
	uint16_t index;
}PathInfo;

#define T PathInfo
#include "ArrayTemplate.h"


#define KT size_t
#define PT Page*
#define DT size_t
typedef struct Page
{
	DT data;
	struct Page* next;
}Page;
Page* PageInit(DT data);
//union pNode;
typedef union pNode//节点间指针
{
	struct LayerData* Dp;
	struct LayerN* Np;
	void* any;
}pNode;
typedef struct IndexTree
{
	pNode root;//根节点指针
	size_t height;//树高
	size_t count;//索引数量
	Stack(PathInfo) path;//栈
	//void (*sval_add)(void*, void*);//(classPtr, dataPointer)
	//bool (*data_release)(void*, void*);//(classPtr, dataPointer)
}IndexTree;




IndexTree IndexTreeInit();

void IndexTreeRelease(IndexTree* tree);


void IndexTreeAdd(IndexTree* self, KT key, DT val);

void IndexTreeRemove(IndexTree* self, KT key, PT del_page);

PT IndexTreeFind(IndexTree* self, KT key);

void IndexTreePrint(IndexTree* tree, void print(WCHAR* buffer, size_t count, DT data));
