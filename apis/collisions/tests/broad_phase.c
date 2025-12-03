#include "../../unit_testing.c"
#include <stdint.h>

#include "../broad_phase.c"
#define INCLUDE_IMPLEMENTATION
#include "../broad_phase.c"

void bvh_print(bvh_bt_node* node, uint8_t tab_level);
void bvh_print(bvh_bt_node* node, uint8_t tab_level)
{
    if(node == NULL) return;

    PRINT_FN("\n");
    uint8_t i = 0;
    for(; i < tab_level; i++)
        PRINT_FN("\t");

    aabb box = node->data.box;
    PRINT_FN("[%f,%f,%f] [%f,%f,%f]", box.center.x, box.center.y, box.center.z, box.half_size.x, box.half_size.y, box.half_size.z);

    bvh_print(node->child[0], tab_level + 1);
    bvh_print(node->child[1], tab_level + 1);
}

int main()
{
    bvh_data* data_result;
    bvh_bt_node *result, *temp;
    bvh_bt_node *a, *b, *c;

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
    a = create_bvh_bt_node((bvh_data){(aabb){(vec3){0,0,0}, (vec3){1,1,1}}, NULL});
    result = bvh_insert(NULL, a);
    VERIFY_SINGLE_VALUE(result, ==, a);
    VERIFY_SINGLE_VALUE(result->data.box.half_size.x, ==, a->data.box.half_size.x);
    VERIFY_SINGLE_VALUE(result->data.box.half_size.y, ==, a->data.box.half_size.y);
    VERIFY_SINGLE_VALUE(result->data.box.half_size.z, ==, a->data.box.half_size.z);
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "bvh_insert(NOT_NULL, NULL) - should return root"
    temp = result;
    result = bvh_insert(result, NULL);
    VERIFY_SINGLE_VALUE(result, ==, temp);
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "aabb update on insertion"
    b = create_bvh_bt_node((bvh_data){0});
    b->data.box = (aabb){(vec3){2,0,0}, (vec3){1,1,1}};
    result = bvh_insert(result, b);
    VERIFY_SINGLE_VALUE(result, !=, a);
    VERIFY_SINGLE_VALUE(result, !=, b);
    VERIFY_SINGLE_VALUE(result->child[0], ==, a);
    VERIFY_SINGLE_VALUE(result->child[1], ==, b);
    VERIFY_SINGLE_VALUE(result->data.box.center.x, ==, 1);
    VERIFY_SINGLE_VALUE(result->data.box.center.y, ==, 0);
    VERIFY_SINGLE_VALUE(result->data.box.center.z, ==, 0);
    VERIFY_SINGLE_VALUE(result->data.box.half_size.x, ==, 2);
    VERIFY_SINGLE_VALUE(result->data.box.half_size.y, ==, 1);
    VERIFY_SINGLE_VALUE(result->data.box.half_size.z, ==, 1);
    COLLECT_FINDINGS

    //
    c = create_bvh_bt_node((bvh_data){0});
    c->data.box = (aabb){(vec3){4,0,0}, (vec3){1,1,1}};
    result = bvh_insert(result, c);
    VERIFY_SINGLE_VALUE(result->data.box.half_size.x, ==, 3);
    VERIFY_SINGLE_VALUE(result->data.box.half_size.y, ==, 1);
    VERIFY_SINGLE_VALUE(result->data.box.half_size.z, ==, 1);
    COLLECT_FINDINGS

    // bvh_print(result, 0);

    //TODO collision_volume_group
    //TODO calculate_aabb_from_collision_volume

    DEBRIEF
}