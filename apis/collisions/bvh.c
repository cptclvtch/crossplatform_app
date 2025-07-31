#ifndef API_IMPLEMENTATION_ONLY
/*
- possibly collect stats while traversing or inserting?
- static and dynamic split at root?
- split static branch into permanent and dormant?
*/
#ifndef BINARY_TREE
#error "Please include data_structures/binary_tree/api.c first."
#endif

typedef struct
{
    binary_tree node;
    aabb box;
}bvh_data;

bvh_data* bvh_get_new_node();
void bvh_upward_aabb_update(bvh_data* a);

bvh_data* bvh_insert(bvh_data* root, bvh_data* to_insert);
void bvh_traverse_for_self_collision(bvh_data* a, bvh_data* b);

void bvh_delete_node(bvh_data* a);

//----------------------------------
#else
//----------------------------------

bvh_data* bvh_get_new_node()
{
    bvh_data* new = (bvh_data*)calloc(1, sizeof(bvh_data));
    return new;
}

void bvh_upward_aabb_update(bvh_data* a)
{
    if(a == NULL) return;

    uint8_t count = 0;
    aabb volumes[2];

    if(a->node.child[0]) volumes[count++] = ((bvh_data*)a->node.child[0])->box;
    if(a->node.child[1]) volumes[count++] = ((bvh_data*)a->node.child[1])->box;

    if(count)
        a->box = aabb_for_pair( volumes[0],
                                volumes[1], AABB_FULL_CALCULATION);

    bvh_upward_aabb_update((bvh_data*)a->node.parent);
}

uint8_t _insertion_test(binary_tree* node, binary_tree* to_insert)
{
    real growth[2] = {fl2real(0), fl2real(0)};
    uint8_t i = 0;
    for(; i < 2; i++)
    {
        if(node->child[i] != NULL)
        {
            real old_volume =   m_mul(  ((bvh_data*)node->child[i])->box.half_size.x,
                                m_mul(  ((bvh_data*)node->child[i])->box.half_size.y,
                                        ((bvh_data*)node->child[i])->box.half_size.z));

            aabb new = aabb_for_pair(((bvh_data*)node->child[i])->box, ((bvh_data*)to_insert)->box, AABB_SIZE_CALCULATION);
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

bvh_data* bvh_insert(bvh_data* root, bvh_data* to_insert)
{
    bvh_data* result = binary_tree_insert_leaf((binary_tree*)root, (binary_tree*)to_insert, &_insertion_test);
    bvh_upward_aabb_update(to_insert);
    return result;
}

collision_list bvh_collisions;
uint8_t _self_collision_test(binary_tree* a, binary_tree* b)
{
    return aabb_check(((bvh_data*)a)->box, ((bvh_data*)b)->box);
}

void _self_collision_func(binary_tree* a, binary_tree* b)
{
    //TODO implement this once contacts.c is done
    //TODO would the contact list be a global variable
    if( a->child[0] == NULL && a->child[1] == NULL
        &&
        b->child[0] == NULL && b->child[1] == NULL)
    {
        bvh_collisions.last_index++;
        collision_data* cd = NULL;
        if(bvh_collisions.last_index < COLLISION_CHUNK_SIZE)
            cd = &bvh_collisions.pairs[bvh_collisions.last_index];

        if(cd)
        {
            //generate contact
            cd->type = POTENTIAL_COLLISION;
            cd->data[0] = a;
            cd->data[1] = b;
        }
    }
}

void bvh_traverse_for_self_collision(bvh_data* a, bvh_data* b)
{
    binary_tree_cross_traversal((binary_tree*)a, (binary_tree*)b, &_self_collision_test, &_self_collision_func);
    binary_tree_sibling_traversal(a, &_self_collision_test, &_self_collision_func);
    binary_tree_sibling_traversal(b, &_self_collision_test, &_self_collision_func);
}

void bvh_delete_node(bvh_data* a)
{
    if(a == NULL) return;

    binary_tree* p = ((binary_tree*)a)->parent;

    binary_tree_delete((binary_tree*)a);

    bvh_upward_aabb_update(p);
    //Dont do this if using bvh_data in an inheritance style (at the front of a larger struct)
    // free(a->node);
    // free(a);
}
#endif