#include "../../unit_testing.c"

#include "../../data_structures/linked_list/api.c"
#include "../../geometric_algebra/api.c"
#include "../api.c"
#define API_IMPLEMENTATION_ONLY
#include "../../data_structures/linked_list/api.c"
#include "../../geometric_algebra/api.c"
#include "../api.c"

//Tests
#include "aabb.c"
#include "contacts.c"
#include "bvh.c"

void bvh_print(bvh_node* root, uint8_t tab_level);
void bvh_print(bvh_node* root, uint8_t tab_level)
{
    if(root == NULL) return;

    uint8_t i = 0;
    for(; i < tab_level; i++)
        PRINT_FN("\t");

    //contents
    PRINT_FN("Node\n");
    
    bvh_print(root->children[0], tab_level++);
    bvh_print(root->children[1], tab_level++);
}

int main()
{
    CLEAR_SCREEN

    #define TITLE "BVH - node type"
    POST_TITLE

    bvh_node* node_a = bvh_get_empty_node();
    bvh_node* node_b = bvh_get_empty_node();
    #define SUBTITLE "NULL input"
    VERIFY_SINGLE_VALUE(bvh_node_type(NULL), ==, BVH_NULL);
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "no children, no data present (BVH_EMPTY)"
    VERIFY_SINGLE_VALUE(bvh_node_type(node_a), ==, BVH_EMPTY);
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "no children, data present (BVH_LEAF)"
    node_a->data = (void*)1;
    VERIFY_SINGLE_VALUE(bvh_node_type(node_a), ==, BVH_LEAF);
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "1 child, data present (BVH_BRANCH)"
    node_a->children[0] = (void*)1;
    VERIFY_SINGLE_VALUE(bvh_node_type(node_a), ==, BVH_BRANCH);
    COLLECT_FINDINGS
    
    #undef SUBTITLE
    #define SUBTITLE "1 child, no data present (BVH_BRANCH)"
    node_a->data = NULL;
    VERIFY_SINGLE_VALUE(bvh_node_type(node_a), ==, BVH_BRANCH);
    COLLECT_FINDINGS
    
    #undef SUBTITLE
    #define SUBTITLE "2 children, no data present (BVH_BRANCH)"
    node_a->children[1] = (void*)1;
    VERIFY_SINGLE_VALUE(bvh_node_type(node_a), ==, BVH_BRANCH);
    COLLECT_FINDINGS
    
    #undef SUBTITLE
    #define SUBTITLE "2 children, data present (BVH_BRANCH)"
    node_a->data = (void*)1;
    VERIFY_SINGLE_VALUE(bvh_node_type(node_a), ==, BVH_BRANCH);
    COLLECT_FINDINGS

    #undef TITLE
    #define TITLE "BVH - AABB check"
    POST_TITLE
    
    #undef SUBTITLE
    #define SUBTITLE "any NULL input"
    VERIFY_SINGLE_VALUE(bvh_aabb_check(NULL, NULL), ==, 0);
    VERIFY_SINGLE_VALUE(bvh_aabb_check(node_a, NULL), ==, 0);
    VERIFY_SINGLE_VALUE(bvh_aabb_check(NULL, node_a), ==, 0);
    COLLECT_FINDINGS
    
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

    #undef TITLE
    #define TITLE "BVH - insertion"
    POST_TITLE

    #undef SUBTITLE
    #define SUBTITLE "(NULL, NULL) - shouldnt crash"
    bvh_insert(NULL, NULL);
    VERIFY_SINGLE_VALUE(1, ==, 1);
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "(NULL, NOT NULL)"
    free(node_a); free(node_b);
    node_b = bvh_get_empty_node();
    node_a = bvh_insert(NULL, node_b);
    VERIFY_SINGLE_VALUE(node_a->center.x, ==, node_b->center.x);
    VERIFY_SINGLE_VALUE(node_a->center.y, ==, node_b->center.y);
    VERIFY_SINGLE_VALUE(node_a->center.z, ==, node_b->center.z);
    VERIFY_SINGLE_VALUE(node_a->half_size.x, ==, node_b->half_size.x);
    VERIFY_SINGLE_VALUE(node_a->half_size.y, ==, node_b->half_size.y);
    VERIFY_SINGLE_VALUE(node_a->half_size.z, ==, node_b->half_size.z);
    VERIFY_SINGLE_VALUE(node_a->parent, ==, node_b->parent);
    VERIFY_SINGLE_VALUE(node_a->children[0], ==, node_b->children[0]);
    VERIFY_SINGLE_VALUE(node_a->children[1], ==, node_b->children[1]);
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "one child"
    node_a = bvh_insert(node_a, bvh_get_empty_node());
    node_a = bvh_insert(node_a, bvh_get_empty_node());
    node_a = bvh_insert(node_a, bvh_get_empty_node());
    node_a = bvh_insert(node_a, bvh_get_empty_node());

    COLLECT_FINDINGS
    
    CLEAR_SCREEN
    bvh_print(node_a, 0);
    
    #undef TITLE
    #define TITLE "BVH - volume calculation"
    POST_TITLE

    #undef SUBTITLE
    #define SUBTITLE "(NULL, NULL) - shouldnt crash"
    bvh_aabb_for_pair(NULL, NULL, AABB_SIZE_CALCULATION);
    VERIFY_SINGLE_VALUE(1, ==, 1);
    COLLECT_FINDINGS

    //creation
    //  - insertion
    //  - volume update

    //traversal
    //  - NULL children and data
    //  - only one child
    //  - normal traversal

    //deletion

    DEBRIEF
}