#ifndef API_IMPLEMENTATION_ONLY
#ifndef M_PI
#include <math.h>
#endif

#ifndef VEC3
#define VEC3
#endif
typedef struct
{
    float x,y,z;
}vec3;

typedef struct
{
    float II;   //parallel
    vec3 T;     //perpendicular
}rotor3;

//Component-wise operations
vec3 vec_add(vec3 a, vec3 b);
vec3 vec_subtract(vec3 a, vec3 b);
vec3 vec_multiply(vec3 a, vec3 b);
vec3 vec_divide(vec3 a, vec3 b);

//Unary operations
float vec_length_squared(vec3 a);
float vec_length(vec3 a);
vec3 vec_scalar_multiply(vec3 a, float amount);
vec3 vec_normalize(vec3 a);
vec3 vec_geometric_inverse(vec3 a);

//Products
float vec_dot_product(vec3 a, vec3 b); //cosine between vectors
vec3 vec_cross_product(vec3 a, vec3 b); //normal between vectors with sine magnitude
rotor3 vec_geometric_product(vec3 a, vec3 b);

//Rotors
rotor3 rotor_reverse(rotor3 a);
rotor3 rotor_combine(rotor3 a, rotor3 b);

//Reflections and Rotations
vec3 vec_reflection(vec3 a, vec3 b);
vec3 vec_rotate(vec3 a, vec3 amount);

//----------------------------------
#else
//----------------------------------

//Component-wise operations
#define _vec_add(a,b) (vec3){a.x + b.x, a.y + b.y, a.z + b.z}
vec3 vec_add(vec3 a, vec3 b)
{
    return _vec_add(a,b);
}

#define _vec_subtract(a,b) (vec3){a.x - b.x, a.y - b.y, a.z - b.z}
vec3 vec_subtract(vec3 a, vec3 b)
{
    return _vec_subtract(a,b);
}

#define _vec_multiply(a,b) (vec3){a.x*b.x, a.y*b.y, a.z*b.z}
vec3 vec_multiply(vec3 a, vec3 b)
{
    return _vec_multiply(a,b);
}

#define _vec_divide(a,b) (vec3){a.x/b.x, a.y/b.y, a.z/b.z}
vec3 vec_divide(vec3 a, vec3 b)
{
    return _vec_divide(a,b);
}

#define _vec_scalar_multiply(a, k) (vec3){a.x*k, a.y*k, a.z*k}
vec3 vec_scalar_multiply(vec3 a, float amount)
{
    return _vec_scalar_multiply(a, amount);
}

//Unary operations
#define _vec_length_squared(a) (a.x*a.x + a.y*a.y + a.z*a.z)
float vec_length_squared(vec3 a)
{
    return _vec_length_squared(a);
}

#define _vec_length(a) sqrt(_vec_length_squared(a))
float vec_length(vec3 a)
{
    return _vec_length(a);
}

vec3 vec_normalize(vec3 a)
{
    float length = _vec_length(a);
    if(length < 0.0000001) return (vec3){0};

    return _vec_scalar_multiply(a, 1.0/length);
}
#define _vec_normalize(a) vec_normalize(a)

#define _vec_reverse(a) (vec3){-a.x, -a.y, -a.z}
vec3 vec_reverse(vec3 a)
{
    return _vec_reverse(a);
}

//Products
#define _vec_dot_product(a,b) (a.x*b.x + a.y*b.y + a.z*b.z)
float vec_dot_product(vec3 a, vec3 b) //cosine between vectors
{
    //3 multiplications
    //2 additions
    return _vec_dot_product(a,b);
}
#define _vec_dot_inverse(a) (vec3){1/(3*a.x), 1/(3*a.y), 1/(3*a.z)}
vec3 vec_dot_inverse(vec3 a)
{
    return _vec_dot_inverse(a);
}

#define _vec_cross_product(a,b) (vec3){a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x}
vec3 vec_cross_product(vec3 a, vec3 b) //normal between vectors with sine magnitude
{
    //6 multiplications
    //3 additions
    return _vec_cross_product(a,b);
}
//rotor3 vec_cross_inverse(a,b)?

#define _vec_geometric_product(a,b) (rotor3){_vec_dot_product(a,b), _vec_cross_product(a,b)}
rotor3 vec_geometric_product(vec3 a, vec3 b)
{
    //9 multiplications
    //5 additions
    return _vec_geometric_product(a,b);
}
#define _vec_geometric_inverse(a) _vec_scalar_multiply(a, 1.0/_vec_length_squared(a))
vec3 vec_geometric_inverse(vec3 a)
{
    return _vec_geometric_inverse(a);
}

//Rotors
#define _rotor_reverse(a) (rotor3){a.II, _vec_reverse(a.T)}
rotor3 rotor_reverse(rotor3 a)
{
    return _rotor_reverse(a);
}

#define _rotor_combine(a,b) (rotor3)\
                            {\
                                a.II*b.II - _vec_dot_product(a.T, b.T),\
                                (vec3)\
                                {\
                                    a.II*b.T.z + a.T.z*b.II  - a.T.x*b.T.y + a.T.y*b.T.x,\
                                    a.II*b.T.x + a.T.z*b.T.y + a.T.x*b.II  - a.T.y*b.T.z,\
                                    a.II*b.T.y - a.T.z*b.T.x + a.T.x*b.T.z + a.T.y*b.II\
                                }\
                            }
rotor3 rotor_combine(rotor3 a, rotor3 b)
{
    //16 multiplications
    //12 additions
    return  _rotor_combine(a,b);
}

//Reflections and Rotations
vec3 vec_reflection(vec3 a, vec3 b)
{
    //19 multiplications
    //12 additions
    vec3 a_sq = vec_multiply(a,a);
    vec3 dot_v = (vec3){a.x*b.x, a.y*b.y, a.z*b.z};
    vec3 temp = {   (a_sq.x - a_sq.y - a_sq.z)*b.x + 2*a.x*(dot_v.y + dot_v.z),
                    (-a_sq.x + a_sq.y - a_sq.z)*b.y + 2*a.y*(dot_v.x + dot_v.z),
                    (-a_sq.x - a_sq.y + a_sq.z)*b.z + 2*a.z*(dot_v.x + dot_v.y)};

    return vec_scalar_multiply(temp, 1/vec_length_squared(a));
}
#define _vec_reflection(a,b) vec_reflection(a,b)

#define DEG_TO_RAD(x) x*M_PI/180.0
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
#define _vec_rotate(a,b) vec_rotate(a,b)
#endif