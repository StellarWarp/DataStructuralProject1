#include "OutPutBuffer.h"

OutPutBuffer OutPutBufferInit(uint32_t X_length, uint32_t Y_length)
{
	OutPutBuffer outPutBuffer;
	outPutBuffer.buffer = MALLOC(WCHAR, X_length * Y_length);
	outPutBuffer.X_length = X_length;
	outPutBuffer.Y_length = Y_length;
	return outPutBuffer;
}

void OutPutBufferFree(OutPutBuffer* outPutBuffer)
{
	free(outPutBuffer->buffer);
}

void OutPutBufferClear(OutPutBuffer* outPutBuffer)
{
	memset(outPutBuffer->buffer, 0, sizeof(WCHAR) * outPutBuffer->X_length * outPutBuffer->Y_length);
}

void OutPutBufferResize(OutPutBuffer* outPutBuffer)
{
	outPutBuffer->buffer = REALLOC(WCHAR, outPutBuffer->buffer, outPutBuffer->X_length * outPutBuffer->Y_length);
}

void OutPutBufferSetChar(OutPutBuffer* outPutBuffer, uint32_t x, uint32_t y, WCHAR c)
{
	CHECK(if (!(x >= 0 && y >= 0 && x < outPutBuffer->X_length && y < outPutBuffer->Y_length)) ETNR(OutOfRange, "OutPutBufferSetChar"));
	outPutBuffer->buffer[y * outPutBuffer->X_length + x] = c;
}

void OutPutBufferSetString(OutPutBuffer* outPutBuffer, uint32_t x, uint32_t y, WCHAR* str)
{
	uint32_t end_x = x + wcslen(str);
	uint32_t end_y = y;
	for (WCHAR* p = str; *p; p++)
	{
		if (*p == L'\n')end_y++;
	}
	if (end_x >= outPutBuffer->X_length || end_y >= outPutBuffer->Y_length)
	{
		outPutBuffer->X_length = end_x + 1;
		outPutBuffer->Y_length = end_y + 1;
		OutPutBufferResize(outPutBuffer);
	}
	uint32_t i = 0;
	while (str[i] != L'\0')
	{
		outPutBuffer->buffer[y * outPutBuffer->X_length + x + i] = str[i];
		i++;
	}
}