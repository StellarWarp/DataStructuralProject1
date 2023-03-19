#include "Macro.h"

//private
#ifndef TYPED_DEFINE_H
#define TYPED_DEFINE_H

#define _template_ MACRO_CAT(Template,_,_type_)
#define _template2_ MACRO_CAT(Template,_,_type1_,_,_type2_)
#define _template3_ MACRO_CAT(Template,_,_type1_,_,_type2_,_,_type3_)

//class extend
#define _class_struct_ struct _class_
#pragma warning(disable : 4005)

#endif // TYPED_DEFINE_H

//public
#ifndef TYPED_VAR_H
#define TYPED_VAR_H

#define _TypedVar_4(val,type1,type2,type3) MACRO_CAT(val,_,type1,_,type2,_,type3)
#define _TypedVar_3(val,type1,type2) MACRO_CAT(val,_,type1,_,type2)
#define _TypedVar_2(val,type) MACRO_CAT(val,_,type)
#define _TypedVar_(val,...) VA_MACRO(_TypedVar,val,__VA_ARGS__)

//#define _TypedVar_(val,...) MACRO_CAT(val,_,__VA_ARGS__)



#endif // TYPED_VAR_H
/*
example:
//...template.h
#define Template function
#define function(T, param_real) _TypedVar_(function, T)(param_real)
void _template_(param);
//...core.h
#define Template function
void _template_(param){...}

//....h
#define T int
#include "template.h"
#undef T
//....c
#define T int
#include "core.h"
#undef T
*/