//Dependencies
#include <stdlib.h>

#include "../template_header.c"
#define bt_node            CONCATENATE(PREFIX, bt_node)                   //PREFIX_bt_node
#define create_bt_node     CONCATENATE(create_, bt_node)                  //create_PREFIX_bt_node
#define bt_insert_node     CONCATENATE(PREFIX, bt_insert_node)            //PREFIX_bt_insert_node
#define bt_insert_leaf     CONCATENATE(PREFIX, bt_insert_leaf)            //PREFIX_bt_insert_leaf
#define bt_node_count      CONCATENATE(PREFIX, bt_node_count)             //PREFIX_bt_node_count
#define bt_leaf_count      CONCATENATE(PREFIX, bt_leaf_count)             //PREFIX_bt_leaf_count
#define bt_is_leaf         CONCATENATE(PREFIX, bt_is_leaf)                //PREFIX_bt_is_leaf
#define bt_depth_traversal CONCATENATE(PREFIX, bt_depth_traversal)        //PREFIX_bt_depth_traversal
#define bt_width_traversal CONCATENATE(PREFIX, bt_width_traversal)        //PREFIX_bt_width_traversal
#define bt_cross_traversal CONCATENATE(PREFIX, bt_cross_traversal)        //PREFIX_bt_cross_traversal
#define bt_sibling_traversal CONCATENATE(PREFIX, bt_sibling_traversal)    //PREFIX_bt_sibling_traversal
#define free_bt_node       CONCATENATE(free_, bt_node)                    //free_PREFIX_bt_node

typedef struct bt_node
{
    TYPE data;

    struct bt_node* parent;
    struct bt_node* child[2];
}bt_node;

//TODO consider storing as array

// typedef struct
// {
//     bt_node* root;
    
//     // TODO possibly collect stats while traversing or inserting?
        // maybe create bt_stats instead and pass it to funcs, instead of rewriting funcs to accept binary_tree input?
//     // TODO include a free function?
// }binary_tree;

typedef void            (*node_func_t )(bt_node* node, ASSISTING_TYPE user_data);
typedef unsigned char   (*combo_test_t)(bt_node* a, bt_node* b, ASSISTING_TYPE user_data);
typedef void            (*combo_func_t)(bt_node* a, bt_node* b, ASSISTING_TYPE user_data);

bt_node* create_bt_node(TYPE data);

bt_node* bt_insert_node(bt_node* root, bt_node* to_insert, combo_test_t insertion_test, ASSISTING_TYPE user_data);
bt_node* bt_insert_leaf(bt_node* root, bt_node* to_insert, combo_test_t insertion_test, ASSISTING_TYPE user_data);

unsigned int bt_node_count(bt_node* root);
unsigned int bt_leaf_count(bt_node* root);
inline unsigned char bt_is_leaf(bt_node* node);

void bt_depth_traversal(bt_node* root, node_func_t non_leaf_func, node_func_t leaf_func, ASSISTING_TYPE user_data);
void bt_width_traversal(bt_node* root, node_func_t non_leaf_func, node_func_t leaf_func, ASSISTING_TYPE user_data);
void bt_cross_traversal  (bt_node* a, bt_node* b, combo_test_t combination_test, combo_func_t combination_func, ASSISTING_TYPE user_data);
void bt_sibling_traversal(bt_node* a,             combo_test_t combination_test, combo_func_t combination_func, ASSISTING_TYPE user_data);

void free_bt_node(bt_node* root);

//----------------------------------
#ifndef HEADERS_ONLY
inline bt_node* create_bt_node(TYPE data)
{
    bt_node* new = (bt_node*)calloc(1, sizeof(bt_node));
    new->data = data;
    return new;
}

#define default_insertion_test CONCATENATE(default_, CONCATENATE(PREFIX, insertion_test))
unsigned char default_insertion_test(bt_node* root, bt_node* to_insert, ASSISTING_TYPE user_data)
{
    //TODO turn this into a balanced tree algorithm
    return (root->child[0] != NULL) && (root->child[1] == NULL);
}

bt_node* bt_insert_node(bt_node* root, bt_node* to_insert, combo_test_t insertion_test, ASSISTING_TYPE user_data)
{
    if(to_insert == NULL) return root;
    if(root == NULL) return to_insert;

    if(insertion_test == NULL) insertion_test = default_insertion_test;

    unsigned char side = insertion_test(root, to_insert, user_data) > 0; //forces boolean return

    to_insert->parent = root;
    root->child[side] = bt_insert_node(root->child[side], to_insert, insertion_test, user_data);

    return root;
}

bt_node* bt_insert_leaf(bt_node* root, bt_node* to_insert, combo_test_t insertion_test, ASSISTING_TYPE user_data)
{
    if(to_insert == NULL) return root;
    if(root == NULL) return to_insert;

    if(root->child[0] == NULL && root->child[1] == NULL)
    {
        bt_node* split = create_bt_node((TYPE){0});
        
        split->parent = root->parent;
        split->child[0] = root;
        split->child[1] = to_insert;
        root->parent = split;
        to_insert->parent = split;

        return split;
    }

    if(insertion_test == NULL) insertion_test = default_insertion_test;

    unsigned char side = insertion_test(root, to_insert, user_data) > 0; //forces boolean return

    root->child[side] = bt_insert_leaf(root->child[side], to_insert, insertion_test, user_data);

    return root;
}
#undef default_insertion_test

