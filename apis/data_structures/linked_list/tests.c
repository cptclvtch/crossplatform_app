#include "../../unit_testing.c"

#include "api.c"

int main()
{
    CLEAR_SCREEN

    ll_node* node_a = NULL;
    ll_node* node_b = NULL;
    #define TITLE "linked list node"
    POST_TITLE

    #undef SUBTITLE
    #define SUBTITLE "create_ll_node - Best Case Scenario"
    node_a = create_ll_node(NULL);
    VERIFY_SINGLE_VALUE(node_a, !=, NULL)
    VERIFY_SINGLE_VALUE(node_a->data, ==, NULL)
    free_ll_node(node_a); node_a = NULL;
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "ll_add_data(PREV) - NULL add_to"
    node_a = ll_add_data(NULL, PREV, NULL);
    VERIFY_SINGLE_VALUE(node_a, !=, NULL)
    VERIFY_SINGLE_VALUE(node_a->data, ==, NULL)
    COLLECT_FINDINGS
    
    #undef SUBTITLE
    #define SUBTITLE "ll_add_data(PREV) - Best Case Scenario"
    node_b = ll_add_data(node_a, PREV, NULL);
    VERIFY_SINGLE_VALUE(node_b, !=, NULL)
    VERIFY_SINGLE_VALUE(node_b->data, ==, NULL)
    VERIFY_SINGLE_VALUE(node_b->NEXT, == , node_a)
    VERIFY_SINGLE_VALUE(node_b->PREV, == , NULL)
    COLLECT_FINDINGS

    free_ll_node(node_a); node_a = NULL;
    free_ll_node(node_b); node_b = NULL;

    #undef SUBTITLE
    #define SUBTITLE "ll_add_data(NEXT) - NULL add_to"
    node_a = ll_add_data(NULL, NEXT, NULL);
    VERIFY_SINGLE_VALUE(node_a, !=, NULL)
    VERIFY_SINGLE_VALUE(node_a->data, ==, NULL)
    COLLECT_FINDINGS
    
    #undef SUBTITLE
    #define SUBTITLE "ll_add_data(NEXT) - Best Case Scenario"
    node_b = ll_add_data(node_a, NEXT, NULL);
    VERIFY_SINGLE_VALUE(node_b, !=, NULL)
    VERIFY_SINGLE_VALUE(node_b->data, ==, NULL)
    VERIFY_SINGLE_VALUE(node_b->PREV, == , node_a)
    VERIFY_SINGLE_VALUE(node_b->NEXT, == , NULL)
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "ll_add_link(PREV) - NULL add_to"
    node_a = ll_add_link(NULL, PREV, NULL);
    VERIFY_SINGLE_VALUE(node_a, !=, NULL)
    VERIFY_SINGLE_VALUE(node_a->data, ==, NULL)
    COLLECT_FINDINGS
    
    #undef SUBTITLE
    #define SUBTITLE "ll_add_link(PREV) - NULL link_in"
    node_b = ll_add_link(node_a, PREV, NULL);
    VERIFY_SINGLE_VALUE(node_b, !=, NULL)
    VERIFY_SINGLE_VALUE(node_b->data, ==, NULL)
    VERIFY_SINGLE_VALUE(node_b->NEXT, == , node_a)
    VERIFY_SINGLE_VALUE(node_b->PREV, == , NULL)
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "ll_add_link(PREV) - Best Case Scenario"
    ll_node* temp = create_ll_node(NULL);
    ll_add_link(node_b, PREV, temp);
    VERIFY_SINGLE_VALUE(node_b->PREV, ==, temp)
    VERIFY_SINGLE_VALUE(temp->NEXT, ==, node_b)
    COLLECT_FINDINGS

    free_ll_node(node_a); node_a = NULL;
    free_ll_node(node_b); node_b = NULL;

    #undef SUBTITLE
    #define SUBTITLE "ll_add_link(NEXT) - NULL add_to"
    node_a = ll_add_link(NULL, NEXT, NULL);
    VERIFY_SINGLE_VALUE(node_a, !=, NULL)
    VERIFY_SINGLE_VALUE(node_a->data, ==, NULL)
    COLLECT_FINDINGS
    
    #undef SUBTITLE
    #define SUBTITLE "ll_add_link(NEXT) - NULL link_in"
    node_b = ll_add_link(node_a, NEXT, NULL);
    VERIFY_SINGLE_VALUE(node_a, !=, NULL)
    VERIFY_SINGLE_VALUE(node_b->data, ==, NULL)
    COLLECT_FINDINGS
    
    #undef SUBTITLE
    #define SUBTITLE "ll_add_link(NEXT) - Best Case Scenario"
    ll_add_link(node_a, NEXT, temp);
    VERIFY_SINGLE_VALUE(node_a->NEXT, == , temp)
    VERIFY_SINGLE_VALUE(temp->PREV, ==, node_a)
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "free_link_chain - Best Case Scenario - shouldnt crash"
    free_ll_chain(node_a, NEXT);
    VERIFY_SINGLE_VALUE(1,==,1);
    COLLECT_FINDINGS

    DEBRIEF
}