#pragma once
#include "Common.h"
#include <windows.h>  
#include <conio.h>

#define FORE_BLUE        FOREGROUND_BLUE           //蓝色文本属性  
#define FORE_GREEN      FOREGROUND_GREEN          //绿色文本属性  
#define FORE_RED          FOREGROUND_RED            //红色文本属性  
#define FORE_PURPLE   (FORE_BLUE | FORE_RED)      //紫色文本属性  
#define FORE_CYAN        (FORE_BLUE | FORE_GREEN)    //青色文本属性  
#define FORE_YELLOW   (FORE_RED | FORE_GREEN)     //黄色文本属性  
#define FORE_GRAY         FOREGROUND_INTENSITY      //灰色文本属性  
#define FORE_WHITE   (FORE_GREEN | FORE_BLUE | FORE_RED)     //白色文本属性  

#define BACK_BLUE         BACKGROUND_BLUE           //蓝色背景属性  
#define BACK_GREEN       BACKGROUND_GREEN          //绿色背景属性  
#define BACK_RED            BACKGROUND_RED            //红色背景属性  
#define BACK_PURPLE    (BACK_BLUE | BACK_RED)      //紫色背景属性  
#define BACK_CYAN         (BACK_BLUE | BACK_GREEN)    //青色背景属性  
#define BACK_YELLOW     (BACK_RED | BACK_GREEN)     //黄色背景属性  
#define BACK_GRAY          BACKGROUND_INTENSITY      //灰色背景属性 
#define BACK_WHITE   (BACK_GREEN | BACK_BLUE | BACK_RED)     //白色背景属性

typedef struct ConsoleSingleton
{
	HANDLE handle_in;
	HANDLE handle_out;
	CONSOLE_SCREEN_BUFFER_INFO csbi;        //定义窗口缓冲区信息结构体
	INPUT_RECORD input_record;
	
	void (*OnSizeChange)();

	void (*SetSize)(int width, int height);
	void (*SetBackColor)(WORD color);
	void (*SetTextColor)(WORD color);

	void (*Update)();

	COORD(*GetMousePos)();
	
	void (*SetCursorPos)(COORD pos);
	void (*SetCursorVisible)(bool visible);
	void (*SetCursorSize)(int size);
	COORD(*GetCursorPos)();
	
	void (*Print)(COORD pos, WCHAR* str);
	void (*PrintChar)(COORD pos, WCHAR c);
	void (*Read)(WCHAR* str, DWORD size);
	WCHAR(*ReadKey)();
	
	void (*Clear)();
}ConsoleSingleton;

extern ConsoleSingleton console;

void ConsoleInit();
void ConsoleOff();