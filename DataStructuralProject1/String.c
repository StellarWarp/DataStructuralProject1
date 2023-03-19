#include "String.h"

String StringInit()
{
	String str;
	str.data = MALLOC(WCHAR,1);
	str.len = 0;
	return str;
}

String StringCreate(const WCHAR* str)
{
	String ret;
	ret.len = wcslen(str);
	ret.data = MALLOC(WCHAR, ret.len + 1);
	wcscpy_s(ret.data, ret.len + 1, str);
	return ret;
}

void StringRelease(String* str)
{
	free(str->data);
	str->data = NULL;
	str->len = 0;
}

String StringCopy(const String* str)
{
	String ret;
	ret.len = str->len;
	ret.data = MALLOC(WCHAR, ret.len + 1);
	wcscpy_s(ret.data, ret.len + 1, str->data);
	return ret;
}

String StringConcat(const String* str1, const String* str2)
{
	String ret;
	ret.len = str1->len + str2->len;
	ret.data = MALLOC(WCHAR, ret.len + 1);
	wcscpy_s(ret.data, ret.len + 1, str1->data);
	wcscat_s(ret.data, ret.len + 1, str2->data);
	return ret;
}

void StringClear(String* str)
{
	free(str->data);
	str->data = MALLOC(WCHAR, 1);
	str->len = 0;
}

void StringAdd(String* str1, const String* str2)
{
	str1->len += str2->len;
	str1->data = (WCHAR*)realloc(str1->data, sizeof(WCHAR) * (str1->len + 1));
	wcscat_s(str1->data, str1->len + 1, str2->data);
}

void StringExtend(String* tar, const WCHAR* add)
{
	size_t add_len = wcslen(add);
	tar->data = (WCHAR*)realloc(tar->data, sizeof(WCHAR) * (tar->len + add_len + 1));
	tar->len += add_len;
	wcscat_s(tar->data, tar->len + 1, add);
}
