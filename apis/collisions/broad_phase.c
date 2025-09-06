//Dependencies
#include "../data_structures/binary_tree/api.c"
#include "aabb.c"
#include "contacts.c"

#ifndef _BROAD_PHASE_H
    #define _BROAD_PHASE_H
//TODO consider making this an API

typedef struct
{
    aabb box;
    collision_volume* group;
}bvh_data;

bvh_data* bvh_new_data();
void bvh_upward_aabb_update(binary_tree* a);

binary_tree* bvh_insert(binary_tree* root, binary_tree* to_insert);
void bvh_traverse_for_self_collision(binary_tree* a, binary_tree* b);
#endif //_BVH_H

//----------------------------------

#if defined(INCLUDE_IMPLEMENTATION) && !defined(_BROAD_PHASE_C)
    #define _BROAD_PHASE_C

inline bvh_data* bvh_new_data()
{
    return (bvh_data*)calloc(1, sizeof(bvh_data));
}

void bvh_upward_aabb_update(binary_tree* a)
{
    if(a == NULL) return;
    if(a->data == NULL) a->data = bvh_new_data();

    uint8_t count = 0;
    aabb volumes[2] = {0};

    if(a->child[0] && a->child[0]->data) volumes[count++] = ((bvh_data*)a->child[0]->data)->box;
    if(a->child[1] && a->child[1]->data) volumes[count++] = ((bvh_data*)a->child[1]->data)->box;

    if(count)
        ((bvh_data*)a->data)->box = aabb_union(volumes[0], volumes[1]);

    bvh_upward_aabb_update(a->parent);
}

uint8_t _insertion_test(binary_tree* node, binary_tree* to_insert)
{
    real growth[2] = {fl2real(0), fl2real(0)};
    uint8_t i = 0;
    for(; i < 2; i++)
    {
        bvh_data* child = node->child[i]->data;
        if(child != NULL)
        {
            real old_volume =   m_mul(  child->box.half_size.x,
                                m_mul(  child->box.half_size.y,
                                        child->box.half_size.z));

            aabb new = aabb_union(child->box, ((bvh_data*)to_insert->data)->box);
            real new_volume =   m_mul(  new.half_size.x,
                                m_mul(  new.half_size.y,
                                        new.half_size.z));
            growth[i] = new_volume - old_volume;
        }
        else
            growth[i] = fl2real(0);
    }
    
    return growth[1] < growth[0];
}

binary_tree* bvh_insert(binary_tree* root, binary_tree* to_insert)
{
    binary_tree* result = binary_tree_insert_leaf(root, to_insert, _insertion_test);
    if(result == NULL) return NULL;

    bvh_upward_aabb_update(to_insert);
    return result;
}

//TODO fix memory leaks when using bvhs
#endif