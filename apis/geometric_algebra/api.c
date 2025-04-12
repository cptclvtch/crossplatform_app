#ifndef API_IMPLEMENTATION_ONLY
#ifndef GEOMETRIC_ALGEBRA
#define GEOMETRIC_ALGEBRA
#endif

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

//Unary operations
real vec_length_squared(vec3 a);
real vec_length(vec3 a);
unsigned char vec_is_normalized(vec3 a);
vec3 vec_normalize(vec3 a);
vec3 vec_reverse(vec3 a);

//Products
real vec_dot_product(vec3 a, vec3 b); //cosine-proportional magnitude between vectors
vec3 vec_cross_product(vec3 a, vec3 b); //normal between vectors with sine-proportional magnitude
rotor3 vec_geometric_product(vec3 a, vec3 b);
vec3 vec_geometric_inverse(vec3 a);

//Rotors
#define IDENTITY_ROTOR (rotor3){fl2real(1.0), (bivec3){0}}
rotor3 rotor_from_vectors(vec3 a, vec3 b);
rotor3 rotor_reverse(rotor3 r);
real rotor_dot_product(rotor3 a, rotor3 b);
rotor3 rotor_normalize(rotor3 r);
rotor3 rotor_combine(rotor3 a, rotor3 b);

//Reflections and Rotations
vec3 vec_reflection(vec3 a, vec3 v); //reflect a on v

vec3 vec_rotate_w_rotor(vec3 a, rotor3 r);
vec3 vec_rotate(vec3 a, vec3 from, vec3 to);
vec3 vec_rotateX(vec3 a, real theta);
vec3 vec_rotateY(vec3 a, real theta);
vec3 vec_rotateZ(vec3 a, real theta);

// vec3 vec_rotate(vec3 a, vec3 amount);

//----------------------------------
#else
//----------------------------------

//Component-wise operations
vec3 vec_add(vec3 a, vec3 b)
{
    return (vec3){a.x + b.x, a.y + b.y, a.z + b.z};
}

vec3 vec_subtract(vec3 a, vec3 b)
{
    return (vec3){a.x - b.x, a.y - b.y, a.z - b.z};
}

vec3 vec_multiply(vec3 a, vec3 b)
{
    return (vec3){m_mul(a.x,b.x), m_mul(a.y,b.y), m_mul(a.z,b.z)};
}

vec3 vec_divide(vec3 a, vec3 b)
{
    real min_max[2] = {REAL_MIN, REAL_MAX};
    
    vec3 result = {min_max[a.x>=0], min_max[a.y>=0], min_max[a.z>=0]}; //0 is positive, idc what you say

    if(b.x)
        result.x = m_div(a.x,b.x);

    if(b.y)
        result.y = m_div(a.y,b.y);

    if(b.z)
        result.z = m_div(a.z,b.z);

    return result;
}
 
vec3 vec_scalar_multiply(vec3 a, real amount)
{
    return (vec3){m_mul(a.x,amount), m_mul(a.y,amount), m_mul(a.z,amount)};
}

//Unary operations
#define _vec_length_squared(a) (m_mul(a.x,a.x) + m_mul(a.y,a.y) + m_mul(a.z,a.z))
real vec_length_squared(vec3 a)
{
    //3 multiplications
    //2 additions
    return _vec_length_squared(a);
}

#define _vec_length(a) m_sqrt(_vec_length_squared(a))
real vec_length(vec3 a)
{
    //3 multiplications
    //2 additions
    //1 square root
    return _vec_length(a);
}

#define _vec_is_normalized(a) (m_abs(_vec_length_squared(a) - fl2real(1.0)) < fl2real(0.0001))
unsigned char vec_is_normalized(vec3 a)
{
    return _vec_is_normalized(a);
}

vec3 vec_normalize(vec3 a)
{
    //7 multiplications
    //2 additions
    //1 square root
    real length = _vec_length_squared(a);
    if(length < fl2real(0.0001)) return (vec3){0};
    
    return vec_scalar_multiply(a, m_div(fl2real(1.0), m_sqrt(length)));
}

vec3 vec_reverse(vec3 a)
{
    return (vec3){-a.x, -a.y, -a.z};
}
#define bivec_reverse(a) (bivec3)vec_reverse(a)

