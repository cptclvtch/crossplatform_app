#include "../../unit_testing.c"

typedef struct
{
    char a,b;
}test_struct;

#define INCLUDE_IMPLEMENTATION

#define DESIRED_TYPE test_struct
#include "api.c"

int main()
{
    chunked_array* a;
    test_struct* b;

    #define TITLE "creation and deletion"
    POST_TITLE

    #define SUBTITLE "create_chunked_array - Best Case Scenario"
    a = create_chunked_array(2,2);
    VERIFY_SINGLE_VALUE(a, !=, NULL);
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "create_chunked_array - size is zero"
    VERIFY_SINGLE_VALUE(create_chunked_array(0,2), ==, NULL);
    VERIFY_SINGLE_VALUE(create_chunked_array(2,0), ==, NULL);
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "free_chunked_array(NULL)"
    free_chunked_array(NULL);
    VERIFY_SINGLE_VALUE(1, ==, 1);
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "free_chunked_array() - Best Case Scenario"
    free_chunked_array(a);
    VERIFY_SINGLE_VALUE(1, ==, 1);
    COLLECT_FINDINGS

    #undef TITLE
    #define TITLE "addition and removal"
    POST_TITLE

    a = create_chunked_array(50, sizeof(test_struct));

    #undef SUBTITLE
    #define SUBTITLE "add_to_chunked_array - Best Case Scenario"
    b = calloc(1,sizeof(test_struct));
    b->a = 2;
    add_to_chunked_array(a, b);
    VERIFY_SINGLE_VALUE(a->occupancy->last_index, ==, 0);
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "add_to_chunked_array - repeated use"
    b = calloc(1,sizeof(test_struct));
    b->a = 3;
    add_to_chunked_array(a, b);
    VERIFY_SINGLE_VALUE(a->occupancy->last_index, ==, 1);
    COLLECT_FINDINGS

    #undef TITLE
    #define TITLE "data retrieval"
    POST_TITLE

    #undef SUBTITLE
    #define SUBTITLE "Best case scenario"
    b = (test_struct*)chunked_array_at(a, 0);
    VERIFY_SINGLE_VALUE(b->a, ==, 2);
    COLLECT_FINDINGS

    DEBRIEF
}