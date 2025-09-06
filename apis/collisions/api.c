//Dependencies
#include <stdlib.h>
#include "../geometric_algebra/api.c"
#include "../data_structures/linked_list/api.c"

#ifndef _COLLISIONS_H
    #define _COLLISIONS_H
    
#include "volumes.c"
#include "broad_phase.c"
#include "narrow_phase.c"

/*
- static and dynamic split at root?
- split static branch into permanent and dormant?
*/

void contact_generation(collision_list list);

// uint8_t point_2_mesh_collision(vec3 p, collision_mesh m, collision_data* data);
#endif //_COLLISIONS_H

//----------------------------------

#if defined(INCLUDE_IMPLEMENTATION) && !defined(_COLLISIONS_C)
#define _COLLISIONS_C

#include "volumes.c"
#include "broad_phase.c"
#include "narrow_phase.c"

collision_list bvh_collisions;
static inline uint8_t _self_collision_test(binary_tree* a, binary_tree* b)
{
    return aabb_aabb_check(((bvh_data*)a->data)->box, ((bvh_data*)b->data)->box);
}

void _self_collision_func(binary_tree* a, binary_tree* b)
{
    if( a->child[0] == NULL && a->child[1] == NULL
        &&
        b->child[0] == NULL && b->child[1] == NULL)
    {
        //TODO switch this to pairs of volumes between group members
        collision_pair* p = NULL;
        if(bvh_collisions.count < COLLISION_CHUNK_SIZE)
            p = &bvh_collisions.pairs[bvh_collisions.count-1];

        if(p)
        {
            //generate contact
            p->type = POTENTIAL_COLLISION;
            p->members[0] = a;
            p->members[1] = b;
        }
        
        bvh_collisions.count++;
    }
}

void bvh_check_for_collision(binary_tree* a, binary_tree* b)
{
    binary_tree_cross_traversal(a, b, _self_collision_test, _self_collision_func);
    binary_tree_sibling_traversal(a, _self_collision_test, _self_collision_func);
    binary_tree_sibling_traversal(b, _self_collision_test, _self_collision_func);
}

void contact_generation(collision_list list)
{
    uint16_t i = 0;



    for(; i < list.count; i++)
    {
        // switch(list[i].)
    }
}
#endif