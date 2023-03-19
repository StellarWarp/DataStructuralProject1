#pragma once

#define _class_ Pair(T1,T2)
#define Pair(T1,T2) _TypedVar_(Pair, T1, T2)
typedef struct _class_
{
	union
	{
		T1 first;
		T1 key;
	};
	union
	{
		T2 second;
		T2 value;
	};
}_class_;

#define Template PairInit
#define PairInit(T1, T2, val1, val2) _TypedVar_(PairInit, T1, T2)(val1, val2)
_class_ _template_(T1 val1, T2 val2);
#ifdef Pair_CF
_class_ _template_(param)
{
	_class_ ret;
	ret.first = val1;
	ret.second = val2;
	return ret;
}
#endif
