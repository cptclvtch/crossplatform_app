#include "../../unit_testing.c"

#define INCLUDE_IMPLEMENTATION
#include "../narrow_phase.c"

int main()
{
    vec3 v_result;
    vec3 v_a, v_b;
    rotor3 r_a = IDENTITY_ROTOR;
    rotor3 r_b = r_a;
    collision_volume* cv_a = (collision_volume*)calloc(1,sizeof(collision_volume));
    collision_volume* cv_b = (collision_volume*)calloc(1,sizeof(collision_volume));
    collision_pair p = (collision_pair){0};
    #define TITLE "helpers"
    POST_TITLE

    #undef SUBTITLE
    #define SUBTITLE "minmax_separating_axis_test - Positive overlap"
    v_result = minmax_separating_axis_test(-1,1,0,2);
    VERIFY_SINGLE_VALUE(v_result.x - fl2real(1), < , M_EPSILON);
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "minmax_separating_axis_test - Out of order"
    v_result = minmax_separating_axis_test(0,2,-1,1);
    VERIFY_SINGLE_VALUE(v_result.x - fl2real(1), < , M_EPSILON);
    COLLECT_FINDINGS
    
    #undef SUBTITLE
    #define SUBTITLE "minmax_separating_axis_test - Negative overlapping"
    v_result = minmax_separating_axis_test(-1,1,2,4);
    VERIFY_SINGLE_VALUE(v_result.x - fl2real(-1), < , M_EPSILON);
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "minmax_separating_axis_test - Full overlap"
    v_result = minmax_separating_axis_test(0,3,1,2);
    VERIFY_SINGLE_VALUE(v_result.x - fl2real(2), < , M_EPSILON);
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "halfsize_separating_axis_test - Positive overlap"
    v_result = halfsize_separating_axis_test(0,1,1,1);
    VERIFY_SINGLE_VALUE(v_result.x - fl2real(1), < , M_EPSILON);
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "halfsize_separating_axis_test - Out of order"
    v_result = halfsize_separating_axis_test(1,1,0,1);
    VERIFY_SINGLE_VALUE(v_result.x - fl2real(1), < , M_EPSILON);
    COLLECT_FINDINGS
    
    #undef SUBTITLE
    #define SUBTITLE "halfsize_separating_axis_test - Negative overlapping"
    v_result = halfsize_separating_axis_test(0,1,3,1);
    VERIFY_SINGLE_VALUE(v_result.x - fl2real(-1), < , M_EPSILON);
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "halfsize_separating_axis_test - Full overlap"
    v_result = halfsize_separating_axis_test(2,1,2,2);
    VERIFY_SINGLE_VALUE(v_result.x - fl2real(2), < , M_EPSILON);
    COLLECT_FINDINGS

    //pick_test_axes
    //- best case scenario
    //- same argument > should return face normals

    #undef TITLE
    #define TITLE "sphere to sphere"
    POST_TITLE
    
    p.members[0] = cv_a;
    p.members[1] = cv_b;
    cv_a->type = SPHERE;
    cv_a->radius = fl2real(1);
    cv_a->position = &v_a;
    cv_a->orientation = &r_a;
    cv_b->type = SPHERE;
    cv_b->radius = fl2real(1);
    cv_b->position = &v_b;
    cv_b->orientation = &r_b;
    
    #undef SUBTITLE
    #define SUBTITLE "Passing"
    v_a = (vec3){0,0,0};
    v_b = (vec3){1.5,0,0};
    collision_detect(&p);
    VERIFY_SINGLE_VALUE(p.type, ==, CONFIRMED_COLLISION);
    vec3 n = p.points[p.contact_count-1].normal;
    VERIFY_SINGLE_VALUE(vec_are_parallel(p.points[p.contact_count-1].normal, (vec3){1,0,0}, M_EPSILON), ==, 1);
    VERIFY_SINGLE_VALUE(p.points[p.contact_count-1].penetration - fl2real(1), <, M_EPSILON);
    COLLECT_FINDINGS
    p.contact_count = 0;

    #undef SUBTITLE
    #define SUBTITLE "Passing"
    v_b = (vec3){1,1,1};
    sphere_to_sphere(&p);
    VERIFY_SINGLE_VALUE(p.type, ==, CONFIRMED_COLLISION);
    VERIFY_SINGLE_VALUE(vec_are_parallel(p.points[p.contact_count-1].normal, (vec3){1,1,1}, M_EPSILON), ==, 1);
    COLLECT_FINDINGS
    p.contact_count = 0;

    #undef SUBTITLE
    #define SUBTITLE "Passing - exactly"
    v_b = (vec3){2,0,0};
    sphere_to_sphere(&p);
    VERIFY_SINGLE_VALUE(p.type, ==, CONFIRMED_COLLISION);
    COLLECT_FINDINGS
    p.contact_count = 0;
    
    #undef SUBTITLE
    #define SUBTITLE "Failing"
    v_b = (vec3){3,0,0};
    sphere_to_sphere(&p);
    VERIFY_SINGLE_VALUE(p.type, ==, NO_COLLISION);
    COLLECT_FINDINGS
    p.contact_count = 0;

    #undef TITLE
    #define TITLE "sphere_to_box"
    POST_TITLE

    cv_b->type = BOX;
    cv_b->half_size = (vec3){1,1,1};
    #undef SUBTITLE
    #define SUBTITLE "Passing"
    v_b = (vec3){1,0,0};
    sphere_to_box(&p);
    VERIFY_SINGLE_VALUE(p.type, ==, CONFIRMED_COLLISION);
    COLLECT_FINDINGS
    p.contact_count = 0;

    #undef SUBTITLE
    #define SUBTITLE "Passing - exactly"
    v_b = (vec3){2,0,0};
    sphere_to_box(&p);
    VERIFY_SINGLE_VALUE(p.type, ==, CONFIRMED_COLLISION);
    COLLECT_FINDINGS
    p.contact_count = 0;
    
    #undef SUBTITLE
    #define SUBTITLE "Failing"
    v_b = (vec3){3,0,0};
    sphere_to_box(&p);
    VERIFY_SINGLE_VALUE(p.type, ==, NO_COLLISION);
    COLLECT_FINDINGS
    p.contact_count = 0;

    //narrow_func array testing

    DEBRIEF
}