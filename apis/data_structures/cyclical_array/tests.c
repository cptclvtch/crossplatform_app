#include "../../unit_testing.c"

#define TYPE char*
#define PREFIX charp
#define INCLUDE_IMPLEMENTATION
#include "api.c"

int main()
{
    charp_cyclical_array* a;

    #define TITLE "creation and freeing"
    POST_TITLE

    #define SUBTITLE "creation - Best Case Scenario"
    a = create_charp_cyclical_array(2);
    VERIFY_SINGLE_VALUE(a->data, !=, NULL)
    VERIFY_SINGLE_VALUE(a->max_index, ==, 2)
    COLLECT_FINDINGS


    DEBRIEF
}