#ifndef _CUSTOM_MATH_
#define _CUSTOM_MATH_
#ifndef M_PI
#include <math.h>
#endif
#ifndef FLT_MAX
#include <float.h>
#endif

#ifdef CUSTOM_MATH_USE_FIXED_POINT
#include "fptc/fptc.h"
#endif

#ifdef _FPTC_H_
#warning Using fixed point math. Nothing inherently wrong with that.
    typedef fpt real;
    #define REAL_MIN FPT_MIN
    #define REAL_MAX FPT_MAX
    #define REAL_ZERO FPT_ZERO
    #define REAL_ONE FPT_ONE
    #define fl2real(a)  fl2fpt(a)
    #define i2real(a)   i2fpt(a)
    #define real2fl(a)  fpt2fl(a)
    #define m_abs(a)    fpt_abs(a)
    #define m_sqrt(a)   fpt_sqrt(a)
    #define m_add(a,b)  (a + b)
    #define m_sub(a,b)  (a - b)
    #define m_mul(a,b)  fpt_mul(a,b)
    #define m_div(a,b)  fpt_div(a,b)
    #define m_cos(a)    fpt_cos(a)
    #define m_sin(a)    fpt_sin(a)
    #define m_tan(a)    fpt_tan(a)
    #define m_exp(a)    fpt_exp(a)
    #define m_ln(a)     fpt_ln(a)
    #define m_log(a,b)  fpt_log(a,b)
    #define m_pow(a,e)  fpt_pow(a,e)
#else
#warning Using floating point math. Nothing inherently wrong with that.
    typedef float real;
    #define REAL_MIN -FLT_MAX
    #define REAL_MAX  FLT_MAX
    #define REAL_ZERO 0.0
    #define REAL_ONE 1.0
    #define fl2real(a)  (a)
    #define i2real(a)   (float)(a)
    #define real2fl(a)  (a)
    #define m_abs(a)    fabs(a)
    #define m_sqrt(a)   sqrt(a)
    #define m_add(a,b)  (a + b)
    #define m_sub(a,b)  (a - b)
    #define m_mul(a,b)  (a*b)
    #define m_div(a,b)  (a/b)
    #define m_cos(a)    cos(a)
    #define m_sin(a)    sin(a)
    #define m_tan(a)    tan(a)
    #define m_exp(a)    exp(a)
    #define m_ln(a)     log(a)
    #define m_log(a,b)  (log(a)/log(b))
    #define m_pow(a,e)  pow(a,e)
#endif
#endif