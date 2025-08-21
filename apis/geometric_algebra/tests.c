#include "../unit_testing.c"
#include "api.c"
#define INCLUDE_IMPLEMENTATION
#include "api.c"

int main()
{    
    CLEAR_SCREEN

    real f_result;
    vec3 result;
    bivec3 b_result;
    rotor3 r_result;
    vec3 a = (vec3){fl2real(1.0),fl2real(1.0),fl2real(1.0)};
    vec3 x = (vec3){fl2real(1.0),fl2real(0.0),fl2real(0.0)};
    vec3 y = (vec3){fl2real(0.0),fl2real(1.0),fl2real(0.0)};
    vec3 z = (vec3){fl2real(0.0),fl2real(0.0),fl2real(1.0)};
    vec3 neg = (vec3){fl2real(-1.0),fl2real(-1.0),fl2real(-1.0)};
    vec3 zero = (vec3){fl2real(0.0),fl2real(0.0),fl2real(0.0)};
    vec3 two = (vec3){fl2real(2.0),fl2real(2.0),fl2real(2.0)};

    #define TITLE "component wise operations"
    POST_TITLE

    #define SUBTITLE "vec_add"
    result = vec_add(a, x);
    VERIFY_SINGLE_VALUE(result.x, ==, fl2real(2.0))
    VERIFY_SINGLE_VALUE(result.y, ==, fl2real(1.0))
    VERIFY_SINGLE_VALUE(result.z, ==, fl2real(1.0))
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "vec_subtract"
    result = vec_subtract(a, x);
    VERIFY_SINGLE_VALUE(result.x, ==, fl2real(0.0))
    VERIFY_SINGLE_VALUE(result.y, ==, fl2real(1.0))
    VERIFY_SINGLE_VALUE(result.z, ==, fl2real(1.0))
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "vec_multiply"
    result = vec_multiply(a, two);
    VERIFY_SINGLE_VALUE(result.x, ==, two.x)
    VERIFY_SINGLE_VALUE(result.y, ==, two.y)
    VERIFY_SINGLE_VALUE(result.z, ==, two.z)
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "vec_divide"
    result = vec_divide(a, two);
    VERIFY_SINGLE_VALUE(result.x, ==, m_div(1.0,2.0))
    VERIFY_SINGLE_VALUE(result.y, ==, m_div(1.0,2.0))
    VERIFY_SINGLE_VALUE(result.z, ==, m_div(1.0,2.0))
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "positive vec_divide by 0"
    result = vec_divide(a, zero);
    VERIFY_SINGLE_VALUE(result.x, ==, REAL_MAX)
    VERIFY_SINGLE_VALUE(result.y, ==, REAL_MAX)
    VERIFY_SINGLE_VALUE(result.z, ==, REAL_MAX)
    COLLECT_FINDINGS
    
    #undef SUBTITLE
    #define SUBTITLE "negative vec_divide by 0"
    result = vec_divide(neg, zero);
    VERIFY_SINGLE_VALUE(result.x, ==, REAL_MIN)
    VERIFY_SINGLE_VALUE(result.y, ==, REAL_MIN)
    VERIFY_SINGLE_VALUE(result.z, ==, REAL_MIN)
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "vec_scalar_multiply"
    result = vec_scalar_multiply(a, fl2real(3.0));
    VERIFY_SINGLE_VALUE(result.x, ==, fl2real(3.0))
    VERIFY_SINGLE_VALUE(result.y, ==, fl2real(3.0))
    VERIFY_SINGLE_VALUE(result.z, ==, fl2real(3.0))
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "vec_scalar_divide"
    result = vec_scalar_divide(a, fl2real(2.0));
    VERIFY_SINGLE_VALUE(result.x, ==, fl2real(0.5))
    VERIFY_SINGLE_VALUE(result.y, ==, fl2real(0.5))
    VERIFY_SINGLE_VALUE(result.z, ==, fl2real(0.5))
    COLLECT_FINDINGS

    #undef TITLE
    #define TITLE "unary operators"
    POST_TITLE

    #undef SUBTITLE
    #define SUBTITLE "vec_length_squared"
    f_result = vec_length_squared(a);
    VERIFY_SINGLE_VALUE(f_result, ==, fl2real(3.0))
    COLLECT_FINDINGS

    f_result = vec_length_squared((vec3){fl2real(1.0),fl2real(2.0),fl2real(3.0)});
    VERIFY_SINGLE_VALUE(f_result, ==, fl2real(1.0 + 4.0 + 9.0))
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "vec_length"
    f_result = vec_length(a);
    VERIFY_SINGLE_VALUE(m_abs(f_result - m_sqrt(fl2real(3.0))), < , M_EPSILON)
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "vec_is_normalized"
    VERIFY_SINGLE_VALUE(vec_is_normalized(a), ==, 0)
    VERIFY_SINGLE_VALUE(vec_is_normalized(x), ==, 1)
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "vec_normalize"
    result = vec_normalize(a);
    VERIFY_SINGLE_VALUE(vec_is_normalized(result), ==, 1)
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "vec_reverse"
    result = vec_reverse(a);
    VERIFY_SINGLE_VALUE(result.x, ==, neg.x)
    VERIFY_SINGLE_VALUE(result.y, ==, neg.y)
    VERIFY_SINGLE_VALUE(result.z, ==, neg.z)
    COLLECT_FINDINGS

    #undef TITLE
    #define TITLE "vector products"
    POST_TITLE

    #undef SUBTITLE
    #define SUBTITLE "vec_dot_product"
    f_result = vec_dot_product(a,x);
    VERIFY_SINGLE_VALUE(f_result, ==, a.x)
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "vec_cross_product"
    result = vec_cross_product(a,x);
    VERIFY_SINGLE_VALUE(m_abs(vec_length(result)\
                                -\
                                m_sqrt(vec_length(a)*vec_length(a)\
                                    -\
                                    vec_dot_product(a,x)*vec_dot_product(a,x))\
                                *vec_length(x)),\
     < , M_EPSILON)
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "vec_geometric_product"
    r_result = vec_geometric_product(x,y);
    VERIFY_SINGLE_VALUE(r_result.II, ==, fl2real(0.0))
    VERIFY_SINGLE_VALUE(r_result.T.x, ==, z.x)
    VERIFY_SINGLE_VALUE(r_result.T.y, ==, z.y)
    VERIFY_SINGLE_VALUE(r_result.T.z, ==, z.z)
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "vec_geometric_inverse"
    result = vec_geometric_inverse(a);
    f_result = vec_length_squared(a);
    VERIFY_SINGLE_VALUE(m_abs(result.x - m_div(a.x,f_result)), < , M_EPSILON)
    VERIFY_SINGLE_VALUE(m_abs(result.y - m_div(a.y,f_result)), < , M_EPSILON)
    VERIFY_SINGLE_VALUE(m_abs(result.z - m_div(a.z,f_result)), < , M_EPSILON)
    r_result = vec_geometric_product(a, result);
    VERIFY_SINGLE_VALUE(r_result.II, ==, fl2real(1.0))
    VERIFY_SINGLE_VALUE(r_result.T.x, ==, zero.x)
    VERIFY_SINGLE_VALUE(r_result.T.y, ==, zero.y)
    VERIFY_SINGLE_VALUE(r_result.T.z, ==, zero.z)
    COLLECT_FINDINGS

    #undef TITLE
    #define TITLE "rotors"
    POST_TITLE

    #undef SUBTITLE
    #define SUBTITLE "rotor_from_vectors"
    r_result = rotor_from_vectors(x,y);
    VERIFY_SINGLE_VALUE(m_abs(r_result.II  - m_div(fl2real(1.0),m_sqrt(fl2real(2.0)))), < , M_EPSILON)
    VERIFY_SINGLE_VALUE(r_result.T.x, ==, zero.x)
    VERIFY_SINGLE_VALUE(r_result.T.y, ==, zero.y)
    VERIFY_SINGLE_VALUE(m_abs(r_result.T.z + m_div(fl2real(1.0),m_sqrt(fl2real(2.0)))), < , M_EPSILON)
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "rotor_from_vectors - same vectors"
    r_result = rotor_from_vectors(x,x);
    VERIFY_SINGLE_VALUE(r_result.II, ==, fl2real(1.0))
    VERIFY_SINGLE_VALUE(r_result.T.x, ==, fl2real(0.0))
    VERIFY_SINGLE_VALUE(r_result.T.y, ==, fl2real(0.0))
    VERIFY_SINGLE_VALUE(r_result.T.z, ==, fl2real(0.0))
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "rotor_from_vectors - opposite vectors"
    r_result = rotor_from_vectors(x,vec_reverse(x));
    VERIFY_SINGLE_VALUE(r_result.II, ==, fl2real(1.0))
    VERIFY_SINGLE_VALUE(r_result.T.x, ==, fl2real(0.0))
    VERIFY_SINGLE_VALUE(r_result.T.y, ==, fl2real(0.0))
    VERIFY_SINGLE_VALUE(r_result.T.z, ==, fl2real(0.0))
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "rotor_from_scaled_axis_angle"
    r_result = rotor_from_scaled_axis_angle((vec3){0,0, m_div(M_PI,2)});
    VERIFY_SINGLE_VALUE(m_abs(r_result.II - m_cos(M_PI/4)), <, M_EPSILON)
    VERIFY_SINGLE_VALUE(r_result.T.x, ==, fl2real(0.0))
    VERIFY_SINGLE_VALUE(r_result.T.y, ==, fl2real(0.0))
    VERIFY_SINGLE_VALUE(m_abs(r_result.T.z - m_sin(M_PI/4)), <, M_EPSILON)
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "rotor_reverse"
    r_result = rotor_reverse((rotor3){fl2real(1.0), (bivec3)a});
    VERIFY_SINGLE_VALUE(r_result.II, ==, fl2real(1.0))
    VERIFY_SINGLE_VALUE(r_result.T.x, ==, neg.x)
    VERIFY_SINGLE_VALUE(r_result.T.y, ==, neg.y)
    VERIFY_SINGLE_VALUE(r_result.T.z, ==, neg.z)
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "rotor_combine - back and forth"
    r_result = rotor_combine(rotor_from_vectors(x,y), rotor_from_vectors(y,x));
    rotor3 to_compare = IDENTITY_ROTOR;
    VERIFY_SINGLE_VALUE(m_abs(r_result.II - to_compare.II), < , M_EPSILON)
    VERIFY_SINGLE_VALUE(m_abs(r_result.T.x - to_compare.T.x), < , M_EPSILON)
    VERIFY_SINGLE_VALUE(m_abs(r_result.T.y - to_compare.T.y), < , M_EPSILON)
    VERIFY_SINGLE_VALUE(m_abs(r_result.T.z - to_compare.T.z), < , M_EPSILON)
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "rotor_combine - x>y>z"
    r_result = rotor_combine(rotor_from_vectors(x,y), rotor_from_vectors(y,z));
    to_compare = rotor_combine(rotor_from_vectors(x,z), rotor_from_vectors(x,y));
    VERIFY_SINGLE_VALUE(m_abs(r_result.II - to_compare.II), < , M_EPSILON)
    VERIFY_SINGLE_VALUE(m_abs(r_result.T.x - to_compare.T.x), < , M_EPSILON)
    VERIFY_SINGLE_VALUE(m_abs(r_result.T.y - to_compare.T.y), < , M_EPSILON)
    VERIFY_SINGLE_VALUE(m_abs(r_result.T.z - to_compare.T.z), < , M_EPSILON)
    COLLECT_FINDINGS

    #undef TITLE
    #define TITLE "reflections and rotations"
    POST_TITLE

    #undef SUBTITLE
    #define SUBTITLE "vec_reflection"
    result = vec_reflection(x,y);
    VERIFY_SINGLE_VALUE(result.x, ==, neg.x)
    VERIFY_SINGLE_VALUE(result.y, ==, zero.y)
    VERIFY_SINGLE_VALUE(result.z, ==, zero.z)
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "vec_rotate_w_rotor - x by (x -> y)"
    result = vec_rotate_w_rotor(x, rotor_from_vectors(x,y));
    VERIFY_SINGLE_VALUE(m_abs(result.x - (-x.y)), < , M_EPSILON)
    VERIFY_SINGLE_VALUE(m_abs(result.y - (x.x)), < , M_EPSILON)
    VERIFY_SINGLE_VALUE(m_abs(result.z - (x.z)), < , M_EPSILON)
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "vec_rotate_w_rotor - a by (x -> y)"
    result = vec_rotate_w_rotor(a, rotor_from_vectors(x,y));
    VERIFY_SINGLE_VALUE(m_abs(result.x - (-a.y)), < , M_EPSILON)
    VERIFY_SINGLE_VALUE(m_abs(result.y - (a.x)), < , M_EPSILON)
    VERIFY_SINGLE_VALUE(m_abs(result.z - (a.z)), < , M_EPSILON)
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "vec_rotate - a by (x -> y)"
    result = vec_rotate(a, x,y);
    VERIFY_SINGLE_VALUE(m_abs(result.x - (-a.y)), < , M_EPSILON)
    VERIFY_SINGLE_VALUE(m_abs(result.y - (a.x)), < , M_EPSILON)
    VERIFY_SINGLE_VALUE(m_abs(result.z - (a.z)), < , M_EPSILON)
    COLLECT_FINDINGS

    #undef TITLE
    #define TITLE "interpolations"
    POST_TITLE

    #undef SUBTITLE
    #define SUBTITLE "vec_interpolation - x>y @ 0"
    result = vec_interpolation(x,y,fl2real(0.0));
    VERIFY_SINGLE_VALUE(m_abs(result.x - x.x), < , M_EPSILON)
    VERIFY_SINGLE_VALUE(m_abs(result.y - x.y), < , M_EPSILON)
    VERIFY_SINGLE_VALUE(m_abs(result.z - x.z), < , M_EPSILON)
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "vec_interpolation - x>y @ 1"
    result = vec_interpolation(x,y,fl2real(1.0));
    VERIFY_SINGLE_VALUE(m_abs(result.x - y.x), < , M_EPSILON)
    VERIFY_SINGLE_VALUE(m_abs(result.y - y.y), < , M_EPSILON)
    VERIFY_SINGLE_VALUE(m_abs(result.z - y.z), < , M_EPSILON)
    COLLECT_FINDINGS
    
    #undef SUBTITLE
    #define SUBTITLE "vec_interpolation - x>y @ 0.5"
    result = vec_interpolation(x,y,fl2real(0.5));
    vec3 mid_point = vec_normalize(vec_add(x,y));
    VERIFY_SINGLE_VALUE(m_abs(result.x - mid_point.x), < , M_EPSILON)
    VERIFY_SINGLE_VALUE(m_abs(result.y - mid_point.y), < , M_EPSILON)
    VERIFY_SINGLE_VALUE(m_abs(result.z - mid_point.z), < , M_EPSILON)
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "linear rotor interpolation"
    r_result = rotor_interpolation(IDENTITY_ROTOR, rotor_from_vectors(x,y), fl2real(0.5), LINEAR_INTERPOLATION);
    result = vec_rotate_w_rotor(x, r_result);
    VERIFY_SINGLE_VALUE(m_abs(result.x - mid_point.x), < , M_EPSILON)
    VERIFY_SINGLE_VALUE(m_abs(result.y - mid_point.y), < , M_EPSILON)
    VERIFY_SINGLE_VALUE(m_abs(result.z - mid_point.z), < , M_EPSILON)
    COLLECT_FINDINGS

    // #undef SUBTITLE
    // #define SUBTITLE "spherical rotor interpolation"
    // r_result = rotor_interpolation(IDENTITY_ROTOR, rotor_from_vectors(x,y), fl2real(0.5), SPHERICAL_INTERPOLATION);
    // result = vec_rotate_w_rotor(x, r_result);
    // VERIFY_SINGLE_VALUE(m_abs(result.x - mid_point.x), < , M_EPSILON)
    // VERIFY_SINGLE_VALUE(m_abs(result.y - mid_point.y), < , M_EPSILON)
    // VERIFY_SINGLE_VALUE(m_abs(result.z - mid_point.z), < , M_EPSILON)
    // printf("\nresult:%f,%f,%f\n", result.x, result.y, result.z);
    // printf("\nmid_point:%f,%f,%f\n", mid_point.x, mid_point.y, mid_point.z);
    // COLLECT_FINDINGS

    DEBRIEF
}