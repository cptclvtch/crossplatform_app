/*Contact generation priority
- vertex-face, edge-edge(non-parallel)
- edge-face, face-face
- vertex-edge, vertex-vertex, edge-edge(parallel)
*/
//Dependencies
#include "contacts.c"

#ifdef INCLUDE_IMPLEMENTATION

#include "narrow_phase_helper.c"

void sphere_to_sphere(collision_pair* p)
{
    collision_volume* a = p->members[0];
    collision_volume* b = p->members[1];

    vec3 dir = vec_subtract(*b->position, *a->position); //a to b
    real gap = vec_length(dir);

    vec3 overlap = halfsize_separating_axis_test(0, a->radius, gap, b->radius);

    if(overlap.x < fl2real(0))
    {
        p->type = NO_COLLISION;
        return;
    }
    
    vec3 normal = vec_scalar_divide(dir, gap);
    p->points[p->contact_count] =   (contact)
                                    {
                                        .point = vec_add(*a->position, vec_scalar_multiply(normal, a->radius - m_div(overlap.x, 2))),
                                        .normal = normal,
                                        .penetration = overlap.x
                                    };
    p->type = CONFIRMED_COLLISION;
    p->contact_count++;
}

void sphere_to_box(collision_pair* p)
{
    //TODO convert to separating axis test?
    collision_volume* sphere = p->members[0];
    vec3 pos_a = *sphere->position;

    collision_volume* box = p->members[1];
    vec3 pos_b = *box->position;
    rotor3 rot_b = *box->orientation;

    //find sphere position in box space
    vec3 relative = vec_rotate_w_rotor(vec_subtract(pos_a, pos_b), rotor_reverse(rot_b));

    //early out
    if( relative.x > box->half_size.x + sphere->radius ||
        relative.y > box->half_size.y + sphere->radius ||
        relative.z > box->half_size.z + sphere->radius)
    {
        p->type = NO_COLLISION;
        return;
    }

    //find closest point
    vec3 closest = vec_max(relative, vec_reverse(box->half_size));
    closest = vec_min(closest, box->half_size);

    vec3 normal = vec_subtract(relative, closest);
    real overlap = sphere->radius - vec_length(normal);
    if(overlap < fl2real(0))
    {
        p->type = NO_COLLISION;
        return;
    }

    p->points[p->contact_count] =   (contact)
                                    {
                                        .point = vec_add(vec_rotate_w_rotor(closest, rot_b), pos_b),
                                        .normal = vec_rotate_w_rotor(vec_normalize(normal), rot_b),
                                        .penetration = overlap
                                    };
    p->type = CONFIRMED_COLLISION;
    p->contact_count++;
}

