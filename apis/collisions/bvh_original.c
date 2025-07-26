#ifndef API_IMPLEMENTATION_ONLY
/*
- possibly collect stats while traversing or inserting?
- static and dynamic split at root?
- split static branch into permanent and dormant?
*/
typedef struct bvh_node
{
    struct bvh_node* parent;
    struct bvh_node* children[2];

    vec3 center;
    vec3 half_size;

    void* data;
}bvh_node;

bvh_node* bvh_get_empty_node();

// void bvh_traverse_for_special_collision(bvh_node* root, uint8_t (*collision_fn)(bvh_node* n, void* data), collision_list* contacts);
void bvh_traverse_for_self_collision(bvh_node* a, bvh_node* b, void (*narrow_phase_fn)(collision_data* c), collision_list* contacts);

enum{
    AABB_FULL_CALCULATION,
    AABB_SIZE_CALCULATION
};
bvh_node bvh_aabb_for_pair(bvh_node* a, bvh_node* b, uint8_t size_only);
void bvh_upward_aabb_update(bvh_node* a);
bvh_node* bvh_insert(bvh_node* root, bvh_node* to_insert);
void bvh_delete_node(bvh_node* a);
//----------------------------------
#else
//----------------------------------
uint8_t bvh_aabb_check(bvh_node* a, bvh_node* b)
{   
    if(!a || !b) return 0;
    //just like spheres, but even easier
    vec3 center_diff = vec_abs(vec_subtract(a->center, b->center));
    vec3 total_size = vec_add(a->half_size, b->half_size);
    return (center_diff.x <= total_size.x &&
            center_diff.y <= total_size.y &&
            center_diff.z <= total_size.z);
}

inline bvh_node* bvh_get_empty_node()
{
    bvh_node* new_node = (bvh_node*)calloc(1,sizeof(bvh_node));

    *new_node = (bvh_node){  .parent = NULL,
                            .children[0] = NULL,
                            .children[1] = NULL,
                            .data = NULL};
    
    return new_node;
}

enum
{
    BVH_NULL,
    BVH_EMPTY,
    BVH_LEAF,
    BVH_BRANCH
};
uint8_t bvh_node_type(bvh_node* node)
{
    /*  NULL pointer - invalid
                |0 children |   1 child |2 children 
        --------+-----------+-----------+-----------
        no data |empty      |branch     |branch
        --------+-----------+-----------+-----------
        data    |leaf       |branch     |branch
    */

    if(node == NULL) return BVH_NULL;
    if(node->children[0] == NULL && node->children[1] == NULL)
    {
        if(node->data) return BVH_LEAF;
        return BVH_EMPTY;
    }
    return BVH_BRANCH;
}

//frustum pyramid, ray casting, etc
// void bvh_traverse_for_special_collision(bvh_node* root, uint8_t (*collision_fn)(bvh_node* n, void* data), collision_list* contacts)
// {
// }

void bvh_traverse_for_self_collision(bvh_node* a, bvh_node* b, void (*narrow_phase_fn)(collision_data* c), collision_list* contacts)
{
    uint8_t node_type[2] = {bvh_node_type(a), bvh_node_type(b)};

    if((node_type[0] + node_type[1]) < 2) return;

    bvh_node* to_recurse[2] = {a,b};

    if(bvh_aabb_check(a,b))
    {
        switch(node_type[0]*node_type[1])
        {
            case BVH_LEAF * BVH_LEAF:
            {
                collision_data* cd = NULL;
                if(contacts->last_index < COLLISION_CHUNK_SIZE)
                    cd = &contacts->pairs[contacts->last_index];

                if(cd)
                {
                    //generate contact
                    cd->type = POTENTIAL_COLLISION;
                    cd->data[0] = a->data;
                    cd->data[1] = b->data;
                }

                if(narrow_phase_fn)
                {
                    collision_data temp;
                    if(!cd)
                    {
                        cd = &temp;
                        cd->type = POTENTIAL_COLLISION;
                        cd->data[0] = a->data;
                        cd->data[1] = b->data;
                    }
                    narrow_phase_fn(cd);
                }
                return;
            }

            uint8_t i,j;
            case BVH_LEAF * BVH_BRANCH:
            {
                uint8_t leaf_side = ((node_type[0] - node_type[1]) + 1)/2;
                for(i = 0; i < 2; i++)
                    bvh_traverse_for_self_collision(to_recurse[leaf_side],
                                                    to_recurse[!leaf_side]->children[i],
                                                    narrow_phase_fn, contacts);
                break;
            }

            case BVH_BRANCH * BVH_BRANCH:
            {
                for(i = 0; i < 2; i++)
                    for(j = 0; j < 2; j++)
                        bvh_traverse_for_self_collision(to_recurse[0]->children[i],
                                                        to_recurse[1]->children[j],
                                                        narrow_phase_fn, contacts);
            }
        }
    }

    //segregated traversal
    uint8_t node = 0;
    for(; node < 2; node++)
        if(node_type[node] == BVH_BRANCH)
            bvh_traverse_for_self_collision(to_recurse[node]->children[0],
                                            to_recurse[node]->children[1],
                                            narrow_phase_fn, contacts);
}

