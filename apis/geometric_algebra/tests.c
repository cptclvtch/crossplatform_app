#include "../unit_testing.c"
#include "api.c"
#define API_IMPLEMENTATION_ONLY
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

    real epsilon = fl2real(0.0001);

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
    VERIFY_SINGLE_VALUE(m_abs(f_result - m_sqrt(fl2real(3.0))), < , epsilon)
    // printf("\n%s < %s\n", fpt_cstr(m_abs(f_result - m_sqrt(fl2real(3.0))), 5), fpt_cstr(epsilon,5));
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "vec_is_normalized"
    VERIFY_SINGLE_VALUE(vec_is_normalized(a), ==, 0)
    VERIFY_SINGLE_VALUE(vec_is_normalized(vec_normalize(a)), ==, 1)
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
     < ,\
     0.000001)
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
    VERIFY_SINGLE_VALUE(m_abs(result.x - m_div(a.x,f_result)), < , epsilon)
    VERIFY_SINGLE_VALUE(m_abs(result.y - m_div(a.y,f_result)), < , epsilon)
    VERIFY_SINGLE_VALUE(m_abs(result.z - m_div(a.z,f_result)), < , epsilon)
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
    VERIFY_SINGLE_VALUE(m_abs(r_result.II  - m_div(fl2real(1.0),m_sqrt(fl2real(2.0)))), < , epsilon)
    VERIFY_SINGLE_VALUE(r_result.T.x, ==, zero.x)
    VERIFY_SINGLE_VALUE(r_result.T.y, ==, zero.y)
    VERIFY_SINGLE_VALUE(m_abs(r_result.T.z - m_div(fl2real(1.0),m_sqrt(fl2real(2.0)))), < , epsilon)
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
    #define SUBTITLE "rotor_reverse"
    r_result = rotor_reverse((rotor3){fl2real(1.0), (bivec3)a});
    VERIFY_SINGLE_VALUE(r_result.II, ==, fl2real(1.0))
    VERIFY_SINGLE_VALUE(r_result.T.x, ==, neg.x)
    VERIFY_SINGLE_VALUE(r_result.T.y, ==, neg.y)
    VERIFY_SINGLE_VALUE(r_result.T.z, ==, neg.z)
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "rotor_combine"
    r_result = rotor_combine(rotor_from_vectors(x,y), rotor_from_vectors(y,z));
    rotor3 to_compare = rotor_from_vectors(x,z);
    VERIFY_SINGLE_VALUE(m_abs(r_result.II - to_compare.II), < , epsilon)
    VERIFY_SINGLE_VALUE(m_abs(r_result.T.x - to_compare.T.x), < , epsilon)
    VERIFY_SINGLE_VALUE(m_abs(r_result.T.y - to_compare.T.y), < , epsilon)
    VERIFY_SINGLE_VALUE(m_abs(r_result.T.z - to_compare.T.z), < , epsilon)
    printf("\n%f:%f,%f,%f\n", r_result.II, r_result.T.x, r_result.T.y, r_result.T.z);
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
    VERIFY_SINGLE_VALUE(m_abs(result.x - (-x.y)), < , epsilon)
    VERIFY_SINGLE_VALUE(m_abs(result.y - (x.x)), < , epsilon)
    VERIFY_SINGLE_VALUE(m_abs(result.z - (x.z)), < , epsilon)
    printf("\nx:%f,%f,%f\n", x.x, x.y, x.z);
    printf("\nresult:%f,%f,%f\n", result.x, result.y, result.z);
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "vec_rotate_w_rotor - a by (x -> y)"
    result = vec_rotate_w_rotor(a, rotor_from_vectors(x,y));
    VERIFY_SINGLE_VALUE(m_abs(result.x - (-a.y)), < , epsilon)
    VERIFY_SINGLE_VALUE(m_abs(result.y - (a.x)), < , epsilon)
    VERIFY_SINGLE_VALUE(m_abs(result.z - (a.z)), < , epsilon)
    printf("\na:%f,%f,%f\n", a.x, a.y, a.z);
    printf("\nresult:%f,%f,%f\n", result.x, result.y, result.z);
    COLLECT_FINDINGS

    DEBRIEF
}