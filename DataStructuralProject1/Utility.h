#pragma once
#include "Common.h"
#include "TypedDefineBegin.h"

#define T size_t
#include "UtilityTemplate.h"

//typeof [a] and typeof [b] must be the same
#define swap(a, b) _swab((char*)&a,(char*)&b,sizeof(a))

#define NativeArrayInsert(arr, len, index, val) do{\
CHECK(if(!(index>=0 && len - (index) - 1>=0)) ETNR(OutOfRange,"NativeArrayInsert"));\
memmove(arr + index + 1, arr + index, sizeof(arr[0]) * (len - (index) - 1));\
arr[index] = val;\
}while (0)

#define NativeArrayRemove(arr, len, index) do{\
CHECK(if(!(index>=0 && len - (index) - 1>=0)) ETNR(OutOfRange,"NativeArrayRemove"));\
memmove(arr + index, arr + index + 1, sizeof(arr[0]) * (len - (index) - 1));\
}while (0)

#define NativeArrayMove(A, A_low, A_high, B, B_low) do{\
CHECK(if(!(A_high - (A_low) + 1 >= 1 && B_low >= 0)) ETNR(OutOfRange,"NativeArrayMove"));\
memmove(B + B_low, A + A_low, (A_high - (A_low)+1) * sizeof(*A)); \
}while (0)

#include "TypedDefineEnd.h"

