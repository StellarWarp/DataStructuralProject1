#include "UI.h"
#include "Console.h"

Rect RectInit_ser(short x1, short y1, short x2, short y2)
{
	Rect rect;
	rect.X1 = x1;
	rect.Y1 = y1;
	rect.X2 = x2;
	rect.Y2 = y2;
	return rect;
}

Rect RectInit_se(COORD begin, COORD end)
{
	Rect rect;
	rect.begin = begin;
	rect.end = end;
	return rect;
}

Rect RectInit_size(COORD begin, COORD size)
{
	Rect rect;
	rect.begin = begin;
	COORD end = { begin.X + size.X - 1, begin.Y + size.Y - 1 };
	rect.end = end;
	return rect;
}

COORD RectSize(Rect* rect)
{
	COORD size;
	size.X = rect->X2 - rect->X1 + 1;
	size.Y = rect->Y2 - rect->Y1 + 1;
	return size;
}

COORD RectInternalSize(Rect* rect)
{
	COORD size;
	size.X = rect->X2 - rect->X1 - 1;
	size.Y = rect->Y2 - rect->Y1 - 1;
	return size;
}

COORD RectInternalBegin(Rect* rect)
{
	COORD begin = { rect->X1 + 1, rect->Y1 + 1 };
	return begin;
}

COORD RectInternalEnd(Rect* rect)
{
	COORD end = { rect->X2 - 1, rect->Y2 - 1 };
	return end;
}

Frame FrameInit(enum FrameStyle style, WORD color)
{
	Frame frame;
	frame.color = color;
	switch (style)
	{
	default:
	case FRAME_STYLE1:
	{
		frame.u = L'─';
		frame.d = L'─';
		frame.r = L'│';
		frame.l = L'│';
		frame.ul = L'┌';
		frame.ur = L'┐';
		frame.dl = L'└';
		frame.dr = L'┘';
	}
	break;
	case FRAME_STYLE2:
	{
		frame.u = L'═';
		frame.d = L'═';
		frame.r = L'║';
		frame.l = L'║';
		frame.ul = L'╔';
		frame.ur = L'╗';
		frame.dl = L'╚';
		frame.dr = L'╝';
	}
	break;
	case FRAME_STYLE3:
	{
		frame.u = L'━';
		frame.d = L'━';
		frame.r = L'┃';
		frame.l = L'┃';
		frame.ul = L'┏';
		frame.ur = L'┓';
		frame.dl = L'┗';
		frame.dr = L'┛';
		break;
	}
	case FRAME_STYLE4:
	{
		frame.u = L'─';
		frame.d = L'─';
		frame.r = L'│';
		frame.l = L'│';
		frame.ul = L'┎';
		frame.ur = L'┒';
		frame.dl = L'┖';
		frame.dr = L'┚';
		break;
	}
	}
	return frame;
}

void RectDirectDraw(Rect* rect)
{
	console.SetTextColor(rect->frame.color);
	Frame* f = &rect->frame;
	COORD pos = rect->begin;
	COORD internalSize = RectInternalSize(rect);
	console.PrintChar(pos, f->ul);
	for (int i = 0; i < internalSize.X; i++)
	{
		//put wchar
		putwchar(f->u);
	}
	putwchar(f->ur);
	pos.Y++;
	for (int i = 0; i < internalSize.Y; i++)
	{
		console.PrintChar(pos, f->l);
		pos.X = rect->X2;
		console.PrintChar(pos, f->r);
		pos.Y++;
		pos.X = rect->X1;
	}
	console.PrintChar(pos, f->dl);
	for (int i = 0; i < internalSize.X; i++)
	{
		//put wchar
		putwchar(f->d);
	}
	putwchar(f->dr);
}