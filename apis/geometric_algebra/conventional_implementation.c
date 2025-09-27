//Component-wise operations
FORCE_INLINE vec3 vec_add(vec3 a, vec3 b)
{
    return (vec3){a.x + b.x, a.y + b.y, a.z + b.z};
}

FORCE_INLINE vec3 vec_subtract(vec3 a, vec3 b)
{
    return (vec3){a.x - b.x, a.y - b.y, a.z - b.z};
}

FORCE_INLINE vec3 vec_multiply(vec3 a, vec3 b)
{
    return (vec3){m_mul(a.x,b.x), m_mul(a.y,b.y), m_mul(a.z,b.z)};
}

inline vec3 vec_divide(vec3 a, vec3 b)
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

FORCE_INLINE vec3 vec_scalar_multiply(vec3 a, real amount)
{
    return (vec3){m_mul(a.x,amount), m_mul(a.y,amount), m_mul(a.z,amount)};
}

FORCE_INLINE vec3 vec_scalar_divide(vec3 a, real amount)
{
    return (vec3){m_div(a.x,amount), m_div(a.y,amount), m_div(a.z,amount)};
}

inline vec3 vec_min(vec3 a, vec3 b)
{
    return (vec3){
            a.x < b.x? a.x : b.x,
            a.y < b.y? a.y : b.y,
            a.z < b.z? a.z : b.z};
}

inline vec3 vec_max(vec3 a, vec3 b)
{
    return (vec3){
        a.x > b.x? a.x : b.x,
        a.y > b.y? a.y : b.y,
        a.z > b.z? a.z : b.z};
}

//Unary operations
FORCE_INLINE real vec_length_squared(vec3 a)
{
    //3 multiplications
    //2 additions
    return m_mul(a.x,a.x) + m_mul(a.y,a.y) + m_mul(a.z,a.z);
}

FORCE_INLINE real vec_length(vec3 a)
{
    //3 multiplications
    //2 additions
    //1 square root
    return m_sqrt(vec_length_squared(a));
}

inline unsigned char vec_is_normalized(vec3 a)
{
    return m_abs(vec_length_squared(a) - fl2real(1.0)) < M_EPSILON;
}

vec3 vec_normalize(vec3 a)
{
    //7 multiplications
    //2 additions
    //1 square root
    real length = vec_length_squared(a);
    if(length < M_EPSILON) return (vec3){0};
    
    return vec_scalar_divide(a, m_sqrt(length));
}

vec3 vec_unit_cube_fit(vec3 a)
{
    real max = a.x;
    if(max < a.y) max = a.y;
    if(max < a.z) max = a.z;

    return vec_scalar_divide(a, max);
}

FORCE_INLINE vec3 vec_reverse(vec3 a)
{
    return (vec3){-a.x, -a.y, -a.z};
}
#define bivec_reverse(a) (bivec3)vec_reverse(a)

FORCE_INLINE vec3 vec_abs(vec3 a)
{
    return (vec3){m_abs(a.x), m_abs(a.y), m_abs(a.z)};
}

//Products
FORCE_INLINE real vec_dot_product(vec3 a, vec3 b)
{
    //3 multiplications
    //2 additions
    return m_mul(a.x,b.x) + m_mul(a.y,b.y) + m_mul(a.z,b.z);
}

FORCE_INLINE bivec3 vec_cross_product(vec3 a, vec3 b)
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

inline rotor3 vec_geometric_product(vec3 a, vec3 b)
{
    //9 multiplications
    //5 additions
    return (rotor3){vec_dot_product(a,b), vec_cross_product(a,b)};
}

inline vec3 vec_geometric_inverse(vec3 a)
{
    return vec_scalar_divide(a, vec_length_squared(a));
}

//Rotors
rotor3 rotor_from_vectors(vec3 from, vec3 to /*,bivec3 previous_normal*/)
{
    from = vec_normalize(from);
    to = vec_normalize(to);
    
    //no rotation returns identity rotor
    if(vec_length_squared(vec_subtract(from, to)) < M_EPSILON)
        return IDENTITY_ROTOR;

    vec3 halfway = vec_normalize(vec_add(from, to));

    //FIXME for when from and to are almost opposites
    //mirror (but w/ what roll?) (previous_normal?)
    if(vec_length_squared(halfway) < M_EPSILON)
        return IDENTITY_ROTOR;
    //or
    //     half_way = vec_cross_product(previous_normal, from);

    return vec_geometric_product(halfway, from);
}

rotor3 rotor_from_scaled_axis_angle(vec3 a)
{
    real angle = vec_length(a);
    a = vec_scalar_divide(a, angle);
    
    real half_angle = m_div(angle,2);

    return  (rotor3)
            {
                m_cos(half_angle),
                (bivec3)vec_scalar_multiply(a, m_sin(half_angle))
            };
}

FORCE_INLINE rotor3 rotor_reverse(rotor3 r)
{
    return (rotor3){r.II, bivec_reverse(r.T)};
}

inline real rotor_dot_product(rotor3 a, rotor3 b)
{
    //represents the cosine of the angle difference between the two rotors
    return m_mul(a.II,b.II) + vec_dot_product(a.T,b.T);
}

