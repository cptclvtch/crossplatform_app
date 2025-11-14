//Dependencies
#include <stdlib.h>
#include <stdint.h>
#include "../geometric_algebra/api.c"

#include "volumes.c"
#include "broad_phase.c"
#include "narrow_phase.c"

#ifndef _COLLISIONS_H
    #define _COLLISIONS_H

void gather_contacts(bvh_bt_node* broad_phase_root, collision_list* list, const void (*custom_implementation)(bvh_bt_node* root, collision_list* list));
#endif //_COLLISIONS_H

//----------------------------------

#if defined(INCLUDE_IMPLEMENTATION) && !defined(_COLLISIONS_C)
    #define _COLLISIONS_C

// uint32_t pairs_in_bvh;
// void _bvh_leaf_func(bvh_bt_node* a, bvh_bt_node* b, collision_list* list)
// {
//     pairs_in_bvh++;
//     if(pairs_in_bvh < list->count)
//     {
//         list->pairs[pairs_in_bvh]->members[0] = a;
//         list->pairs[pairs_in_bvh]->members[1] = b;
//     }
// }

void gather_contacts(bvh_bt_node* broad_phase_root, collision_list* list, const void (*custom_implementation)(bvh_bt_node* root, collision_list* list))
{
    if(broad_phase_root == NULL) return;
    
    //broad phase
    if(custom_implementation)
        custom_implementation(broad_phase_root, list);
    else
    {
    //     if(broad_phase_root == NULL) printf("Broad phase root NULL\n");
    //     if(broad_phase_root->child[0] == NULL) printf("Child 0 NULL\n");
    //     if(broad_phase_root->child[1] == NULL) printf("Child 1 NULL\n");
    //     if(list == NULL) printf("Collision list NULL\n");
        bvh_check_for_collision(broad_phase_root->child[0], broad_phase_root->child[1], list);
    }
    
    //broad phase placeholder
    // list->pairs[0].members[0] = broad_phase_root->child[0]->data.volume;
    // list->pairs[0].members[1] = broad_phase_root->child[1]->data.volume;
    // list->count = 1;
    // printf("collision pair: %p %p\n", list->pairs[0].members[0], list->pairs[0].members[1]);

    //narrow_phase
    uint32_t i = 0;
    for(; i < list->count; i++)
    {
        collision_detect(&list->pairs[i]);
    }
}
#endif