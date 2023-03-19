#pragma once
#include "Common.h"

typedef struct OutPutBuffer
{
	WCHAR* buffer;
	uint32_t X_length;
	uint32_t Y_length;
}OutPutBuffer;

typedef struct BufferView
{
	OutPutBuffer* buffer;
	COORD begin;
	COORD end;
}BufferView;