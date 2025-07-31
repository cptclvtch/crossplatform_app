#include "../../unit_testing.c"

#include "../../geometric_algebra/api.c"
#include "../../data_structures/linked_list/api.c"
#include "../../data_structures/binary_tree/api.c"
#include "../aabb.c"
#include "../contacts.c"
#include "../bvh.c"
#define API_IMPLEMENTATION_ONLY
#include "../../geometric_algebra/api.c"
#include "../../data_structures/linked_list/api.c"
#include "../../data_structures/binary_tree/api.c"
#include "../aabb.c"
#include "../contacts.c"
#include "../bvh.c"

int main()
{
    #define TITLE "new node"
    POST_TITLE

    bvh_data *result, *temp;
    bvh_data *a, *b, *c;

    #undef SUBTITLE
    #define SUBTITLE "Best Case Scenario"
    result = bvh_get_new_node();
    VERIFY_SINGLE_VALUE(result, !=, NULL);
    COLLECT_FINDINGS

    #undef TITLE
    #define TITLE "leaf insertion and aabb update"
    POST_TITLE

    #undef SUBTITLE
    #define SUBTITLE "bvh_upward_aabb_update(NULL) - shouldnt crash"
    bvh_upward_aabb_update(NULL);
    VERIFY_SINGLE_VALUE(1,==,1);
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "bvh_insert(NULL, NULL) - should return NULL"
    VERIFY_SINGLE_VALUE(bvh_insert(NULL, NULL), ==, NULL);
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "bvh_insert(NULL, NOT NULL) - should return to_insert"
    a = bvh_get_new_node();
    a->box = (aabb){(vec3){0,0,0}, (vec3){1,1,1}};
    result = bvh_insert(NULL, a);
    VERIFY_SINGLE_VALUE(result, ==, a);
    VERIFY_SINGLE_VALUE(result->box.half_size.x, ==, a->box.half_size.x);
    VERIFY_SINGLE_VALUE(result->box.half_size.y, ==, a->box.half_size.y);
    VERIFY_SINGLE_VALUE(result->box.half_size.z, ==, a->box.half_size.z);
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "bvh_insert(NOT_NULL, NULL) - should return root"
    temp = result;
    result = bvh_insert(result, NULL);
    VERIFY_SINGLE_VALUE(result, ==, temp);
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "aabb update on insertion"
    b = bvh_get_new_node();
    b->box = (aabb){(vec3){2,0,0}, (vec3){1,1,1}};
    result = bvh_insert(result, b);
    VERIFY_SINGLE_VALUE(result, !=, a);
    VERIFY_SINGLE_VALUE(result, !=, b);
    VERIFY_SINGLE_VALUE(result->node.child[0], ==, (binary_tree*)a);
    VERIFY_SINGLE_VALUE(result->node.child[1], ==, (binary_tree*)b);
    VERIFY_SINGLE_VALUE(result->box.center.x, ==, 1);
    VERIFY_SINGLE_VALUE(result->box.center.y, ==, 0);
    VERIFY_SINGLE_VALUE(result->box.center.z, ==, 0);
    VERIFY_SINGLE_VALUE(result->box.half_size.x, ==, 2);
    VERIFY_SINGLE_VALUE(result->box.half_size.y, ==, 1);
    VERIFY_SINGLE_VALUE(result->box.half_size.z, ==, 1);
    COLLECT_FINDINGS
    
    //
    c = bvh_get_new_node();
    c->box = (aabb){(vec3){4,0,0}, (vec3){1,1,1}};
    result = bvh_insert(result, c);
    VERIFY_SINGLE_VALUE(result->box.half_size.x, ==, 3);
    VERIFY_SINGLE_VALUE(result->box.half_size.y, ==, 1);
    VERIFY_SINGLE_VALUE(result->box.half_size.z, ==, 1);
    COLLECT_FINDINGS

    #undef TITLE
    #define TITLE "deletion"
    POST_TITLE

    #undef SUBTITLE
    #define SUBTITLE "last node"
    bvh_delete_node(c);
    VERIFY_SINGLE_VALUE(result->box.half_size.x, ==, 2);
    VERIFY_SINGLE_VALUE(result->box.half_size.y, ==, 1);
    VERIFY_SINGLE_VALUE(result->box.half_size.z, ==, 1);
    COLLECT_FINDINGS

    DEBRIEF
}