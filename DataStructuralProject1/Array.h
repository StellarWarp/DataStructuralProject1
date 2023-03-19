#pragma once
#include "Common.h"
#include "Utility.h"

//常用类型
//整形
#define T int
#include "ArrayTemplate.h"
#define T int64_t
#include "ArrayTemplate.h"
#define T size_t
#include "ArrayTemplate.h"
//浮点
#define T float
#include "ArrayTemplate.h"
#undef T

#define Stack Array
#define StackInit ArrayInit
#define StackRelease ArrayRelease
#define StackPush ArrayPushBack
#define StackPop ArrayPopBack
#define StackEmpty ArrayEmpty

#define StackTop ArrayBack

#define StackSize ArraySize
#define StackClear ArrayClear

#define Queue LoopArray
#define QueueInit LoopArrayInit
#define QueueRelease LoopArrayRelease
#define QueuePush LoopArrayPushBack
#define QueuePop LoopArrayPopFront
#define QueueEmpty LoopArrayEmpty

#define QueueFront LoopArrayFront
#define QueueBack LoopArrayBack

#define QueueSize LoopArraySize
#define QueueClear LoopArrayClear


