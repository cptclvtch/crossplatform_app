#include "../../unit_testing.c"

#include "api.c"
#define INCLUDE_IMPLEMENTATION
#include "api.c"

int main()
{
    CLEAR_SCREEN

    linked_list_node* node_a = NULL;
    linked_list_node* node_b = NULL;
    #define TITLE "linked list node"
    POST_TITLE

    #undef SUBTITLE
    #define SUBTITLE "get_new_link - Best Case Scenario"
    node_a = get_new_link(NULL);
    VERIFY_SINGLE_VALUE(node_a, !=, NULL)
    VERIFY_SINGLE_VALUE(node_a->data, ==, NULL)
    delete_link(node_a, NULL); node_a = NULL;
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "add_link_before - NULL add_to"
    node_a = add_link_before(NULL, NULL);
    VERIFY_SINGLE_VALUE(node_a, !=, NULL)
    VERIFY_SINGLE_VALUE(node_a->data, ==, NULL)
    COLLECT_FINDINGS
    
    #undef SUBTITLE
    #define SUBTITLE "add_link_before - Best Case Scenario"
    node_b = add_link_before(node_a, NULL);
    VERIFY_SINGLE_VALUE(node_b, !=, NULL)
    VERIFY_SINGLE_VALUE(node_b->data, ==, NULL)
    VERIFY_SINGLE_VALUE(node_b->NEXT, == , node_a)
    VERIFY_SINGLE_VALUE(node_b->PREV, == , NULL)
    COLLECT_FINDINGS

    delete_link(node_a, NULL); node_a = NULL;
    delete_link(node_b, NULL); node_b = NULL;

    #undef SUBTITLE
    #define SUBTITLE "add_link_after - NULL add_to"
    node_a = add_link_after(NULL, NULL);
    VERIFY_SINGLE_VALUE(node_a, !=, NULL)
    VERIFY_SINGLE_VALUE(node_a->data, ==, NULL)
    COLLECT_FINDINGS
    
    #undef SUBTITLE
    #define SUBTITLE "add_link_after - Best Case Scenario"
    node_b = add_link_after(node_a, NULL);
    VERIFY_SINGLE_VALUE(node_b, !=, NULL)
    VERIFY_SINGLE_VALUE(node_b->data, ==, NULL)
    VERIFY_SINGLE_VALUE(node_b->PREV, == , node_a)
    VERIFY_SINGLE_VALUE(node_b->NEXT, == , NULL)
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "delete_link_chain - Best Case Scenario - shouldnt crash"
    delete_link_chain(node_a, NULL, 1);
    VERIFY_SINGLE_VALUE(1,==,1);
    COLLECT_FINDINGS

    node_a = NULL;
    node_b = NULL;

    ADD_SEPARATOR

    linked_list list_a;
    #undef TITLE
    #define TITLE "linked list"
    POST_TITLE

    #undef SUBTITLE
    #define SUBTITLE "create_new_list - NULL delete function"
    list_a = create_new_list(NULL);
    VERIFY_SINGLE_VALUE(list_a.delete_func, ==, NULL)
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "delete_list - NULL delete function"
    list_a.nodes = get_new_link(NULL);
    delete_list(&list_a);
    VERIFY_SINGLE_VALUE(list_a.nodes, ==, NULL)
    COLLECT_FINDINGS
    
    #undef SUBTITLE
    #define SUBTITLE "create_new_list - DONT_DELETE_CONTENTS"
    list_a = create_new_list(DONT_DELETE_CONTENTS);
    VERIFY_SINGLE_VALUE(list_a.delete_func, ==, DONT_DELETE_CONTENTS)
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "delete_list - DONT_DELETE_CONTENTS"
    list_a.nodes = get_new_link(malloc(2));
    void* a = list_a.nodes->data;
    delete_list(&list_a);
    VERIFY_SINGLE_VALUE(a, !=, NULL)
    VERIFY_SINGLE_VALUE(list_a.nodes, ==, NULL)
    COLLECT_FINDINGS

    DEBRIEF
}