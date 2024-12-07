#ifdef DEFINITION_MODE
typedef struct s_vec3
{
    float x,y,z;
}vec3;

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
vec3 vec_rotate(vec3 a, vec3 amount);
#endif

#ifdef IMPLEMENTATION_MODE
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
    return vec_scalar_multiply(a, 1.0/vec_length(a));
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