unsigned int bt_node_count(bt_node* root)
{
    if(root == NULL) return 0;

    return bt_node_count(root->child[0]) + bt_node_count(root->child[1]) + 1;
}

unsigned int bt_leaf_count(bt_node* root)
{
    if(root == NULL) return 0;
    if(root->child[0] == NULL && root->child[1] == NULL) return 1;

    return bt_leaf_count(root->child[0]) + bt_leaf_count(root->child[1]);
}

inline unsigned char bt_is_leaf(bt_node* node)
{
    if(node == NULL) return 0;

    return node->child[0] == NULL && node->child[1] == NULL;
}

void bt_depth_traversal(bt_node* root, node_func_t non_leaf_func, node_func_t leaf_func, ASSISTING_TYPE user_data)
{
    if(root == NULL) return;

    if(root->child[0] || root->child[1])
    {
        if(non_leaf_func) non_leaf_func(root, user_data);
    }
    else
    {
        if(leaf_func) leaf_func(root, user_data);
        return;
    }

    bt_depth_traversal(root->child[0], non_leaf_func, leaf_func, user_data);
    bt_depth_traversal(root->child[1], non_leaf_func, leaf_func, user_data);
}

//same as breadth, but width has the same amount of letters as depth
void bt_width_traversal(bt_node* root, node_func_t non_leaf_func, node_func_t leaf_func, ASSISTING_TYPE user_data)
{
    if(root == NULL) return;

    //count nodes in tree and allocate corresponding cache memory    
    unsigned int node_count = bt_node_count(root);
    bt_node** queue = (bt_node**)calloc(node_count, sizeof(bt_node*));
    
    unsigned int max_index = 0;
    unsigned int current_index = 0;
    
    queue[max_index] = root;

    while(current_index < node_count)
    {
        unsigned char i = 0;
        for(;i < 2; i++)
            if(queue[current_index]->child[i])
            {
                max_index++;
                queue[max_index] = queue[current_index]->child[i];
            }

        if(queue[current_index]->child[0] || queue[current_index]->child[1])
        {
            if(non_leaf_func) non_leaf_func(queue[current_index], user_data);
        }
        else
        {
            if(leaf_func) leaf_func(queue[current_index], user_data);
        }

        current_index++;
    }

    free(queue);
}

//performs a function on every segregated combination of nodes that pass the test (doesnt apply to node combinations from the same tree)
void bt_cross_traversal(bt_node* a, bt_node* b, combo_test_t combination_test, combo_func_t combination_func, ASSISTING_TYPE user_data)
{
    if(a == NULL || b == NULL) return;

    if(combination_test && combination_test(a,b,user_data) == 0) return;

    if(combination_func) combination_func(a,b, user_data);

    //pick either the node itself, or its child(ren), if it has any
    unsigned char side,j,k = 0;
    bt_node* combo[2] = {a,b};
    bt_node* check[2][2] = {{a, NULL},{b, NULL}};
    for(side = 0; side < 2; side++)
    {
        for(j = 0; j < 2; j++)
        {
            if(combo[side]->child[j] != NULL)
            {
                check[side][k] = combo[side]->child[j];
                k++;
            }
        }
    }

    if(check[0][0] == a && check[1][0] == b) return;
    
    for(j = 0; j < 2; j++)
        for(k = 0; k < 2; k++)
            bt_cross_traversal(check[0][j], check[1][k], combination_test, combination_func, user_data);
}

//performs a function on every pair of siblings that pass the combination test
void bt_sibling_traversal(bt_node* a, combo_test_t combination_test, combo_func_t combination_func, ASSISTING_TYPE user_data)
{
    if(a == NULL) return;
    if(a->child[0] == NULL || a->child[1] == NULL) return;
    if(combination_test == NULL) return;

    if(combination_test(a->child[0], a->child[1], user_data))
    {
        if(combination_func) combination_func(a->child[0], a->child[1], user_data);

        bt_sibling_traversal(a->child[0], combination_test, combination_func, user_data);
        bt_sibling_traversal(a->child[1], combination_test, combination_func, user_data);
    }
}

void free_bt_node(bt_node* root)
{
    if(root == NULL) return;

    if(root->parent) root->parent->child[root->parent->child[1] == root] = NULL;

    free_bt_node(root->child[0]);
    free_bt_node(root->child[1]);
    
    //Dont do this if using bt_node in an inheritance style (at the front of a larger struct)
    // free(root);
}
#endif

#undef bt_node
#undef create_bt_node
#undef bt_insert_node
#undef bt_insert_leaf
#undef bt_node_count
#undef bt_leaf_count
#undef bt_is_leaf
#undef bt_depth_traversal
#undef bt_width_traversal
#undef bt_cross_traversal
#undef bt_sibling_traversal
#undef free_bt_node
#include "../template_footer.c"