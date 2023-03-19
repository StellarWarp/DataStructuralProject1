#pragma once
//协程

/*
 * `sco' macros for static coroutines.
 * 全局唯一
 * 使用后需要#undef STATIC_COROUTINE
 */
#ifdef SCO

#define co_begin         static int co_line = 0; switch(co_line) { case 0:;
#define co_end(ret)     } return ret

//#define co_end_void       } return

#define co_return(ret)     \
        do {\
            co_line=__LINE__;\
            return ret; case __LINE__:;\
        } while (0)
//#define co_return_void       \
//        do {\
//            co_line=__LINE__;\
//            return; case __LINE__:;\
//        } while (0)

#define co_stop(ret)       do{ co_line = 0; return ret; }while(0)

#endif // STATIC_COROUTINE

 /*
  * `rco' macros for re-entrant coroutines.
  * 使用上下文结构体指针作为参数进行状态恢复
  * 使用后需要#undef RE_ENTRANT_COROUTINE
  */
#ifdef RCO

#define CoroutineParam     void **co_param

#define co_context_begin  struct coContext { int co_line
#define co_context_end(x) } *x = (struct coContext *)*co_param

//
#define co_begin(x)      if(!x) {x= *co_param=malloc(sizeof(*x)); x->co_line=0;}\
                         if (x) switch(x->co_line) { case 0:;
#define co_end(ret)     } free(*co_param); *co_param=0; return ret
//#define co_end_void       } free(*co_param); *co_param=0; return

#define co_return(ret)     \
        do {\
            ((struct coContext *)*co_param)->co_line=__LINE__;\
            return ret; case __LINE__:;\
        } while (0)
//#define co_return_void       \
//        do {\
//            ((struct coContext *)*co_param)->co_line=__LINE__;\
//            return; case __LINE__:;\
//        } while (0)

#define co_stop(ret)       do{ free(*co_param); *co_param=0; return ret; }while(0)
//#define co_stop_void         do{ free(*co_param); *co_param=0; return; }while(0)

#endif //RE_ENTRANT_COROUTINE

typedef void* co_context;
#define co_abort(ctx)    do { free (ctx); ctx = 0; } while (0)