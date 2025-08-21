#include "../../unit_testing.c"
#include <stdint.h>

#include "../aabb.c"
#define INCLUDE_IMPLEMENTATION
#include "../aabb.c"

int main()
{
    #undef TITLE
    #define TITLE "aabb_check"
    POST_TITLE

    aabb a,b, result;
    #undef SUBTITLE
    #define SUBTITLE "({{0,0,0},{1,1,1}} , {{1,1,1},{1,1,1}})"
    a.center = (vec3){0,0,0};
    a.half_size = (vec3){1,1,1};
    b.center = (vec3){1,1,1};
    b.half_size = a.half_size;
    VERIFY_SINGLE_VALUE(aabb_check(a, b), ==, 1);
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "({{0,0,0},{1,1,1}} , {{2,1,1},{1,1,1}}) - exactly touching"
    b.center = (vec3){2,1,1};
    VERIFY_SINGLE_VALUE(aabb_check(a, b), ==, 1);
    COLLECT_FINDINGS
    
    #undef SUBTITLE
    #define SUBTITLE "({{0,0,0},{1,1,1}} , {{3,1,1},{1,1,1}}) - not touching"
    b.center = (vec3){3,1,1};
    VERIFY_SINGLE_VALUE(aabb_check(a, b), ==, 0);
    COLLECT_FINDINGS
    
    #undef SUBTITLE
    #define SUBTITLE "({{0,0,0},{1,1,1}} , {{1,3,1},{1,1,1}}) - not touching"
    b.center = (vec3){1,3,1};
    VERIFY_SINGLE_VALUE(aabb_check(a, b), ==, 0);
    COLLECT_FINDINGS
    
    #undef SUBTITLE
    #define SUBTITLE "({{0,0,0},{1,1,1}} , {{1,1,3},{1,1,1}}) - not touching"
    b.center = (vec3){1,1,3};
    VERIFY_SINGLE_VALUE(aabb_check(a, b), ==, 0);
    COLLECT_FINDINGS

    #undef TITLE
    #define TITLE "aabb_for_pair"
    POST_TITLE
    
    b.center = (vec3){1,1,1};
    #undef SUBTITLE
    #define SUBTITLE "Best Case Scenario - size calculation only"
    result = aabb_for_pair(a,b, AABB_SIZE_CALCULATION);
    VERIFY_SINGLE_VALUE(m_abs(result.center.x - 0), <, M_EPSILON);
    VERIFY_SINGLE_VALUE(m_abs(result.center.y - 0), <, M_EPSILON);
    VERIFY_SINGLE_VALUE(m_abs(result.center.z - 0), <, M_EPSILON);
    VERIFY_SINGLE_VALUE(m_abs(result.half_size.x - fl2real(1.5)), <, M_EPSILON);
    VERIFY_SINGLE_VALUE(m_abs(result.half_size.y - fl2real(1.5)), <, M_EPSILON);
    VERIFY_SINGLE_VALUE(m_abs(result.half_size.z - fl2real(1.5)), <, M_EPSILON);
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "Best Case Scenario - full calculation"
    result = aabb_for_pair(a,b, AABB_FULL_CALCULATION);
    VERIFY_SINGLE_VALUE(m_abs(result.center.x - fl2real(0.5)), <, M_EPSILON);
    VERIFY_SINGLE_VALUE(m_abs(result.center.y - fl2real(0.5)), <, M_EPSILON);
    VERIFY_SINGLE_VALUE(m_abs(result.center.z - fl2real(0.5)), <, M_EPSILON);
    VERIFY_SINGLE_VALUE(m_abs(result.half_size.x - fl2real(1.5)), <, M_EPSILON);
    VERIFY_SINGLE_VALUE(m_abs(result.half_size.y - fl2real(1.5)), <, M_EPSILON);
    VERIFY_SINGLE_VALUE(m_abs(result.half_size.z - fl2real(1.5)), <, M_EPSILON);
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "zero half_size"
    a.half_size = (vec3){0,0,0};
    result = aabb_for_pair(a,b,0);
    VERIFY_SINGLE_VALUE(result.center.x, ==, b.center.x);
    VERIFY_SINGLE_VALUE(result.center.y, ==, b.center.y);
    VERIFY_SINGLE_VALUE(result.center.z, ==, b.center.z);
    VERIFY_SINGLE_VALUE(result.half_size.x, ==, b.half_size.x);
    VERIFY_SINGLE_VALUE(result.half_size.y, ==, b.half_size.y);
    VERIFY_SINGLE_VALUE(result.half_size.z, ==, b.half_size.z);
    COLLECT_FINDINGS
    a.half_size = (vec3){1,1,1};
    
    b.half_size = (vec3){0,0,0};
    result = aabb_for_pair(a,b,0);
    VERIFY_SINGLE_VALUE(result.center.x, ==, a.center.x);
    VERIFY_SINGLE_VALUE(result.center.y, ==, a.center.y);
    VERIFY_SINGLE_VALUE(result.center.z, ==, a.center.z);
    VERIFY_SINGLE_VALUE(result.half_size.x, ==, a.half_size.x);
    VERIFY_SINGLE_VALUE(result.half_size.y, ==, a.half_size.y);
    VERIFY_SINGLE_VALUE(result.half_size.z, ==, a.half_size.z);
    COLLECT_FINDINGS
    b.half_size = (vec3){1,1,1};

    DEBRIEF
}