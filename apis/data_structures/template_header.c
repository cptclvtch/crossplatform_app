#define CPP_IS_STUPID(a,b) a##_##b
#define CONCATENATE(a,b) CPP_IS_STUPID(a,b)

//
#if !defined(TYPE) && defined(PREFIX)
#error Can't have PREFIX defined but not TYPE
#endif

//this provides TYPE and PREFIX defaults
#ifndef TYPE
    #define TYPE void*
    #define PREFIX a
    #define USING_VOIDP_DEFAULT
#else
    #ifndef PREFIX
        #define PREFIX TYPE
    #endif
#endif