bvh_node bvh_aabb_for_pair(bvh_node* a, bvh_node* b, uint8_t size_only)
{
    if(a == NULL || b == NULL) return (bvh_node){0};
    vec3 max_corner = vec_max(  vec_add(a->center, a->half_size),
                                vec_add(b->center, b->half_size));

    vec3 min_corner = vec_min(  vec_subtract(a->center, a->half_size),
                                vec_subtract(b->center, b->half_size));

    bvh_node new_aabb;
    new_aabb.half_size = vec_scalar_divide(vec_subtract(max_corner, min_corner), 2);
    if(size_only) return new_aabb;

    new_aabb.center = vec_subtract(max_corner, new_aabb.half_size);

    return new_aabb;
}

void bvh_upward_aabb_update(bvh_node* a)
{
    uint8_t type = bvh_node_type(a);
    if(type == BVH_NULL) return;
    if(type == BVH_BRANCH)
    {
        bvh_node new_aabb = bvh_aabb_for_pair(a->children[0], a->children[1], AABB_FULL_CALCULATION);

        a->center = new_aabb.center;
        a->half_size = new_aabb.half_size;
    }

    //propagate up
    bvh_upward_aabb_update(a->parent);
}

bvh_node* bvh_insert(bvh_node* root, bvh_node* to_insert)
{
    if(to_insert == NULL) return root;
    if(root == NULL) return to_insert;

    //keep travelling down the root
    real growth[2] = {fl2real(0), fl2real(0)};
    uint8_t i = 0;
    for(; i < 2; i++)
    {
        if(root->children[i] != NULL)
        {
            real old_volume =   m_mul(  root->children[i]->half_size.x,
                                m_mul(  root->children[i]->half_size.y,
                                        root->children[i]->half_size.z));

            bvh_node new_aabb = bvh_aabb_for_pair(root->children[i], to_insert, AABB_SIZE_CALCULATION);
            real new_volume =   m_mul(  new_aabb.half_size.x,
                                m_mul(  new_aabb.half_size.y,
                                        new_aabb.half_size.z));
            growth[i] = new_volume - old_volume;
        }
        else
            growth[i] = fl2real(0);
    }
    
    uint8_t smaller_growth = growth[1] < growth[0];
    root->children[smaller_growth] = bvh_insert(root->children[smaller_growth], to_insert);

    return root;
}

// void bvh_insert(bvh_node* root, bvh_node* to_insert)
// {
//     uint8_t node_types[2] = {bvh_node_type(root), bvh_node_type(to_insert)};
//     if(node_types[0] == BVH_NULL || node_types[1] == BVH_NULL) return;

//     if(node_types[0] == BVH_EMPTY && node_types[1] != BVH_EMPTY)
//     {
//         *root = *to_insert;
//         return;
//     }

//     if(node_types[0] == BVH_LEAF)
//     {
//         //split in two and place old and new together
//         bvh_node* split = (bvh_node*)calloc(1, sizeof(bvh_node));
//         split->children[0] = root;
//         split->children[1] = to_insert;

//         //perform nepotism/eugenics/whatever
//         bvh_node* p = root->parent;
//         if(p)
//         {
//             uint8_t is_root = p->children[1] == root;
//             p->children[is_root] = split;

//             split->parent = p;
//         }
//         root->parent = split;
        
//         //recalculate volume
//         bvh_upward_aabb_update(split);

//         return;
//     }

//     //keep travelling down the root
//     real growth[2] = {fl2real(0), fl2real(0)};
//     uint8_t i = 0;
//     for(; i < 2; i++)
//     {
//         real old_volume =   m_mul(  root->children[i]->half_size.x,
//                             m_mul(  root->children[i]->half_size.y,
//                                     root->children[i]->half_size.z));

//         bvh_node new_aabb = bvh_aabb_for_pair(root->children[i], to_insert, AABB_SIZE_CALCULATION);
//         real new_volume =   m_mul(  new_aabb.half_size.x,
//                             m_mul(  new_aabb.half_size.y,
//                                     new_aabb.half_size.z));
//         growth[i] = new_volume - old_volume;
//     }
    
//     uint8_t smaller_growth = growth[1] < growth[0];
//     bvh_insert(root->children[smaller_growth], to_insert);
// }

void bvh_delete_node(bvh_node* a)
{
    if(a == NULL) return;

    //handle degenerate parent
    bvh_node* p = a->parent;
    if(p)
    {
        uint8_t is_b = p->children[1] != a;
        bvh_node* b = p->children[is_b];
        if(b)
            *p = *b;
        else
            p->half_size = (vec3){0};

        bvh_upward_aabb_update(p->parent);
    }

    //cleanup
    uint8_t i = 0;
    for(; i < 2; i++)
    {
        a->children[i]->parent = NULL;
        bvh_delete_node(a->children[i]);
    }

    free(a);
}
#endif
