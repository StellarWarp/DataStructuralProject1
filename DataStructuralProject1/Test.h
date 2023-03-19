#pragma once
#include "Console.h"
#include "Input.h"
#include "Compare.h"
#include "IndexTree.h"
#include "Input.h"
#include "AutoPtr.h"
#include "Allocator.h"
#include "Array.h"
#include "LinkedList.h"


//void IndexTreeTest()
//{
//#define SCALE 400
//	size_t test_data[SCALE];
//	Page* test_page[SCALE];
//	IndexTree tree = IndexTreeInit();
//	COORD pos = { 0, 0 };
//	for (int i = 121231; true; i++)
//	{
//		srand(i);
//		for (int i = 0; i < SCALE; i++)
//		{
//			test_data[i] = rand() % SCALE;
//		}
//		for (int i = 0; i < SCALE; i++)
//		{
//			test_page[i] = PageInit(test_data[i]);
//			IndexTreeAppend(&tree, test_page[i], test_data[i]);
//			if (i % 9 == 0)
//			{
//				console.Clear();
//				//IndexTreePrint(&tree, pos);
//				//wait();
//			}
//		}
//		for (int i = 0; i < SCALE; i++)
//		{
//			IndexTreeRemove(&tree, test_page[i], test_data[i]);
//			if (i % 9 == 0)
//			{
//				console.Clear();
//				//IndexTreePrint(&tree, pos);
//				//wait();
//			}
//		}
//	}
//	//IndexTreeRemove(&tree, 5, 5);
//	//console.SetCursorPos(pos);
//	//IndexTreePrint(&tree, pos);
//}

#define TOWSTR(x) #x
#define EmptyCommand if(!inputBuffer[0]) command = Empty;
#define FitEnum(target)if( stricmp(TOWSTR(target), inputBuffer) == 0) command = target;

enum Command
{
	Empty,
	End,
	PreOrder,
	PostOrder,
	Find,
	Insert,
	Remove,
	Replace,
	Parent,
	Switch
};
#pragma warning(disable:4996)
enum Command InputCommand()
{
	char inputBuffer[100] = { 0 };
	enum Command command = Empty;
	Input_blocked(inputBuffer);
	EmptyCommand;
	FitEnum(End);
	FitEnum(Switch);
	FitEnum(PreOrder);
	FitEnum(PostOrder);
	FitEnum(Find);
	FitEnum(Insert);
	FitEnum(Remove);
	FitEnum(Replace);
	FitEnum(Parent);



	return command;
}
int InputIntParam(const char* prompt)
{
	char inputBuffer[100] = { 0 };
	int param = 0;
	printf(" %s [", prompt);
	Input_blocked(inputBuffer);
	printf("]");
	if (inputBuffer[0] == '0' && inputBuffer[1] == 'x')
	{
		sscanf_s(inputBuffer, "%x", &param);
	}
	else
	{
		sscanf_s(inputBuffer, "%d", &param);
	}
	return param;
}

SharedPtr PassTest()
{
	AutoMallocBegin();

	Array(int) a = ArrayInit(int, 4, true);
	Array(float) b = ArrayInit(float, 4, true);
	char* temp = AUTO_MALLOC(char, 100);
	
	SharedPtr shared_resourece = SharedPtrInit(
		malloc(sizeof(char)*1024), 
		free, 
		false);

	AutoMallocEnd();
	return shared_resourece;
}
void test()
{
	AutoMallocBegin();

	SharedPtr resource = SharedPtrPass(
		PassTest(), 
		true);

	AutoMallocEnd();
}

void ualloc_test()
{
	UniformAllocator allocator = UniformAllocatorInit(sizeof(int));

	int* A = ualloc(allocator);
	int* B = ualloc(allocator);
	int* C = ualloc(allocator);
	ufree(allocator, A);
	ufree(allocator, B);
	ufree(allocator, C);
	A = ualloc(allocator);
	B = ualloc(allocator);
	C = ualloc(allocator);
}
