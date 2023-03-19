#pragma once

char _template_(T a, T b);//声明
#ifdef Compare_CF//此宏在编译文件中提前定义
char _template_(T a, T b)//实现
{
	return (a > b) - (a < b);
}
#endif
