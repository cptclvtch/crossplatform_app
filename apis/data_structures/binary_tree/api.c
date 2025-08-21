//Dependencies
#include "../linked_list/api.c"
#include <stdlib.h>

#ifndef _BINARY_TREE_H
    #define _BINARY_TREE_H

// TODO possibly collect stats while traversing or inserting?
// TODO maybe store and use a delete function?
typedef struct binary_tree
{
    struct binary_tree* parent;
    struct binary_tree* child[2];

    void* data;
}binary_tree;

binary_tree* binary_tree_new(void* data);

binary_tree* binary_tree_insert_node(binary_tree* root, binary_tree* to_insert, unsigned char (*insertion_test)(binary_tree* root, binary_tree* to_insert));
binary_tree* binary_tree_insert_leaf(binary_tree* root, binary_tree* to_insert, unsigned char (*insertion_test)(binary_tree* root, binary_tree* to_insert));

void binary_tree_depth_traversal(binary_tree* root, void (*non_leaf_func)(binary_tree* node), void (*leaf_func)(binary_tree* node));
void binary_tree_width_traversal(binary_tree* root, void (*non_leaf_func)(binary_tree* node), void (*leaf_func)(binary_tree* node));
void binary_tree_cross_traversal(binary_tree* a, binary_tree* b, unsigned char (*combination_test)(binary_tree* a, binary_tree* b), void (*combination_func)(binary_tree* a, binary_tree* b));
void binary_tree_sibling_traversal(binary_tree* a, unsigned char (*combination_test)(binary_tree* a, binary_tree* b), void (*combination_func)(binary_tree* a, binary_tree* b));

void binary_tree_delete(binary_tree* root);
#endif //_BINARY_TREE_H

//----------------------------------

#if defined(INCLUDE_IMPLEMENTATION) && !defined(_BINARY_TREE_C)
    #define _BINARY_TREE_C

inline binary_tree* binary_tree_new(void* data)
{
    binary_tree* new = (binary_tree*)calloc(1, sizeof(binary_tree));
    new->data = data;
    return new;
}

unsigned char default_insertion_test(binary_tree* root, binary_tree* to_insert)
{
    //TODO turn this into a balanced tree algorithm
    return (root->child[0] != NULL) && (root->child[1] == NULL);
}

binary_tree* binary_tree_insert_node(binary_tree* root, binary_tree* to_insert, unsigned char (*insertion_test)(binary_tree* root, binary_tree* to_insert))
{
    if(to_insert == NULL) return root;
    if(root == NULL) return to_insert;

    if(insertion_test == NULL) insertion_test = default_insertion_test;

    unsigned char side = insertion_test(root, to_insert) > 0; //forces boolean return

    to_insert->parent = root;
    root->child[side] = binary_tree_insert_node(root->child[side], to_insert, insertion_test);

    return root;
}

binary_tree* binary_tree_insert_leaf(binary_tree* root, binary_tree* to_insert, unsigned char (*insertion_test)(binary_tree* root, binary_tree* to_insert))
{
    if(to_insert == NULL) return root;
    if(root == NULL) return to_insert;

    if(root->child[0] == NULL && root->child[1] == NULL)
    {
        binary_tree* split = binary_tree_new(NULL);
        
        split->parent = root->parent;
        split->child[0] = root;
        split->child[1] = to_insert;
        root->parent = split;
        to_insert->parent = split;

        return split;
    }

    if(insertion_test == NULL) insertion_test = default_insertion_test;

    unsigned char side = insertion_test(root, to_insert) > 0; //forces boolean return

    root->child[side] = binary_tree_insert_leaf(root->child[side], to_insert, insertion_test);

    return root;
}

void binary_tree_depth_traversal(binary_tree* root, void (*non_leaf_func)(binary_tree* node), void (*leaf_func)(binary_tree* node))
{
    if(root == NULL) return;

    if(root->child[0] || root->child[1])
    {
        if(non_leaf_func) non_leaf_func(root);
    }
    else
    {
        if(leaf_func) leaf_func(root);
        return;
    }

    binary_tree_depth_traversal(root->child[0], non_leaf_func, leaf_func);
    binary_tree_depth_traversal(root->child[1], non_leaf_func, leaf_func);
}

//same as breadth, but width has the same amount of letters as depth
void binary_tree_width_traversal(binary_tree* root, void (*non_leaf_func)(binary_tree* node), void (*leaf_func)(binary_tree* node))
{
    if(root == NULL) return;

    linked_list_node* current = add_link_after(NULL, root);
    linked_list_node* queue = current;

    while(current)
    {
        binary_tree* current_node = (binary_tree*)current->data;

        unsigned char i = 0;
        for(;i < 2; i++)
            if(current_node->child[i]) queue = add_link_after(queue, current_node->child[i]);

        if(current_node->child[0] || current_node->child[1])
        {
            if(non_leaf_func) non_leaf_func(current_node);
        }
        else
        {
            if(leaf_func) leaf_func(current_node);
        }

        delete_link(current->PREV, DONT_DELETE_CONTENTS);
        current = current->NEXT;
    }
    delete_link(queue, DONT_DELETE_CONTENTS);
}

//performs a function on every segregated combination of nodes that pass the test (doesnt apply to node combinations from the same tree)
void binary_tree_cross_traversal(binary_tree* a, binary_tree* b, unsigned char (*combination_test)(binary_tree* a, binary_tree* b), void (*combination_func)(binary_tree* a, binary_tree* b))
{
    //TODO try to make more elegant
    //TODO consider rewriting this in a way that allows leaf and non_leaf functions
    if(a == NULL || b == NULL) return;
    if(combination_test == NULL) return;

    if(combination_test(a,b))
    {
        if(combination_func) combination_func(a,b);

        //pick either the node itself, or its child(ren), if it has any
        unsigned char side,j,k = 0;
        binary_tree* combo[2] = {a,b};
        binary_tree* check[2][2] = {{a, NULL},{b, NULL}};
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
                binary_tree_cross_traversal(check[0][j], check[1][k], combination_test, combination_func);
    }
}

//performs a function on every pair of siblings that pass the combination test
void binary_tree_sibling_traversal(binary_tree* a, unsigned char (*combination_test)(binary_tree* a, binary_tree* b), void (*combination_func)(binary_tree* a, binary_tree* b))
{
    if(a == NULL) return;
    if(a->child[0] == NULL || a->child[1] == NULL) return;
    if(combination_test == NULL) return;

    if(combination_test(a->child[0], a->child[1]))
    {
        if(combination_func) combination_func(a->child[0], a->child[1]);

        binary_tree_sibling_traversal(a->child[0], combination_test, combination_func);
        binary_tree_sibling_traversal(a->child[1], combination_test, combination_func);
    }
}

void binary_tree_delete(binary_tree* root)
{
    if(root == NULL) return;

    if(root->parent)
        root->parent->child[root->parent->child[1] == root] = NULL;

    binary_tree_delete(root->child[0]);
    binary_tree_delete(root->child[1]);
    
    //Dont do this if using binary_tree in an inheritance style (at the front of a larger struct)
    // free(root);
}
#endif