//Products
#define _vec_dot_product(a,b) (m_mul(a.x,b.x) + m_mul(a.y,b.y) + m_mul(a.z,b.z))
real vec_dot_product(vec3 a, vec3 b)
{
    //3 multiplications
    //2 additions
    return _vec_dot_product(a,b);
}

bivec3 vec_cross_product(vec3 a, vec3 b)
{
    //6 multiplications
    //3 additions
    return  (bivec3)
            {
                m_mul(a.y,b.z) - m_mul(a.z,b.y),
                m_mul(a.z,b.x) - m_mul(a.x,b.z),
                m_mul(a.x,b.y) - m_mul(a.y,b.x)
            };
}

rotor3 vec_geometric_product(vec3 a, vec3 b)
{
    //9 multiplications
    //5 additions
    return (rotor3){_vec_dot_product(a,b), vec_cross_product(a,b)};
}

vec3 vec_geometric_inverse(vec3 a)
{
    return vec_scalar_multiply(a, m_div(fl2real(1.0), _vec_length_squared(a)));
}

//TODO add optional previous_normal?
//Rotors
rotor3 rotor_from_vectors(vec3 from, vec3 to /*,bivec3 previous_normal*/)
{
    from = vec_normalize(from);
    to = vec_normalize(to);
    
    //no rotation returns identity rotor
    if(_vec_length_squared(vec_subtract(from, to)) < fl2real(0.000001))
        return IDENTITY_ROTOR;

    vec3 halfway = vec_normalize(vec_add(from, to));

    //FIXME for when from and to are almost opposites
    //mirror (but w/ what roll?) (previous_normal?)
    if(_vec_length_squared(halfway) < fl2real(0.000001))
        return IDENTITY_ROTOR;
    //or
    //     half_way = _vec_cross_product(previous_normal, from);

    return vec_geometric_product(from, halfway);
}

rotor3 rotor_reverse(rotor3 r)
{
    return (rotor3){r.II, bivec_reverse(r.T)};
}

#define _rotor_dot_product(a,b) (m_mul(a.II,b.II) + _vec_dot_product(a.T,b.T))
real rotor_dot_product(rotor3 a, rotor3 b)
{
    //represents the cosine of the angle difference between the two rotors
    return _rotor_dot_product(a,b);
}

rotor3 rotor_normalize(rotor3 r)
{
    real length = m_sqrt(_rotor_dot_product(r,r));

    return  (rotor3)
            {
                m_div(r.II, length),
                (bivec3)    
                {
                    m_div(r.T.x, length),
                    m_div(r.T.y, length),
                    m_div(r.T.z, length)
                }
            };
}

rotor3 rotor_combine(rotor3 a, rotor3 b)
{
    //16 multiplications
    //12 additions
    return  (rotor3)
            {
                m_mul(a.II,b.II) - _vec_dot_product(a.T, b.T),
                (bivec3)
                {
                    m_mul(a.II,b.T.z) + m_mul(a.T.z,b.II) - m_mul(a.T.x,b.T.y) + m_mul(a.T.y,b.T.x),
                    m_mul(a.II,b.T.x) + m_mul(a.T.z,b.T.y) + m_mul(a.T.x,b.II) - m_mul(a.T.y,b.T.z),
                    m_mul(a.II,b.T.y) - m_mul(a.T.z,b.T.x) + m_mul(a.T.x,b.T.z) + m_mul(a.T.y,b.II)
                }
            };
}

//Reflections and Rotations
vec3 vec_reflection(vec3 a, vec3 v)
{
    //22 multiplications
    //14 additions
    vec3 v_sq = vec_multiply(v,v);
    vec3 dot_v = vec_multiply(v,a);
    vec3 temp = (vec3){ m_mul((v_sq.x - v_sq.y - v_sq.z),a.x) + m_mul(m_mul(fl2real(2.0),v.x),(dot_v.y + dot_v.z)),
                        m_mul((-v_sq.x + v_sq.y - v_sq.z),a.y) + m_mul(m_mul(fl2real(2.0),v.y),(dot_v.x + dot_v.z)),
                        m_mul((-v_sq.x - v_sq.y + v_sq.z),a.z) + m_mul(m_mul(fl2real(2.0),v.z),(dot_v.x + dot_v.y))};

    return vec_scalar_multiply(temp, fl2real(1.0)/vec_length_squared(v));
}

