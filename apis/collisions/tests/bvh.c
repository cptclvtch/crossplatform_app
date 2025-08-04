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

void bvh_print(binary_tree* node, uint8_t tab_level);
void bvh_print(binary_tree* node, uint8_t tab_level)
{
    if(node == NULL) return;

    PRINT_FN("\n");
    uint8_t i = 0;
    for(; i < tab_level; i++)
        PRINT_FN("\t");

    aabb box = ((bvh_data*)node->data)->box;
    PRINT_FN("[%f,%f,%f] [%f,%f,%f]", box.center.x, box.center.y, box.center.z, box.half_size.x, box.half_size.y, box.half_size.z);

    bvh_print(node->child[0], tab_level + 1);
    bvh_print(node->child[1], tab_level + 1);
}

int main()
{
    #define TITLE "new node"
    POST_TITLE

    binary_tree *result, *temp;
    binary_tree *a, *b, *c;

    #undef SUBTITLE
    #define SUBTITLE "Best Case Scenario"
    result = bvh_new_data();
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
    a = binary_tree_new(bvh_new_data());
    ((bvh_data*)a->data)->box = (aabb){(vec3){0,0,0}, (vec3){1,1,1}};
    result = bvh_insert(NULL, a);
    VERIFY_SINGLE_VALUE(result, ==, a);
    VERIFY_SINGLE_VALUE(((bvh_data*)result->data)->box.half_size.x, ==, ((bvh_data*)a->data)->box.half_size.x);
    VERIFY_SINGLE_VALUE(((bvh_data*)result->data)->box.half_size.y, ==, ((bvh_data*)a->data)->box.half_size.y);
    VERIFY_SINGLE_VALUE(((bvh_data*)result->data)->box.half_size.z, ==, ((bvh_data*)a->data)->box.half_size.z);
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "bvh_insert(NOT_NULL, NULL) - should return root"
    temp = result;
    result = bvh_insert(result, NULL);
    VERIFY_SINGLE_VALUE(result, ==, temp);
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "aabb update on insertion"
    b = binary_tree_new(bvh_new_data());
    ((bvh_data*)b->data)->box = (aabb){(vec3){2,0,0}, (vec3){1,1,1}};
    result = bvh_insert(result, b);
    VERIFY_SINGLE_VALUE(result, !=, a);
    VERIFY_SINGLE_VALUE(result, !=, b);
    VERIFY_SINGLE_VALUE(result->child[0], ==, a);
    VERIFY_SINGLE_VALUE(result->child[1], ==, b);
    VERIFY_SINGLE_VALUE(((bvh_data*)result->data)->box.center.x, ==, 1);
    VERIFY_SINGLE_VALUE(((bvh_data*)result->data)->box.center.y, ==, 0);
    VERIFY_SINGLE_VALUE(((bvh_data*)result->data)->box.center.z, ==, 0);
    VERIFY_SINGLE_VALUE(((bvh_data*)result->data)->box.half_size.x, ==, 2);
    VERIFY_SINGLE_VALUE(((bvh_data*)result->data)->box.half_size.y, ==, 1);
    VERIFY_SINGLE_VALUE(((bvh_data*)result->data)->box.half_size.z, ==, 1);
    COLLECT_FINDINGS

    //
    c = binary_tree_new(bvh_new_data());
    ((bvh_data*)c->data)->box = (aabb){(vec3){4,0,0}, (vec3){1,1,1}};
    result = bvh_insert(result, c);
    VERIFY_SINGLE_VALUE(((bvh_data*)result->data)->box.half_size.x, ==, 3);
    VERIFY_SINGLE_VALUE(((bvh_data*)result->data)->box.half_size.y, ==, 1);
    VERIFY_SINGLE_VALUE(((bvh_data*)result->data)->box.half_size.z, ==, 1);
    COLLECT_FINDINGS

    bvh_print(result, 0);

    DEBRIEF
}