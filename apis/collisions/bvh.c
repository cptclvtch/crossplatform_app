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
    aabb box;
    //game object
    //mesh
}bvh_data;

bvh_data* bvh_new_data();
void bvh_upward_aabb_update(binary_tree* a);

binary_tree* bvh_insert(binary_tree* root, binary_tree* to_insert);
void bvh_traverse_for_self_collision(binary_tree* a, binary_tree* b);

//----------------------------------
#else
//----------------------------------

bvh_data* bvh_new_data()
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
        ((bvh_data*)a->data)->box = aabb_for_pair(  volumes[0],
                                                    volumes[1], AABB_FULL_CALCULATION);

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

            aabb new = aabb_for_pair(child->box, ((bvh_data*)to_insert->data)->box, AABB_SIZE_CALCULATION);
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

collision_list bvh_collisions;
inline uint8_t _self_collision_test(binary_tree* a, binary_tree* b)
{
    return aabb_check(((bvh_data*)a->data)->box, ((bvh_data*)b->data)->box);
}

void _self_collision_func(binary_tree* a, binary_tree* b)
{
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

void bvh_traverse_for_self_collision(binary_tree* a, binary_tree* b)
{
    binary_tree_cross_traversal(a, b, _self_collision_test, _self_collision_func);
    binary_tree_sibling_traversal(a, _self_collision_test, _self_collision_func);
    binary_tree_sibling_traversal(b, _self_collision_test, _self_collision_func);
}

//TODO fix memory leaks when using bvhs

#endif