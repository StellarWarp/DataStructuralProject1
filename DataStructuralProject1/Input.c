#include "Input.h"

//#define SCO
//#include "Coroutine.h"
//
//bool InputLine(WCHAR* buffer)
//{
//	co_begin;
//	static i;
//	i = 0;
//	while (true)
//	{
//		if (!buffer) co_stop(Break);
//		WCHAR key = console.ReadKey();
//		if (key != '\0')
//		{
//			buffer[i++] = key;
//			putwchar(key);
//			if (key == '\r')
//			{
//				buffer[i] = '\0';
//				break;
//			}
//		}
//		co_return(Continue);
//	}
//	co_end(Break);
//}
//#undef SCO

void Input_blocked(char* buffer)
{
	static i;
	i = 0;
	while (true)
	{
		char key = _getch();
		if (key != '\0')
		{
			if (key == '\b')
			{
				if (i > 0)
				{
					printf("\b \b");
					buffer[i--] = '\0';
				}
			}
			else
			{
				buffer[i++] = key;
				if (key == '\r' || key == ' ')
				{
					buffer[i - 1] = '\0';
					break;
				}
				else
				{
					putchar(key);
				}
			}
		}
	}
}


