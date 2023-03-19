#pragma once
#include "Common.h"
#include "OutPutBuffer.h"

typedef struct Frame
{
	/*
	┌┬┐
	├┼┤
	└┴┘
	┌─┐
	│┼│
	└─┘
	┏┳┓
	┣╋┫
	┗┻┛
	┏━┓
	┃╋┃
	┗━┛
	┎┰┒
	┠╂┨
	┖┸┚
	┍┯┑
	┝┿┥
	┕┷┙
	╔╦╗
	╠╬╣
	╚╩╝
	*/
	WCHAR u;//up
	WCHAR d;//down
	WCHAR r;//right
	WCHAR l;//left
	WCHAR ul;//upper left corner
	WCHAR ur;//upper right corner
	WCHAR dl;//down left corner
	WCHAR dr;//down right corner
	WORD color;//color
}Frame;

enum FrameStyle
{
	FRAME_STYLE1,
	FRAME_STYLE2,
	FRAME_STYLE3,
	FRAME_STYLE4,
	FRAME_STYLE_DEFAULT = FRAME_STYLE1,
};



typedef struct Rect
{
	union
	{
		COORD begin;
		struct
		{
			short X1;
			short Y1;
		};
	};
	union
	{
		COORD end;
		struct
		{
			short X2;
			short Y2;
		};
	};
	Frame frame;
}Rect;

typedef struct ViewRect
{
	BufferView view;
	
}ViewRect;

