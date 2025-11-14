#include "../../unit_testing.c"
#include <stdint.h>

#include "api.c"

uint8_t always_right(bt_node* a, bt_node* b, void* user_data) {return 1;}
uint8_t faulty_insertion_test(bt_node* a, bt_node* b, void* user_data) {return 2;}

#define NO_OF_NODES 30
uint8_t result_list[NO_OF_NODES];
uint8_t index = 0;

void clear_result_list()
{
    for(index = 0; index < NO_OF_NODES; index++)
        result_list[index] = -1;
}

void node_func(bt_node* node, void* user_data)
{
    result_list[index++] = (uint8_t)node->data;
}
void leaf_func(bt_node* node, void* user_data)
{
    node_func(node, user_data);
}

uint8_t combo_test(bt_node* a, bt_node* b, void* user_data)
{
    return 1;
}

void combo_func(bt_node* a, bt_node* b, void* user_data)
{
    result_list[index++] = (uint8_t)a->data + (uint8_t)b->data;
}

int main()
{
    bt_node* result;
    bt_node *a, *b, *c, *d;
    clear_result_list();

    #define TITLE "new node"
    POST_TITLE

    unsigned int test = 2;
    #define SUBTITLE "Best Case Scenario"
    bt_node* new = create_bt_node(&test);
    VERIFY_SINGLE_VALUE(new->parent, ==, NULL);
    VERIFY_SINGLE_VALUE(new->child[0], ==, NULL);
    VERIFY_SINGLE_VALUE(new->child[1], ==, NULL);
    VERIFY_SINGLE_VALUE(new->data, ==, &test);
    COLLECT_FINDINGS
    free(new);

    #undef SUBTITLE
    #define SUBTITLE "NULL data"
    new = create_bt_node(NULL);
    VERIFY_SINGLE_VALUE(new->parent, ==, NULL);
    VERIFY_SINGLE_VALUE(new->child[0], ==, NULL);
    VERIFY_SINGLE_VALUE(new->child[1], ==, NULL);
    VERIFY_SINGLE_VALUE(new->data, ==, NULL);
    COLLECT_FINDINGS
    free(new);

    #undef TITLE
    #define TITLE "node insertion"
    POST_TITLE

    #undef SUBTITLE
    #define SUBTITLE "bt_insert_node(NULL, NULL, NULL) - should return NULL"
    VERIFY_SINGLE_VALUE(bt_insert_node(NULL, NULL, NULL, NULL), ==, NULL);
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "bt_insert_node(NOT NULL, NULL, NULL) - should return root"
    new = create_bt_node(NULL);
    VERIFY_SINGLE_VALUE(bt_insert_node(new, NULL, NULL, NULL), ==, new);
    COLLECT_FINDINGS
    free(new);

    #undef SUBTITLE
    #define SUBTITLE "bt_insert_node(NULL, NOT NULL, NULL)"
    bt_node* leaf = create_bt_node((void*)1);
    new = bt_insert_node(NULL, leaf, NULL, NULL);
    VERIFY_SINGLE_VALUE(new, ==, leaf);
    VERIFY_SINGLE_VALUE(new->data, ==, leaf->data);
    VERIFY_SINGLE_VALUE(new->parent, ==, NULL);
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "bt_insert_node - fill children"
    a = create_bt_node((void*)2);
    b = create_bt_node((void*)3);
    c = create_bt_node((void*)4);
    bt_insert_node(new, a, NULL, NULL);
    bt_insert_node(new, b, NULL, NULL);
    bt_insert_node(new, c, NULL, NULL);
    VERIFY_SINGLE_VALUE(a->parent, ==, new);
    VERIFY_SINGLE_VALUE(b->parent, ==, new);
    VERIFY_SINGLE_VALUE(c->parent, ==, a);
    VERIFY_SINGLE_VALUE(new->child[0], ==, a);
    VERIFY_SINGLE_VALUE(new->child[1], ==, b);
    VERIFY_SINGLE_VALUE(a->child[0], ==, c);
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "bt_insert_node - custom insertion test"
    d = create_bt_node((void*)5);
    bt_insert_node(new, d, always_right, NULL);
    VERIFY_SINGLE_VALUE(d->parent, ==, b);
    VERIFY_SINGLE_VALUE(b->child[1], ==, d);
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "bt_insert_node - insertion test returning 2 or greater - should clamp to 1"
    c = create_bt_node((void*)6);
    bt_insert_node(new, c, faulty_insertion_test, NULL);
    VERIFY_SINGLE_VALUE(c->parent, ==, d);
    VERIFY_SINGLE_VALUE(d->child[1], ==, c);
    COLLECT_FINDINGS
    
    //TODO bt_insert_leaf

    #undef TITLE
    #define TITLE "utilities"
    POST_TITLE
    
    #undef SUBTITLE
    #define SUBTITLE "bt_node_count - Best Case Scenario"
    VERIFY_SINGLE_VALUE(bt_node_count(new), ==, 6)
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "bt_node_count - NULL root"
    VERIFY_SINGLE_VALUE(bt_node_count(NULL), ==, 0)
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "bt_leaf_count - Best Case Scenario"
    VERIFY_SINGLE_VALUE(bt_leaf_count(new), ==, 2)
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "bt_leaf_count - NULL root"
    VERIFY_SINGLE_VALUE(bt_leaf_count(NULL), ==, 0)
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "bt_is_leaf - NULL node"
    VERIFY_SINGLE_VALUE(bt_is_leaf(NULL), ==, 0)
    COLLECT_FINDINGS
    
    #undef SUBTITLE
    #define SUBTITLE "bt_is_leaf - root leaf"
    bt_node* root_leaf = create_bt_node((void*)1);
    VERIFY_SINGLE_VALUE(bt_is_leaf(root_leaf), ==, 1)
    COLLECT_FINDINGS
    
    #undef SUBTITLE
    #define SUBTITLE "bt_is_leaf - root child"
    bt_insert_node(root_leaf, create_bt_node((void*)1), always_right, NULL);
    VERIFY_SINGLE_VALUE(bt_is_leaf(root_leaf->child[1]), ==, 1)
    COLLECT_FINDINGS

    #undef TITLE
    #define TITLE "traversal"
    POST_TITLE

    uint8_t depth_list[] = {1,2,4,3,5,6};
    #undef SUBTITLE
    #define SUBTITLE "depth, NULL funcs - shouldnt crash"
    index = 0;
    bt_depth_traversal(new, NULL, NULL, NULL);
    VERIFY_SINGLE_VALUE(1, ==, 1);
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "depth"
    index = 0;
    bt_depth_traversal(new, node_func, leaf_func, NULL);
    VERIFY_ARRAY_OF_VALUES(result_list[test_iterator], ==, depth_list[test_iterator], sizeof(depth_list)/sizeof(uint8_t)-1);
    COLLECT_FINDINGS

    uint8_t width_list[] = {1,2,3,4,5,6};
    #undef SUBTITLE
    #define SUBTITLE "width, NULL funcs - shouldnt crash"
    index = 0;
    bt_width_traversal(new, NULL, NULL, NULL);
    VERIFY_SINGLE_VALUE(1, ==, 1);
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "width"
    clear_result_list();
    index = 0;
    bt_width_traversal(new, node_func, leaf_func, NULL);
    VERIFY_ARRAY_OF_VALUES(result_list[test_iterator], ==, width_list[test_iterator], sizeof(width_list)/sizeof(uint8_t)-1);
    COLLECT_FINDINGS

    a = create_bt_node((void*)7);
    bt_insert_node(new->child[0], a, NULL, NULL);
    uint8_t cross_list[] = {5,7,9,10,10,12,13};
    #undef SUBTITLE
    #define SUBTITLE "cross, NULL funcs - shouldnt crash"
    index = 0;
    bt_cross_traversal(new->child[0], new->child[1], NULL, NULL, NULL);
    VERIFY_SINGLE_VALUE(1, ==, 1);
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "cross"
    clear_result_list();
    index = 0;
    bt_cross_traversal(new->child[0], new->child[1], combo_test, combo_func, NULL);
    VERIFY_ARRAY_OF_VALUES(result_list[test_iterator], ==, cross_list[test_iterator], sizeof(cross_list)/sizeof(uint8_t)-1)
    COLLECT_FINDINGS
    
    #undef SUBTITLE
    #define SUBTITLE "cross - NULL combo test"
    clear_result_list();
    index = 0;
    bt_cross_traversal(new->child[0], new->child[1], NULL, combo_func, NULL);
    VERIFY_ARRAY_OF_VALUES(result_list[test_iterator], ==, cross_list[test_iterator], sizeof(cross_list)/sizeof(uint8_t)-1)
    COLLECT_FINDINGS

    uint8_t sibling_list[] = {5,11};
    #undef SUBTITLE
    #define SUBTITLE "sibling, NULL funcs - shouldnt crash"
    index = 0;
    bt_sibling_traversal(new, NULL, NULL, NULL);
    VERIFY_SINGLE_VALUE(1, ==, 1);
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "sibling"
    clear_result_list();
    index = 0;
    bt_sibling_traversal(new, combo_test, combo_func, NULL);
    VERIFY_ARRAY_OF_VALUES(result_list[test_iterator], ==, sibling_list[test_iterator], sizeof(sibling_list)/sizeof(uint8_t)-1)
    COLLECT_FINDINGS

    #undef TITLE
    #define TITLE "deletion"
    POST_TITLE

    #undef SUBTITLE
    #define SUBTITLE "leaf"
    a = c->parent;
    free_bt_node(c);
    VERIFY_SINGLE_VALUE(a->child[1], != , c);
    COLLECT_FINDINGS
    
    #undef SUBTITLE
    #define SUBTITLE "whole tree"
    free_bt_node(new);
    VERIFY_SINGLE_VALUE(1, == , 1);
    COLLECT_FINDINGS

    #undef TITLE
    #define TITLE "leaf insertion"
    POST_TITLE

    new = NULL;

    #undef SUBTITLE
    #define SUBTITLE "leaf(NULL, NULL, NULL) - should return NULL"
    VERIFY_SINGLE_VALUE(bt_insert_leaf(NULL, NULL, NULL, NULL), ==, NULL);
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "leaf(NOT_NULL, NULL, NULL) - should return root"
    new = create_bt_node(NULL);
    VERIFY_SINGLE_VALUE(bt_insert_leaf(new, NULL, NULL, NULL), ==, new);
    COLLECT_FINDINGS
    new = NULL;

    #undef SUBTITLE
    #define SUBTITLE "leaf(NULL, NOT NULL, NULL)"
    a = create_bt_node((void*)1);
    new = bt_insert_leaf(new, a, NULL, NULL);
    VERIFY_SINGLE_VALUE(new, ==, a);
    VERIFY_SINGLE_VALUE(new->data, ==, a->data);
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "leaf - fill children"
    b = create_bt_node((void*)2);
    new = bt_insert_leaf(new, b, NULL, NULL);
    c = create_bt_node((void*)3);
    new = bt_insert_leaf(new, c, NULL, NULL);

    clear_result_list();
    index = 0;
    uint8_t leaf_list[] = {2,3,5,2,4};
    bt_cross_traversal(new->child[0], new->child[1], combo_test, combo_func, NULL);
    bt_sibling_traversal(new, combo_test, combo_func, NULL);

    VERIFY_ARRAY_OF_VALUES(result_list[test_iterator], ==, leaf_list[test_iterator], sizeof(leaf_list)/sizeof(uint8_t)-1);
    COLLECT_FINDINGS

    DEBRIEF
}