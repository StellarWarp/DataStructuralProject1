#pragma once
#include "Common.h"

typedef struct String
{
	WCHAR* data;
	size_t len;
}String;

String StringInit();
String StringCreate(const WCHAR* str);
void StringRelease(String* str);
//复制
String StringCopy(const String* str);
//合并
String StringConcat(const String* str1, const String* str2);

void StringClear(String* str);
//合并
void StringAdd(String* str1, const String* str2);

void StringExtend(String* str1, const WCHAR* str);