rotor3 rotor_normalize(rotor3 r)
{
    real length = m_sqrt(rotor_dot_product(r,r));

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

inline rotor3 rotor_combine(rotor3 a, rotor3 b)
{
    //16 multiplications
    //12 additions
    return  (rotor3)
            {
                m_mul(a.II,b.II) - vec_dot_product(a.T, b.T),
                (bivec3)
                {
                    m_mul(a.II,b.T.x) + m_mul(b.II,a.T.x) + m_mul(a.T.y,b.T.z) - m_mul(a.T.z,b.T.y),
                    m_mul(a.II,b.T.y) + m_mul(b.II,a.T.y) + m_mul(a.T.z,b.T.x) - m_mul(a.T.x,b.T.z),
                    m_mul(a.II,b.T.z) + m_mul(b.II,a.T.z) + m_mul(a.T.x,b.T.y) - m_mul(a.T.y,b.T.x)
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

    return vec_scalar_divide(temp, v_sq.x + v_sq.y + v_sq.z);
}

vec3 vec_rotate_w_rotor(vec3 a, rotor3 r)
{
    //rotor must be normalized (formed using unit-length vectors)
    
    //24 multiplications
    //17 additions
    
    vec3 a_II = vec_scalar_multiply(a, r.II);
    vec3 a_T  = vec_cross_product(a, r.T); 
    vec3 v = vec_add(a_II, a_T);
    
    real trivec = vec_dot_product(r.T, a);

    return  (vec3)
            {
                m_mul(v.x ,r.II ) + m_mul(v.y ,r.T.z) - m_mul(v.z ,r.T.y) + m_mul(trivec ,r.T.x),
                m_mul(v.x,-r.T.z) + m_mul(v.y ,r.II ) + m_mul(v.z ,r.T.x) + m_mul(trivec ,r.T.y),
                m_mul(v.x ,r.T.y) - m_mul(v.y ,r.T.x) + m_mul(v.z ,r.II ) + m_mul(trivec ,r.T.z)
            };
}

vec3 vec_rotate(vec3 a, vec3 from, vec3 to)
{
    return vec_rotate_w_rotor(a, rotor_from_vectors(from,to));
}

#define DEG_TO_RAD(x) m_mul(x, m_div(M_PI,180.0))

#define EULER_ROTATION(x,y) \
real temp = a.x;\
a.x = m_mul(a.x, cos(theta)) - m_mul(a.y, sin(theta));\
a.y = m_mul(temp, sin(theta)) + m_mul(a.y, cos(theta));\

vec3 vec_rotateX(vec3 a, real theta)
{
    EULER_ROTATION(y,z)
}

vec3 vec_rotateY(vec3 a, real theta)
{
    EULER_ROTATION(x,z)
}

vec3 vec_rotateZ(vec3 a, real theta)
{
    EULER_ROTATION(x,y)
}
#undef EULER_ROTATION

//Linear interpolations
vec3 vec_simple_lerp(vec3 from, vec3 to, real t)
{
    real f = fl2real(1.0) - t;
    return vec_add(vec_scalar_multiply(from,f), vec_scalar_multiply(to,t));
}

vec3 vec_interpolation(vec3 from, vec3 to, real t)
{
    real f = fl2real(1.0) - t;

    vec3 lerp_vec = vec_simple_lerp(vec_multiply(from,from), vec_multiply(to,to), t);

    return (vec3){m_sqrt(lerp_vec.x), m_sqrt(lerp_vec.y), m_sqrt(lerp_vec.z)};
}

enum{
    LINEAR_INTERPOLATION,
    SPHERICAL_INTERPOLATION
};
rotor3 rotor_interpolation(rotor3 from, rotor3 to, real t, unsigned char mode)
{
    real cos_theta = rotor_dot_product(from,to);

    if(cos_theta < fl2real(0.0))
    {
        to = (rotor3){-to.II, (bivec3){-to.T.x, -to.T.y, -to.T.z}};
        cos_theta = -cos_theta;
    }
    
    real f = fl2real(1.0) - t;

    //TODO fix this
    // if(mode == SPHERICAL_INTERPOLATION && cos_theta <= fl2real(0.99995))
    // {
    //     real theta = fl2real(acos(cos_theta));
    //     real sin_theta = m_sqrt(fl2real(1.0) - m_mul(cos_theta,cos_theta));

    //     t = m_div(m_sin(m_mul(t, theta)), sin_theta);
    //     f = fl2real(1.0) - t;
    //     // f = m_div(m_sin(m_mul(f, theta)), sin_theta); //doesnt work, meant for vectors, not rotors?
    // }
    
    rotor3 interpolated_rotor =
    (rotor3)
    {
        m_mul(from.II, f) + m_mul(to.II, t),
        (bivec3)
        {
            m_mul(from.T.x, f) + m_mul(to.T.x, t),
            m_mul(from.T.y, f) + m_mul(to.T.y, t),
            m_mul(from.T.z, f) + m_mul(to.T.z, t)
        }
    };

    return rotor_normalize(interpolated_rotor);
}

//Extras
unsigned char vec_are_parallel(vec3 a, vec3 b, real tolerance)
{
    // a = vec_normalize(a);
    // b = vec_normalize(b);
    return fl2real(1.0) - m_abs(vec_dot_product(a, b)) < tolerance;
    // return vec_length_squared(vec_subtract(a, b)) < tolerance*tolerance;
}

vec3 convert_to_coordinate_system(vec3 a, vec3 translation, rotor3 rotation)
{
    //TODO implement change of basis function
}