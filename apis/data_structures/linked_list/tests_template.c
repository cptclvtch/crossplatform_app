#include "../../unit_testing.c"

#define DESIRED_TYPE int
#include "api.c"

typedef struct
{
    unsigned int pos[3];
}vec;

#define DESIRED_TYPE vec
#include "api.c"

int main()
{
    int_ll_node* inode;
    vec_ll_node* vnode;
    int a = 2;
    vec v = (vec){.pos = {1,2,3}};

    #define TITLE "basic type"
    POST_TITLE

    #define SUBTITLE "creation and data retrieval"
    inode = create_int_ll_node(a);
    VERIFY_SINGLE_VALUE(inode->data, ==, 2)
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "deletion"
    free_int_ll_node(inode, NULL);
    VERIFY_SINGLE_VALUE(1, ==, 1)
    COLLECT_FINDINGS

    #undef TITLE
    #define TITLE "struct type"
    POST_TITLE

    #undef SUBTITLE
    #define SUBTITLE "creation and data retrieval"
    vnode = create_vec_ll_node(v);
    VERIFY_SINGLE_VALUE(vnode->data.pos[0], ==, 1)
    VERIFY_SINGLE_VALUE(vnode->data.pos[1], ==, 2)
    VERIFY_SINGLE_VALUE(vnode->data.pos[2], ==, 3)
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "deletion"
    free_vec_ll_node(vnode, NULL);
    VERIFY_SINGLE_VALUE(1, ==, 1)
    COLLECT_FINDINGS

    DEBRIEF
}