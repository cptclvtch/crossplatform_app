#ifndef API_IMPLEMENTATION_ONLY
/*
- possibly collect stats while traversing or inserting?
- static and dynamic split at root?
- split static branch into permanent and dormant?
*/
#ifndef BINARY_TREE
#error "Please include data_structures/binary_tree/api.c first."
#endif

#include "aabb.c"

typedef struct
{
    binary_tree node;
    aabb box;
}bvh_data;

bvh_data* bvh_get_new_node();
void bvh_upward_aabb_update(bvh_data* a);

bvh_data* bvh_insert(bvh_data* root, bvh_data* to_insert);
void bvh_traverse_for_self_collision(bvh_node* a, bvh_node* b, void (*narrow_phase_fn)(collision_data* c), collision_list* contacts);

void bvh_delete_node(bvh_data* a);

//----------------------------------
#else
//----------------------------------

bvh_data* bvh_get_new_node()
{
    bvh_data* new = (bvh_data*)calloc(1, sizeof(bvh_data));
    new->node = binary_tree_new();
    return new;
}

void bvh_upward_aabb_update(bvh_data* a)
{
    if(a == NULL) return;

    aabb volume_a = {0};
    aabb volume_b = {0};

    if(a->node.child[0]) volume_a = a->node.child[0]->box;
    if(a->node.child[1]) volume_b = a->node.child[1]->box;

    a->box = aabb_for_pair( volume_a,
                            volume_b, AABB_FULL_CALCULATION);

    bvh_upward_aabb_update((bvh_data*)(a->node->parent));
}

uint8_t _insertion_test(binary_tree* a, binary_tree* b)
{
    bvh_data* combo = {(bvh_data*)a, (bvh_data*)b};
    real growth[2] = {fl2real(0), fl2real(0)};
    uint8_t i = 0;
    for(; i < 2; i++)
    {
        if(combo[i] != NULL)
        {
            real old_volume =   m_mul(  combo[i]->box.half_size.x,
                                m_mul(  combo[i]->box.half_size.y,
                                        combo[i]->box.half_size.z));

            aabb new_aabb = aabb_for_pair(combo[i], to_insert, AABB_SIZE_CALCULATION);
            real new_volume =   m_mul(  new_aabb.half_size.x,
                                m_mul(  new_aabb.half_size.y,
                                        new_aabb.half_size.z));
            growth[i] = new_volume - old_volume;
        }
        else
            growth[i] = fl2real(0);
    }
    
    return growth[1] < growth[0];    
}

bvh_data* bvh_insert(bvh_data* root, bvh_data* to_insert)
{
    bvh_data* result = binary_tree_insert_leaf((binary_tree*)root, (binary_tree*)to_insert, &_insertion_test);
    bvh_upward_aabb_update(to_insert);
    return result;
}

uint8_t _self_collision_test(binary_tree* a, binary_tree* b)
{
    return aabb_check((bvh_data*)a->box, (bvh_data*)b->box);
}

void _self_collision_func(binary_tree* a, binary_tree* b)
{
    //TODO implement this once contacts.c is done
    //TODO would the contact list be a global variables
    if( a->child[0] == NULL && a->child[1] == NULL
        &&
        b->child[0] == NULL && b->child[1] == NULL)
    {
        // collision_data* cd = NULL;
        // if(contacts->last_index < COLLISION_CHUNK_SIZE)
        //     cd = &contacts->pairs[contacts->last_index];

        // if(cd)
        // {
        //     //generate contact
        //     cd->type = POTENTIAL_COLLISION;
        //     cd->data[0] = a->data;
        //     cd->data[1] = b->data;
        // }

        // if(narrow_phase_fn)
        // {
        //     collision_data temp;
        //     if(!cd)
        //     {
        //         cd = &temp;
        //         cd->type = POTENTIAL_COLLISION;
        //         cd->data[0] = a->data;
        //         cd->data[1] = b->data;
        //     }
        //     narrow_phase_fn(cd);
        // }
    }
}

void bvh_traverse_for_self_collision(bvh_node* a, bvh_node* b, void (*narrow_phase_fn)(collision_data* c), collision_list* contacts)
{
    binary_tree_cross_traversal((binary_tree*)a, (binary_tree*)b, &_self_collision_test, &self_collision_func);
    binary_tree_sibling_traversal(a, &self_collision_test, &_self_collision_func);
    binary_tree_sibling_traversal(b, &self_collision_test, &_self_collision_func);
}

void bvh_delete_node(bvh_data* a)
{
    binary_tree_delete(a->node);

    //Dont do this if using bvh_data in an inheritance style (at the front of a larger struct)
    // free(a->node);
    // free(a);
}
#endif