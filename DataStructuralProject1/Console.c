#include "Console.h"

ConsoleSingleton console;

void ConsoleSetSize(int width, int height)
{
	SMALL_RECT rect;
	rect.Left = 0;
	rect.Top = 0;
	rect.Right = width - 1;
	rect.Bottom = height - 1;
	SetConsoleWindowInfo(console.handle_out, TRUE, &rect);
}

void ConsoleSetBackColor(WORD color)
{
	SetConsoleTextAttribute(console.handle_out, color | BACKGROUND_INTENSITY);
}

void ConsoleSetTextColor(WORD color)
{
	SetConsoleTextAttribute(console.handle_out, color | FOREGROUND_INTENSITY);
}

void ConsoleUpdate()
{
	COORD prevSize = console.csbi.dwSize;
	GetConsoleScreenBufferInfo(console.handle_out, &console.csbi);
	COORD size = console.csbi.dwSize;
	if (prevSize.X != size.X || prevSize.Y != size.Y)
	{
		if (console.OnSizeChange)
			console.OnSizeChange();
	}
}

COORD GetMousePos()
{
	return console.input_record.Event.MouseEvent.dwMousePosition;
}


void ConsoleSetCursorPos(COORD pos)
{
	SetConsoleCursorPosition(console.handle_out, pos);
}

void ConsoleSetCursorVisible(bool visible)
{
	CONSOLE_CURSOR_INFO cci;
	GetConsoleCursorInfo(console.handle_out, &cci);
	cci.bVisible = visible;
	SetConsoleCursorInfo(console.handle_out, &cci);
}

void ConsoleSetCursorSize(int size)
{
	CONSOLE_CURSOR_INFO cci;
	GetConsoleCursorInfo(console.handle_out, &cci);
	cci.dwSize = size;
	SetConsoleCursorInfo(console.handle_out, &cci);
}

COORD ConsoleGetCursorPos()
{
	GetConsoleScreenBufferInfo(console.handle_out, &console.csbi);
	return console.csbi.dwCursorPosition;
}

void ConsolePrint(COORD pos, WCHAR* str)
{
	DWORD dw;
	SetConsoleCursorPosition(console.handle_out, pos);
	WriteConsole(console.handle_out, str, (DWORD)wcslen(str), &dw, NULL);
}

void ConsolePrintChar(COORD pos, WCHAR c)
{
	DWORD dw;
	SetConsoleCursorPosition(console.handle_out, pos);
	WriteConsole(console.handle_out, &c, 1, &dw, NULL);
}

void ConsoleRead(WCHAR* str, DWORD size)
{
	DWORD dw;
	ReadConsole(console.handle_in, str, size, &dw, NULL);
}

WCHAR ConsoleReadKey()
{
	DWORD dw;
	ReadConsoleInput(console.handle_in, 
		&console.input_record, 
		1,
		&dw);
	//ReadConsole(console.handle_in, &console.input_record, 1, &dw, NULL);
	if (console.input_record.EventType == KEY_EVENT
		&& console.input_record.Event.KeyEvent.bKeyDown)//按下，释放不计入
		return console.input_record.Event.KeyEvent.uChar.UnicodeChar;
	else
		return 0;
}

void ConsoleClear()
{
	DWORD dw;
	COORD pos = { 0,0 };
	FillConsoleOutputCharacter(console.handle_out, ' ', console.csbi.dwSize.X * console.csbi.dwSize.Y, pos, &dw);
	FillConsoleOutputAttribute(console.handle_out, console.csbi.wAttributes, console.csbi.dwSize.X * console.csbi.dwSize.Y, pos, &dw);
	SetConsoleCursorPosition(console.handle_out, pos);
}

void ConsoleInit()
{
	console.handle_in = GetStdHandle(STD_INPUT_HANDLE);
	console.handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(console.handle_out, &console.csbi);

	console.SetSize = ConsoleSetSize;
	console.SetBackColor = ConsoleSetBackColor;
	console.SetTextColor = ConsoleSetTextColor;
	console.Update = ConsoleUpdate;
	console.SetCursorPos = ConsoleSetCursorPos;
	console.GetCursorPos = ConsoleGetCursorPos;
	console.SetCursorSize = ConsoleSetCursorSize;
	console.SetCursorVisible = ConsoleSetCursorVisible;
	console.Print = ConsolePrint;
	console.PrintChar = ConsolePrintChar;
	console.Read = ConsoleRead;
	console.ReadKey = ConsoleReadKey;
	console.Clear = ConsoleClear;

	//编码
	SetConsoleOutputCP(65001);
	SetConsoleCP(65001);

	system("chcp 65001");
}

void ConsoleOff()
{
	CloseHandle(console.handle_out);
	CloseHandle(console.handle_in);
}
