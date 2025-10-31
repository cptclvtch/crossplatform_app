//Dependencies
#include "../data_structures/binary_tree/api.c"
#include "aabb.c"
#include "contacts.c"

#ifndef _BROAD_PHASE_H
    #define _BROAD_PHASE_H

typedef struct
{
    aabb box;
    collision_volume* volume;
}bvh_data;

bvh_data* bvh_new_data();
bvh_data* bvh_data_from_volume(collision_volume* v);
void bvh_upward_aabb_update(binary_tree* a);
binary_tree* bvh_insert(binary_tree* root, binary_tree* to_insert);

void bvh_check_for_collision(binary_tree* a, binary_tree* b, void* user_data);
#endif //_BVH_H

//----------------------------------

#if defined(INCLUDE_IMPLEMENTATION) && !defined(_BROAD_PHASE_C)
    #define _BROAD_PHASE_C

//building BVH
inline bvh_data* bvh_new_data()
{
    return (bvh_data*)calloc(1, sizeof(bvh_data));
}

bvh_data* bvh_data_from_volume(collision_volume* volume)
{
    bvh_data* new_data = bvh_new_data();

    new_data->volume = volume;
    new_data->box = calculate_aabb_from_volume(new_data->volume, BASIC_CALCULATION);

    return new_data;
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

uint8_t _insertion_test(binary_tree* node, binary_tree* to_insert, void* user_data)
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
    binary_tree* result = binary_tree_insert_leaf(root, to_insert, _insertion_test, NULL);
    if(result != NULL)
        bvh_upward_aabb_update(to_insert);
    
    return result;
}

//broad phase collision
static inline uint8_t _self_collision_test(binary_tree* a, binary_tree* b, void* user_data)
{
    return aabb_aabb_check(((bvh_data*)a->data)->box, ((bvh_data*)b->data)->box);
}

void _self_collision_func(binary_tree* a, binary_tree* b, void* user_data)
{
    collision_list* collisions = user_data;

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
            p->members[0] = a->data;
            p->members[1] = b->data;
        }
        
        collisions->count++;
    }
}

void bvh_check_for_collision(binary_tree* a, binary_tree* b, void* user_data)
{
    binary_tree_cross_traversal(a, b, _self_collision_test, _self_collision_func, user_data);
    binary_tree_sibling_traversal(a, _self_collision_test, _self_collision_func, user_data);
    binary_tree_sibling_traversal(b, _self_collision_test, _self_collision_func, user_data);
}
//TODO fix memory leaks when using bvhs
#endif