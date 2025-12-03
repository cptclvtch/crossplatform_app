//Dependencies
#include "aabb.c"
#include "contacts.c"

#ifndef _BROAD_PHASE_H
    #define _BROAD_PHASE_H

typedef struct
{
    aabb box;
    collision_volume* volume;
}bvh_data;

#define DESIRED_TYPE bvh_data
#define DESIRED_PREFIX bvh
#define ASSISTING_TYPE collision_list*
#include "../data_structures/binary_tree/api.c"

inline bvh_data bvh_data_from_volume(collision_volume* v);
void bvh_upward_aabb_update(bvh_bt_node* a);
bvh_bt_node* bvh_insert(bvh_bt_node* root, bvh_bt_node* to_insert);

void bvh_check_for_collision(bvh_bt_node* a, bvh_bt_node* b, collision_list* user_data);
#endif //_BVH_H

//----------------------------------

#if defined(INCLUDE_IMPLEMENTATION) && !defined(_BROAD_PHASE_C)
    #define _BROAD_PHASE_C

//building BVH
inline bvh_data bvh_data_from_volume(collision_volume* volume)
{
    return (bvh_data){calculate_aabb_from_volume(volume, BASIC_CALCULATION), volume};
}

void bvh_upward_aabb_update(bvh_bt_node* a)
{
    if(a == NULL) return;

    uint8_t count = 0;
    aabb volumes[2] = {0};

    if(a->child[0]) volumes[count++] = a->child[0]->data.box;
    if(a->child[1]) volumes[count++] = a->child[1]->data.box;

    if(count)
        a->data.box = aabb_union(volumes[0], volumes[1]);

    bvh_upward_aabb_update(a->parent);
}

uint8_t _insertion_test(bvh_bt_node* node, bvh_bt_node* to_insert, collision_list* user_data)
{
    real growth[2] = {fl2real(0), fl2real(0)};
    uint8_t i = 0;
    for(; i < 2; i++)
    {
        bvh_data child = node->child[i]->data;
        real old_volume =   m_mul(  child.box.half_size.x,
                            m_mul(  child.box.half_size.y,
                                    child.box.half_size.z));

        aabb new = aabb_union(child.box, to_insert->data.box);
        real new_volume =   m_mul(  new.half_size.x,
                            m_mul(  new.half_size.y,
                                    new.half_size.z));
        growth[i] = new_volume - old_volume;
    }
    
    return growth[1] < growth[0];
}

bvh_bt_node* bvh_insert(bvh_bt_node* root, bvh_bt_node* to_insert)
{
    bvh_bt_node* result = bvh_bt_insert_leaf(root, to_insert, _insertion_test, NULL);
    if(result != NULL)
        bvh_upward_aabb_update(to_insert);
    
    return result;
}

//broad phase collision
static inline uint8_t _self_collision_test(bvh_bt_node* a, bvh_bt_node* b, collision_list* collisions)
{
    return aabb_aabb_check(a->data.box, b->data.box);
}

void _self_collision_func(bvh_bt_node* a, bvh_bt_node* b, collision_list* collisions)
{
    if( a->child[0] == NULL && a->child[1] == NULL
        &&
        b->child[0] == NULL && b->child[1] == NULL)
    {
        //TODO switch this to pairs of volumes between group members
        collision_pair* p = NULL;
        if(collisions->count < COLLISION_CHUNK_SIZE)
            p = &(collisions->pairs[collisions->count]);

        if(p)
        {
            //generate contact
            p->type = POTENTIAL_COLLISION;
            p->members[0] = a->data.volume;
            p->members[1] = b->data.volume;
        }
        
        collisions->count++;
    }
}

void bvh_check_for_collision(bvh_bt_node* a, bvh_bt_node* b, collision_list* user_data)
{
    bvh_bt_cross_traversal(a, b, _self_collision_test, _self_collision_func, user_data);
    bvh_bt_sibling_traversal(a, _self_collision_test, _self_collision_func, user_data);
    bvh_bt_sibling_traversal(b, _self_collision_test, _self_collision_func, user_data);
}
//TODO fix memory leaks when using bvhs
#endif