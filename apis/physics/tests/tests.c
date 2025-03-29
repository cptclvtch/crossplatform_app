#include "../../unit_testing.c"

#include "../../geometric_algebra/api.c"
#include "../../linked_list/api.c"
#include "../api.c"
#define API_IMPLEMENTATION_ONLY
#include "../../geometric_algebra/api.c"
#include "../../linked_list/api.c"
#include "../api.c"

int main()
{
    CLEAR_SCREEN

    #include "common.c"
    #include "phys_point.c"
    #include "particle_bunch.c"
    #include "phys_virtual_spring.c"

    ADD_SEPARATOR
    #include "phys_world.c"

    ADD_SEPARATOR
    #include "determinism.c"

    DEBRIEF
}