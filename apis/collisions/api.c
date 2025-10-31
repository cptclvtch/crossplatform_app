//Dependencies
#include <stdlib.h>
#include <stdint.h>
#include "../geometric_algebra/api.c"
#include "../data_structures/linked_list/api.c"

#include "volumes.c"
#include "broad_phase.c"
#include "narrow_phase.c"

#ifndef _COLLISIONS_H
    #define _COLLISIONS_H

void gather_contacts(binary_tree* broad_phase_root, collision_list* list, const void (*custom_implementation)(binary_tree* root, collision_list* list));
#endif //_COLLISIONS_H

//----------------------------------

#if defined(INCLUDE_IMPLEMENTATION) && !defined(_COLLISIONS_C)
#define _COLLISIONS_C

void gather_contacts(binary_tree* broad_phase_root, collision_list* list, const void (*custom_implementation)(binary_tree* root, collision_list* list))
{
    //broad phase
    if(custom_implementation)
        custom_implementation(broad_phase_root, list);
    else
    {
        bvh_check_for_collision(broad_phase_root->child[0], broad_phase_root->child[1], (void*)list);
    }
    
    //narrow_phase
    uint32_t i = 0;
    for(; i < list->count; i++)
    {
        collision_detect(&list->pairs[i]);
    }
}
#endif