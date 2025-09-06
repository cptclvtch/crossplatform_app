#include "../../unit_testing.c"
#include <stdint.h>

#include "../aabb.c"
#define INCLUDE_IMPLEMENTATION
#include "../aabb.c"

int main()
{
    #undef TITLE
    #define TITLE "aabb_aabb_check"
    POST_TITLE

    aabb a,b, a_result;
    vec3 p;
    #undef SUBTITLE
    #define SUBTITLE "({{0,0,0},{1,1,1}} , {{1,1,1},{1,1,1}}) - touching"
    a.center = (vec3){0,0,0};
    a.half_size = (vec3){1,1,1};
    b.center = (vec3){1,1,1};
    b.half_size = a.half_size;
    VERIFY_SINGLE_VALUE(aabb_aabb_check(a, b), ==, 1);
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "({{0,0,0},{1,1,1}} , {{2,1,1},{1,1,1}}) - exactly touching"
    b.center = (vec3){2,1,1};
    VERIFY_SINGLE_VALUE(aabb_aabb_check(a, b), ==, 1);
    COLLECT_FINDINGS
    
    #undef SUBTITLE
    #define SUBTITLE "({{0,0,0},{1,1,1}} , {{3,1,1},{1,1,1}}) - not touching"
    b.center = (vec3){3,1,1};
    VERIFY_SINGLE_VALUE(aabb_aabb_check(a, b), ==, 0);
    COLLECT_FINDINGS
    
    #undef SUBTITLE
    #define SUBTITLE "({{0,0,0},{1,1,1}} , {{1,3,1},{1,1,1}}) - not touching"
    b.center = (vec3){1,3,1};
    VERIFY_SINGLE_VALUE(aabb_aabb_check(a, b), ==, 0);
    COLLECT_FINDINGS
    
    #undef SUBTITLE
    #define SUBTITLE "({{0,0,0},{1,1,1}} , {{1,1,3},{1,1,1}}) - not touching"
    b.center = (vec3){1,1,3};
    VERIFY_SINGLE_VALUE(aabb_aabb_check(a, b), ==, 0);
    COLLECT_FINDINGS

    #undef TITLE
    #define TITLE "aabb_vec3_check"
    POST_TITLE

    #undef SUBTITLE
    #define SUBTITLE "({{0,0,0},{2,2,2}} , {1,1,1}) - inside"
    a.half_size = (vec3){2,2,2};
    p = (vec3){1,1,1};
    VERIFY_SINGLE_VALUE(aabb_vec3_check(a,p), ==, 1);
    COLLECT_FINDINGS
    
    #undef SUBTITLE
    #define SUBTITLE "({{0,0,0},{2,2,2}} , {0,0,2}) - inside, on the edge"
    p = (vec3){0,0,2};
    VERIFY_SINGLE_VALUE(aabb_vec3_check(a,p), ==, 1);
    COLLECT_FINDINGS

    #undef TITLE
    #define TITLE "aabb_union"
    POST_TITLE
    
    a.half_size = (vec3){1,1,1};
    b.center = (vec3){1,1,1};
    #undef SUBTITLE
    #define SUBTITLE "Passing"
    a_result = aabb_union(a,b);
    VERIFY_SINGLE_VALUE(m_abs(a_result.center.x - fl2real(0.5)), <, M_EPSILON);
    VERIFY_SINGLE_VALUE(m_abs(a_result.center.y - fl2real(0.5)), <, M_EPSILON);
    VERIFY_SINGLE_VALUE(m_abs(a_result.center.z - fl2real(0.5)), <, M_EPSILON);
    VERIFY_SINGLE_VALUE(m_abs(a_result.half_size.x - fl2real(1.5)), <, M_EPSILON);
    VERIFY_SINGLE_VALUE(m_abs(a_result.half_size.y - fl2real(1.5)), <, M_EPSILON);
    VERIFY_SINGLE_VALUE(m_abs(a_result.half_size.z - fl2real(1.5)), <, M_EPSILON);
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "Failing - A invalid"
    a.half_size = (vec3){0,0,0};
    a_result = aabb_union(a,b);
    VERIFY_SINGLE_VALUE(a_result.center.x, ==, b.center.x);
    VERIFY_SINGLE_VALUE(a_result.center.y, ==, b.center.y);
    VERIFY_SINGLE_VALUE(a_result.center.z, ==, b.center.z);
    VERIFY_SINGLE_VALUE(a_result.half_size.x, ==, b.half_size.x);
    VERIFY_SINGLE_VALUE(a_result.half_size.y, ==, b.half_size.y);
    VERIFY_SINGLE_VALUE(a_result.half_size.z, ==, b.half_size.z);
    COLLECT_FINDINGS
    a.half_size = (vec3){1,1,1};
    
    #undef SUBTITLE
    #define SUBTITLE "Failing - B invalid"
    b.half_size = (vec3){0,0,0};
    a_result = aabb_union(a,b);
    VERIFY_SINGLE_VALUE(a_result.center.x, ==, a.center.x);
    VERIFY_SINGLE_VALUE(a_result.center.y, ==, a.center.y);
    VERIFY_SINGLE_VALUE(a_result.center.z, ==, a.center.z);
    VERIFY_SINGLE_VALUE(a_result.half_size.x, ==, a.half_size.x);
    VERIFY_SINGLE_VALUE(a_result.half_size.y, ==, a.half_size.y);
    VERIFY_SINGLE_VALUE(a_result.half_size.z, ==, a.half_size.z);
    COLLECT_FINDINGS
    b.half_size = (vec3){1,1,1};

    #undef TITLE
    #define TITLE "aabb_intersection"
    POST_TITLE

    #undef SUBTITLE
    #define SUBTITLE "Passing"
    a.center = (vec3){0,0,0};
    a.half_size = (vec3){3,3,3};
    b.center = (vec3){4,4,4};
    b.half_size = (vec3){3,3,3};
    a_result = aabb_intersection(a,b);
    VERIFY_SINGLE_VALUE(m_abs(a_result.center.x - 2), <, M_EPSILON);
    VERIFY_SINGLE_VALUE(m_abs(a_result.center.y - 2), <, M_EPSILON);
    VERIFY_SINGLE_VALUE(m_abs(a_result.center.z - 2), <, M_EPSILON);
    VERIFY_SINGLE_VALUE(m_abs(a_result.half_size.x - fl2real(1)), <, M_EPSILON);
    VERIFY_SINGLE_VALUE(m_abs(a_result.half_size.y - fl2real(1)), <, M_EPSILON);
    VERIFY_SINGLE_VALUE(m_abs(a_result.half_size.z - fl2real(1)), <, M_EPSILON);
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "Failing"
    a.half_size = (vec3){1,1,1};
    b.half_size = (vec3){1,1,1};
    a_result = aabb_intersection(a,b);
    VERIFY_SINGLE_VALUE(m_abs(a_result.center.x - 2), <, M_EPSILON);
    VERIFY_SINGLE_VALUE(m_abs(a_result.center.y - 2), <, M_EPSILON);
    VERIFY_SINGLE_VALUE(m_abs(a_result.center.z - 2), <, M_EPSILON);
    VERIFY_SINGLE_VALUE(m_abs(a_result.half_size.x - fl2real(-1)), <, M_EPSILON);
    VERIFY_SINGLE_VALUE(m_abs(a_result.half_size.y - fl2real(-1)), <, M_EPSILON);
    VERIFY_SINGLE_VALUE(m_abs(a_result.half_size.z - fl2real(-1)), <, M_EPSILON);
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "Failing - A invalid"
    a.half_size = (vec3){0,0,0};
    a_result = aabb_intersection(a,b);
    VERIFY_SINGLE_VALUE(a_result.center.x, ==, a.center.x);
    VERIFY_SINGLE_VALUE(a_result.center.y, ==, a.center.y);
    VERIFY_SINGLE_VALUE(a_result.center.z, ==, a.center.z);
    VERIFY_SINGLE_VALUE(a_result.half_size.x, ==, a.half_size.x);
    VERIFY_SINGLE_VALUE(a_result.half_size.y, ==, a.half_size.y);
    VERIFY_SINGLE_VALUE(a_result.half_size.z, ==, a.half_size.z);
    COLLECT_FINDINGS
    a.half_size = (vec3){1,1,1};

    #undef SUBTITLE
    #define SUBTITLE "Failing - B invalid"
    b.half_size = (vec3){0,0,0};
    a_result = aabb_intersection(a,b);
    VERIFY_SINGLE_VALUE(a_result.center.x, ==, b.center.x);
    VERIFY_SINGLE_VALUE(a_result.center.y, ==, b.center.y);
    VERIFY_SINGLE_VALUE(a_result.center.z, ==, b.center.z);
    VERIFY_SINGLE_VALUE(a_result.half_size.x, ==, b.half_size.x);
    VERIFY_SINGLE_VALUE(a_result.half_size.y, ==, b.half_size.y);
    VERIFY_SINGLE_VALUE(a_result.half_size.z, ==, b.half_size.z);
    COLLECT_FINDINGS

    DEBRIEF
}