void box_to_box(collision_pair* p)
{
    collision_volume* a = p->members[0];
    collision_volume* b = p->members[1];

    vec3 pos_a = *a->position;
    vec3 pos_b = *b->position;
    rotor3 rotor_a = *a->orientation;
    rotor3 rotor_b = *b->orientation;

    vec3 a_axes[3] = 
    {
        vec_rotate_w_rotor((vec3){1,0,0}, rotor_a),
        vec_rotate_w_rotor((vec3){0,1,0}, rotor_a),
        (vec3){0,0,0}
    };
    a_axes[2] = vec_cross_product(a_axes[0], a_axes[1]);

    vec3 b_axes[3] = 
    {
        vec_rotate_w_rotor((vec3){1,0,0}, rotor_b),
        vec_rotate_w_rotor((vec3){0,1,0}, rotor_b),
        (vec3){0,0,0}
    };
    b_axes[2] = vec_cross_product(b_axes[0], b_axes[1]);

    linked_list_node* axes = NULL;
    axes = add_link_before(axes, calloc(1,sizeof(vec3)));
    *(vec3*)axes->data = a_axes[0];
    axes = add_link_before(axes, calloc(1,sizeof(vec3)));
    *(vec3*)axes->data = a_axes[1];
    axes = add_link_before(axes, calloc(1,sizeof(vec3)));
    *(vec3*)axes->data = a_axes[2];
    axes = add_link_before(axes, calloc(1,sizeof(vec3)));
    *(vec3*)axes->data = b_axes[0];
    axes = add_link_before(axes, calloc(1,sizeof(vec3)));
    *(vec3*)axes->data = b_axes[1];
    axes = add_link_before(axes, calloc(1,sizeof(vec3)));
    *(vec3*)axes->data = b_axes[2];

    uint8_t i,j;
    for(i = 0; i < 3; i++)
    for(j = 0; j < 3; j++)
    {
        axes = add_link_before(axes, calloc(1, sizeof(vec3)));
        *(vec3*)axes->data = vec_cross_product(a_axes[i], b_axes[j]);
    }

    real least_overlap = REAL_MIN;
    vec3* best_axis = NULL;
    ITERATE_LIST_START(axes, axis)
        real center_a = vec_dot_product(pos_a, *(vec3*)axis);
        real center_b = vec_dot_product(pos_b, *(vec3*)axis);

        real half_size_a = vec_dot_product(vec_rotate_w_rotor(a->half_size, rotor_a), *(vec3*)axis);
        real half_size_b = vec_dot_product(vec_rotate_w_rotor(b->half_size, rotor_b), *(vec3*)axis);

        vec3 overlap_on_axis = halfsize_separating_axis_test(center_a, half_size_a, center_b, half_size_b);
        if(overlap_on_axis.x < least_overlap)
        {
            least_overlap = overlap_on_axis.x;
            best_axis = (vec3*)axis;
        }

        if(least_overlap < fl2real(0))
        {
            p->type = NO_COLLISION;
            delete_link_chain(axes, NULL, 1);
            return;
        }
    ITERATE_LIST_END(NEXT, axis)
    
    //TODO reread p324-326

    p->points[p->contact_count] =   (contact)
                                    {
                                        // .point = vec_scalar_divide(vec_add(closest, pos_a), fl2real(2.0)),
                                        .normal = *best_axis,
                                        .penetration = least_overlap
                                    };
    p->type = CONFIRMED_COLLISION;
    p->contact_count++;

    delete_list(axes);
}

//TODO sphere to mesh
void sphere_to_mesh(collision_pair* p)
{
    collision_volume* a = p->members[0];
    collision_volume* b = p->members[1];

    linked_list_node* axes = pick_test_axes(b,b);

    real least_overlap = REAL_MIN;
    ITERATE_LIST_START(axes, axis)
        real min = REAL_MAX;
        real max = REAL_MIN;
        uint32_t i = 0;
        for(; i < b->mesh.vertex_count; i++)
        {
            real projection = vec_dot_product(b->mesh.vertices[i], *(vec3*)axis);
            if(projection > max) max = projection;
            if(projection < min) min = projection;
        }

        real sphere_pos = vec_dot_product(*a->position, *(vec3*)axis);
        vec3 overlap_on_axis = minmax_separating_axis_test(sphere_pos - a->radius, sphere_pos + a->radius, min, max);
        if(overlap_on_axis.x < least_overlap)
        {
            least_overlap = overlap_on_axis.x;
        }

        if(least_overlap < fl2real(0))
        {
            p->type = NO_COLLISION;
            return;
        }
    ITERATE_LIST_END(NEXT, axis)

    //at most one collision point
    //find closest point on mesh and derive the data from it

    p->points[p->contact_count] =   (contact)
                                    {   //FIXME
                                        // .point = vec_scalar_divide(vec_add(closest, pos_a), fl2real(2.0)),
                                        // .normal = vec_normalize(relative),
                                        .penetration = least_overlap
                                    };
    p->type = CONFIRMED_COLLISION;
    p->contact_count++;
}

//TODO box to mesh
void box_to_mesh(collision_pair* p)
{
    collision_volume* a = p->members[0];
    collision_volume* b = p->members[1];

    //find contact data same way as mesh-mesh

    p->points[p->contact_count] =   (contact)
                                    {   //FIXME
                                        // .point = vec_scalar_divide(vec_add(closest, pos_a), fl2real(2.0)),
                                        // .normal = vec_normalize(relative),
                                        // .penetration = overlap
                                    };
    p->type = CONFIRMED_COLLISION;
    p->contact_count++;
}

