#ifndef API_IMPLEMENTATION_ONLY
#ifndef M_PI
#include <math.h>
#endif

#ifndef VEC3
#define VEC3
typedef struct
{
    float x,y,z;
}vec3;
#endif

#define ITERATE_OVER_VECTOR_MEMBERS(a, op, b)\
    a.x op b.x;\
    a.y op b.y;\
    a.z op b.z;

float vec_length_squared(vec3 a);
float vec_length(vec3 a);
vec3 vec_scalar_multiply(vec3 a, float amount);
vec3 vec_normalize(vec3 a);

vec3 vec_add(vec3 a, vec3 b);
vec3 vec_subtract(vec3 a, vec3 b);
vec3 vec_multiply(vec3 a, vec3 b);
vec3 vec_divide(vec3 a, vec3 b);
float vec_dot_product(vec3 a, vec3 b);
vec3 vec_cross_product(vec3 a, vec3 b);

vec3 vec_rotate(vec3 a, vec3 amount);

//----------------------------------
#else
//----------------------------------

float vec_length_squared(vec3 a)
{
    return a.x*a.x + a.y*a.y + a.z*a.z;
}

float vec_length(vec3 a)
{
    return sqrt(vec_length_squared(a));
}

vec3 vec_scalar_multiply(vec3 a, float amount)
{
    a.x *= amount;
    a.y *= amount;
    a.z *= amount;

    return a;
}

vec3 vec_normalize(vec3 a)
{
    float length = vec_length(a);
    if(length < 0.0000001) return (vec3){0};

    return vec_scalar_multiply(a, 1.0/length);
}

vec3 vec_add(vec3 a, vec3 b)
{
    ITERATE_OVER_VECTOR_MEMBERS(a, +=, b)

    return a;
}

vec3 vec_subtract(vec3 a, vec3 b)
{
    ITERATE_OVER_VECTOR_MEMBERS(a, -=, b)

    return a;
}

vec3 vec_multiply(vec3 a, vec3 b)
{
    ITERATE_OVER_VECTOR_MEMBERS(a, *=, b)

    return a;
}

vec3 vec_divide(vec3 a, vec3 b)
{
    ITERATE_OVER_VECTOR_MEMBERS(a, /=, b)

    return a;
}

float vec_dot_product(vec3 a, vec3 b)
{
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

vec3 vec_cross_product(vec3 a, vec3 b)
{
    return (vec3){  a.y*b.z - a.z*b.y,
                    a.z*b.x - a.x*b.z,
                    a.x*b.y - a.y*b.x};
}

#define DEG_TO_RAD(x) x*3.1415/180.0
vec3 vec_rotate(vec3 a, vec3 amount)
{
    const float error = 1.0/10000.0;
    float temp;

    //roll - X axis
    if(amount.z < -error || amount.z > error)
    {
        temp = a.y;
        a.y = a.y*cos(amount.z) - a.z*sin(amount.z);
        a.z = temp*sin(amount.z) + a.z*cos(amount.z);
    }

    //pitch - Y axis
    if(amount.y < -error || amount.y > error)
    {
        temp = a.x;
        a.x = a.x*cos(amount.y) - a.z*sin(amount.y);
        a.z = temp*sin(amount.y) + a.z*cos(amount.y);
    }

    //yaw - Z axis
    if(amount.x < -error || amount.x > error)
    {
        temp = a.x;
        a.x = a.x*cos(amount.x) - a.y*sin(amount.x);
        a.y = temp*sin(amount.x) + a.y*cos(amount.x);
    }

    return a;
}
#endif