#include "../../unit_testing.c"

#include "../aabb.c"
#define API_IMPLEMENTATION_ONLY
#include "../aabb.c"

int main()
{
    #undef TITLE
    #define TITLE "AABB check"
    POST_TITLE
    
    #undef SUBTITLE
    #define SUBTITLE "any NULL input"
    //aabb_for_pair should return the other aabb if one of them has 0 volume

    #undef SUBTITLE
    #define SUBTITLE "({{0,0,0},{1,1,1}} , {{1,1,1},{1,1,1}})"
    node_a->center = (vec3){0,0,0};
    node_a->half_size = (vec3){1,1,1};
    node_b->center = (vec3){1,1,1};
    node_b->half_size = node_a->half_size;
    VERIFY_SINGLE_VALUE(bvh_aabb_check(node_a, node_b), ==, 1);
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "({{0,0,0},{1,1,1}} , {{2,1,1},{1,1,1}}) - exactly touching"
    node_b->center = (vec3){2,1,1};
    VERIFY_SINGLE_VALUE(bvh_aabb_check(node_a, node_b), ==, 1);
    COLLECT_FINDINGS
    
    #undef SUBTITLE
    #define SUBTITLE "({{0,0,0},{1,1,1}} , {{3,1,1},{1,1,1}}) - not touching"
    node_b->center = (vec3){3,1,1};
    VERIFY_SINGLE_VALUE(bvh_aabb_check(node_a, node_b), ==, 0);
    COLLECT_FINDINGS
    
    #undef SUBTITLE
    #define SUBTITLE "({{0,0,0},{1,1,1}} , {{1,3,1},{1,1,1}}) - not touching"
    node_b->center = (vec3){1,3,1};
    VERIFY_SINGLE_VALUE(bvh_aabb_check(node_a, node_b), ==, 0);
    COLLECT_FINDINGS
    
    #undef SUBTITLE
    #define SUBTITLE "({{0,0,0},{1,1,1}} , {{1,1,3},{1,1,1}}) - not touching"
    node_b->center = (vec3){1,1,3};
    VERIFY_SINGLE_VALUE(bvh_aabb_check(node_a, node_b), ==, 0);
    COLLECT_FINDINGS

    DEBRIEF
}