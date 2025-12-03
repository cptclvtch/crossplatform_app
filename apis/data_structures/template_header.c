/*
    PROS:
    - provides type checking and static code analysis
    - compatible with breakpoints and debugging
    - void* default
    CONS:
    - no template nesting
    - no include guards
    - complicates the build process (requires more compilation & correct order of type definitions)
*/

//TODO test deffered implementations on multiple data structures of the same type

#ifndef CONCATENATE
    #define CPP_IS_STUPID(a,b) a##b
    #define CONCATENATE(a,b) CPP_IS_STUPID(a,b)
#endif

//
#if !defined(DESIRED_TYPE) && defined(DESIRED_PREFIX)
#error Cant have DESIRED_PREFIX defined but not DESIRED_TYPE
#endif

#if !defined(DESIRED_TYPE)
//this provides TYPE and PREFIX defaults
    #define TYPE void*
    #define PREFIX
    #define USING_VOIDP_DEFAULT
#else
    #define TYPE DESIRED_TYPE

    #if !defined(DESIRED_PREFIX)
        #define DESIRED_PREFIX TYPE
    #endif

    #define PREFIX CONCATENATE(DESIRED_PREFIX,_)
#endif

#ifndef ASSISTING_TYPE
    #define ASSISTING_TYPE void*
#endif