//TODO add support for aabb helper optimization
//TODO research how bvhs and aabbs can help with multicontact mesh-mesh implementation (p327 in book)
void mesh_to_mesh(collision_pair* p)
{
    collision_volume* a = p->members[0];
    collision_volume* b = p->members[1];

    linked_list_node* axes = pick_test_axes(a,b);
    if(axes == NULL) return;

    real least_overlap = REAL_MIN;
    ITERATE_LIST_START(axes, axis)
        uint32_t i;

        real min_a = REAL_MAX;
        real max_a = REAL_MIN;
        for(i = 0; i < a->mesh.vertex_count; i++)
        {
            real coord = vec_dot_product(a->mesh.vertices[i], *(vec3*)axis);
            if(max_a < coord) max_a = coord;
            if(min_a > coord) min_a = coord;
        }
        
        real min_b = REAL_MAX;
        real max_b = REAL_MIN;
        for(i = 0; i < b->mesh.vertex_count; i++)
        {
            real coord = vec_dot_product(b->mesh.vertices[i], *(vec3*)axis);
            if(max_b < coord) max_b = coord;
            if(min_b > coord) min_b = coord;
        }

        vec3 overlap_on_axis = minmax_separating_axis_test(min_a, max_a, min_b, max_b);
        if(overlap_on_axis.x < least_overlap)
        {
            least_overlap = overlap_on_axis.x;
        }
        
        if(least_overlap < fl2real(0))
        {
            p->type = NO_COLLISION;
            delete_list(axes);
            return;
        }
    ITERATE_LIST_END(NEXT, axis)

    //if vertex-face
    // find vertex with greatest penetration
    //if edge-edge
    // find points of closest approach of the two edges
    
    p->points[p->contact_count] =   (contact)
                                    {   //FIXME
                                        // .point = vec_scalar_divide(vec_add(closest, pos_a), fl2real(2.0)),
                                        // .normal = vec_normalize(relative),
                                        .penetration = least_overlap
                                    };
    p->type = CONFIRMED_COLLISION;
    p->contact_count++;

    delete_list(axes);
}

void SWAP_AND_CALL(collision_pair* p);

void (*narrow_funcs[])(collision_pair*) = 
{               /*SPHERE        , BOX           , MESH*/
    /*SPHERE*/  sphere_to_sphere, sphere_to_box , sphere_to_mesh  ,
    /*BOX*/     SWAP_AND_CALL   , box_to_box    , box_to_mesh     ,
    /*MESH*/    SWAP_AND_CALL   , SWAP_AND_CALL , mesh_to_mesh
};

#define CALL_NARROW_FUNC(p) \
{uint8_t index = p->members[0]->type + p->members[1]->type*NO_OF_VOLUME_TYPES;\
if(narrow_funcs[index]) narrow_funcs[index](p);}

//TODO test this
void collision_detect(collision_pair* p)
{

    collision_volume* a = p->members[0];
    collision_volume* b = p->members[1];

    if(a->type != GROUP && b->type != GROUP)
        CALL_NARROW_FUNC(p)

    //handle groups
    //FIXME wont work if b is GROUP and a is not
    if(a->type == GROUP)
    {
        ITERATE_LIST_START(a->volumes, volume_a)
            p->members[0] = volume_a;
            if(b->type == GROUP)
            {
                ITERATE_LIST_START(b->volumes, volume_b)
                    p->members[1] = volume_b;
                    CALL_NARROW_FUNC(p)
                ITERATE_LIST_END(NEXT, volume_b)
            }
            else
                CALL_NARROW_FUNC(p);
        ITERATE_LIST_END(NEXT, volume_a)
    }
}

void SWAP_AND_CALL(collision_pair* p)
{
    collision_volume* temp = p->members[0];
    p->members[0] = p->members[1];
    p->members[1] = temp;

    CALL_NARROW_FUNC(p)

    // flip contact normals to account for swap?
}
#undef CALL_NARROW_FUNC
#endif