//Dependencies

#ifndef _GEOMETRIC_ALGEBRA_H
    #define _GEOMETRIC_ALGEBRA_H

#ifdef _CUSTOM_MATH_
    #warning Using custom math library. Nothing inherently wrong with that.
#else
    #warning Using default math library. Nothing inherently wrong with that.
    #ifndef FLT_MAX
    #include <float.h>
    #endif
    #ifndef M_PI
    #include <math.h>
    #endif

    #define real float
    #define M_EPSILON (0.0001)
    #define REAL_MIN -FLT_MAX
    #define REAL_MAX  FLT_MAX
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
#endif

typedef struct
{
    real x,y,z;
}vec3, bivec3;

typedef struct
{
    real II;    //parallel component
    bivec3 T;   //perpendicular component - identified by the bivector normals
}rotor3;        //https://jacquesheunis.com/post/rotors

//Component-wise operations
vec3 vec_add(vec3 a, vec3 b);
vec3 vec_subtract(vec3 a, vec3 b);
vec3 vec_multiply(vec3 a, vec3 b);
vec3 vec_divide(vec3 a, vec3 b);
vec3 vec_scalar_multiply(vec3 a, real amount);
vec3 vec_scalar_divide(vec3 a, real amount);
vec3 vec_min(vec3 a, vec3 b);
vec3 vec_max(vec3 a, vec3 b);

//Unary operations
real vec_length_squared(vec3 a);
real vec_length(vec3 a);
unsigned char vec_is_normalized(vec3 a);
vec3 vec_normalize(vec3 a);
vec3 vec_unit_cube_fit(vec3 a);
vec3 vec_reverse(vec3 a);
vec3 vec_abs(vec3 a);

//Comparisons
vec3 vec_greater(vec3 a, vec3 b);
vec3 vec_greater_equal(vec3 a, vec3 b);
vec3 vec_equal(vec3 a, vec3 b);
vec3 vec_lesser_equal(vec3 a, vec3 b);
vec3 vec_lesser(vec3 a, vec3 b);

//Products
real vec_dot_product(vec3 a, vec3 b); //cosine-proportional magnitude between vectors
vec3 vec_cross_product(vec3 a, vec3 b); //normal between vectors with sine-proportional magnitude
rotor3 vec_geometric_product(vec3 a, vec3 b);
vec3 vec_geometric_inverse(vec3 a);

//Rotors
#define IDENTITY_ROTOR (rotor3){1, {0,0,0}}
rotor3 rotor_from_vectors(vec3 a, vec3 b);
rotor3 rotor_from_scaled_axis_angle(vec3 a);
rotor3 rotor_reverse(rotor3 r);
real rotor_dot_product(rotor3 a, rotor3 b);
rotor3 rotor_normalize(rotor3 r);
rotor3 rotor_combine(rotor3 a, rotor3 b);

//Reflections and Rotations
vec3 vec_reflection(vec3 a, vec3 v); //reflect a on v

vec3 vec_rotate_w_rotor(vec3 a, rotor3 r);
vec3 vec_rotate(vec3 a, vec3 from, vec3 to);
//TODO write tests for these VVV
vec3 vec_rotateX(vec3 a, real theta);
vec3 vec_rotateY(vec3 a, real theta);
vec3 vec_rotateZ(vec3 a, real theta);

//Interpolations
vec3 vec_simple_lerp(vec3 from, vec3 to, real t);
vec3 vec_interpolation(vec3 from, vec3 to, real t);
rotor3 rotor_interpolation(rotor3 from, rotor3 to, real t, unsigned char mode);

//Extras
unsigned char vec_are_parallel(vec3 a, vec3 b, real tolerance); //tolerance measured in percent
vec3 convert_to_coordinate_system(vec3 a, vec3 translation, rotor3 rotation);

char glsl_rotor_implementation[] = 
"void rotate_w_rotor(inout vec3 a, vec4 r)\n"
"{\n"
"    vec3 a_II = a * r.w;\n"
"    vec3 a_T  = cross(a, r.xyz);\n"
"    vec3 v = a_II + a_T;\n"
"\n"
"    float trivec = dot(r.xyz, a);\n"
"\n"
"    a.x =  v.x*r.w + v.y*r.z - v.z*r.y + trivec*r.x;\n"
"    a.y = -v.x*r.z + v.y*r.w + v.z*r.x + trivec*r.y;\n"
"    a.z =  v.x*r.y - v.y*r.x + v.z*r.w + trivec*r.z;\n"
"}\n";
#endif //_GEOMETRIC_ALGEBRA_H

//----------------------------------

#if defined(INCLUDE_IMPLEMENTATION) && !defined(_GEOMETRIC_ALGEBRA_C)
    #define _GEOMETRIC_ALGEBRA_C

// #if 0 /*defined(USE_POLYMORPHIC_VECTORS)*/
// #include "polymorphic_implementation.c"
// #else
#include "conventional_implementation.c"
// #endif

#endif