#include "../../unit_testing.c"

#include "../../geometric_algebra/api.c"
#include "../contacts.c"
#define API_IMPLEMENTATION_ONLY
#include "../../geometric_algebra/api.c"
#include "../contacts.c"

int main()
{
    #define TITLE "contact list"
    POST_TITLE

    collision_list list;

    #undef SUBTITLE
    #define SUBTITLE "clearing"
    uint16_t i = 0;
    for(; i < COLLISION_CHUNK_SIZE; i++)
        list.pairs[i].type = POTENTIAL_COLLISION;
    
    clear_collision_list(&list);
    VERIFY_ARRAY_OF_VALUES(list.pairs[test_iterator].type, ==, NO_COLLISION, COLLISION_CHUNK_SIZE)
    COLLECT_FINDINGS

    DEBRIEF
}