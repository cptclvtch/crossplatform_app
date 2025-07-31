#include "../linked_list/api.c"
#include "api.c"
#define API_IMPLEMENTATION_ONLY
#include "../linked_list/api.c"
#include "api.c"

#include <stdio.h>

unsigned char pick_side(binary_tree* node, binary_tree* to_insert)
{
    if(node->child[0] == NULL) return 0;
    return node->child[0]->data > to_insert->data;
}

void traversal_func(binary_tree* a)
{
    printf("Node: %p\n", a);
}

int main()
{
    binary_tree* root = binary_tree_new();
    binary_tree* leaf = binary_tree_new();

    //------------------------------
    root = binary_tree_insert_node(root, leaf, &pick_side);

    binary_tree_width_traversal(root, &traversal_func, &traversal_func);

    binary_tree_delete(root);

    //------------------------------
    printf("Done.");
}