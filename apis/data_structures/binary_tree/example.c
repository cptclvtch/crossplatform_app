#include <stdio.h>

#include "api.c"

#define DESIRED_TYPE int
#define ASSISTING_TYPE short
#include "api.c"

unsigned char pick_side(bt_node* node, bt_node* to_insert, void* user_data)
{
    if(node->child[0] == NULL) return 0;
    return node->child[0]->data > to_insert->data;
}

void traversal_func(bt_node* a, void* user_data)
{
    printf("Node: %p\n", a);
}

int main()
{
    bt_node* root = create_bt_node(NULL);
    bt_node* leaf = create_bt_node(NULL);

    int_bt_node* other_root = create_int_bt_node(0);

    //------------------------------
    root = bt_insert_node(root, leaf, &pick_side, NULL);

    bt_width_traversal(root, traversal_func, traversal_func, NULL);

    //------------------------------

    free_bt_node(root);
    free_int_bt_node(other_root);

    printf("Done.");
}