vec3 vec_rotate_w_rotor(vec3 a, rotor3 r)
{
    //rotor must be formed using unit length vectors
    
    //24 multiplications
    //17 additions
    
    vec3 a_II = vec_scalar_multiply(a, r.II);
    vec3 a_T  = vec_cross_product(a, r.T); 
    vec3 v = vec_add(a_II, a_T);
    
    real trivec = _vec_dot_product(r.T, a);

    return  (vec3)
            {
                m_mul(v.x ,r.II ) + m_mul(v.y ,r.T.z) - m_mul(v.z ,r.T.y) + m_mul(trivec ,r.T.x),
                m_mul(-v.x,r.T.z) + m_mul(v.y ,r.II ) + m_mul(v.z ,r.T.x) + m_mul(trivec ,r.T.y),
                m_mul(v.x ,r.T.y) - m_mul(v.y ,r.T.x) + m_mul(v.z ,r.II ) + m_mul(trivec ,r.T.z)
            };
}

#define DEG_TO_RAD(x) x*M_PI/180.0
// vec3 vec_rotate(vec3 a, vec3 amount)
// {
//     //12 multiplications
//     //6 additions
//     const real error = 1.0/10000.0;
//     real temp;

//     //roll - X axis
//     if(amount.z < -error || amount.z > error)
//     {
//         temp = a.y;
//         a.y = a.y*cos(amount.z) - a.z*sin(amount.z);
//         a.z = temp*sin(amount.z) + a.z*cos(amount.z);
//     }

//     //pitch - Y axis
//     if(amount.y < -error || amount.y > error)
//     {
//         temp = a.x;
//         a.x = a.x*cos(amount.y) - a.z*sin(amount.y);
//         a.z = temp*sin(amount.y) + a.z*cos(amount.y);
//     }

//     //yaw - Z axis
//     if(amount.x < -error || amount.x > error)
//     {
//         temp = a.x;
//         a.x = a.x*cos(amount.x) - a.y*sin(amount.x);
//         a.y = temp*sin(amount.x) + a.y*cos(amount.x);
//     }

//     return a;
// }

//Linear interpolations
vec3 vec_lerp(vec3 from, vec3 to, real t)
{
    real f = fl2real(1.0) - t;
    return  vec_add(vec_scalar_multiply(from,f), vec_scalar_multiply(to,t));
}

rotor3 rotor_lerp(rotor3 from, rotor3 to, real t)
{
    real length_sq = rotor_dot_product(from,to);

    if(length_sq < fl2real(0.0)) from = (rotor3){-from.II, (bivec3){-from.T.x, -from.T.y, -from.T.z}};

    real f = fl2real(1.0) - t;

    rotor3 interpolated_rotor =
    (rotor3)
    {
        m_mul(from.II,f) + m_mul(to.II,t),
        (bivec3)
        {
            m_mul(from.T.x,f) + m_mul(to.T.x,t),
            m_mul(from.T.y,f) + m_mul(to.T.y,t),
            m_mul(from.T.z,f) + m_mul(to.T.z,t)
        }
    };

    return rotor_normalize(interpolated_rotor);
}

rotor3 rotor_hq_lerp(rotor3 from, rotor3 to, real t)
{
    //High quality, spherical linear interpolation
    real cos_theta = rotor_dot_product(from,to);
    real theta = fl2real(acos(cos_theta));
    real sin_theta = m_sqrt(fl2real(1.0) - m_mul(cos_theta,cos_theta));

    real f = m_div(m_sin(m_mul(fl2real(1.0) - t, theta)), sin_theta);
    t = m_sin(t*theta)/sin_theta;

    return  (rotor3)
            {
                m_mul(from.II,f) + m_mul(to.II,t),
                (bivec3)
                {
                    m_mul(from.T.x,f) + m_mul(to.T.x,t),
                    m_mul(from.T.y,f) + m_mul(to.T.y,t),
                    m_mul(from.T.z,f) + m_mul(to.T.z,t)
                }
            };
}
#endif