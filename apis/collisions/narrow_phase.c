//Dependencies
#include "contacts.c"

#ifndef _NARROW_PHASE_H
    #define _NARROW_PHASE_H

void collision_detect(collision_pair* p);
#endif

#if defined(INCLUDE_IMPLEMENTATION) && !defined(_NARROW_PHASE_C)
    #define _NARROW_PHASE_C

#include "narrow_phase_helper.c"

void sphere_to_sphere(collision_pair* p)
{
    printf("p->members[0]: %p\n", p->members[0]);
    printf("p->members[1]: %p\n", p->members[1]);
    collision_volume* a;
    a = p->members[0];
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
    update_potential_contact(   p,
                                vec_add(*a->position, vec_scalar_multiply(normal, a->radius - m_div(overlap.x, 2))),
                                normal,
                                m_div(overlap.x, 2));

    VALIDATE_POTENTIAL_CONTACT(p)
}

void sphere_to_box(collision_pair* p)
{
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

    vec3 normal = vec_subtract(closest, relative);
    real overlap = sphere->radius - vec_length(normal);
    if(overlap < fl2real(0))
    {
        p->type = NO_COLLISION;
        return;
    }

    update_potential_contact(   p,
                                vec_add(vec_rotate_w_rotor(closest, rot_b), pos_b),
                                vec_rotate_w_rotor(vec_normalize(normal), rot_b),
                                overlap);

    VALIDATE_POTENTIAL_CONTACT(p)
}

// void box_to_box(collision_pair* p)
// {
//     collision_volume* a = p->members[0];
//     collision_volume* b = p->members[1];

//     real overlap;

//     vec3 center_diff = vec_subtract(b->position, a->position); //a to b

//     uint8_t i = 0;
//     for(; i < 3; i++)
//     {
//         vec3 a_axis = (vec3){0,0,0};
//         (real*)(&a_axis)[i] = fl2real(1);
//         a_axis = vec_rotate_w_rotor(a_axis, *a->orientation);

//         if(overlap = separating_axis_test_iteration(p, a_axis) < 0) return;
//         update_candidate_contact_members(p, overlap, FACE, i, VERTEX, -1);

//         vec3 b_axis = (vec3){0,0,0};
//         uint8_t j = i;
//         for(; j < i+3; j++)
//         {
//             (real*)(&b_axis)[j%3] = fl2real(1);
//             b_axis = vec_rotate_w_rotor(b_axis, *b->orientation);

//             if(!vec_are_parallel(a_axis, b_axis, fl2real(0.1))) //avoid if edges are close to parallel, comment if unstable
//             if(overlap = separating_axis_test_iteration(p, vec_cross_product(a_axis, b_axis)) < 0) return;
//             update_candidate_contact_members(p, overlap, EDGE, i, EDGE, j);
//         }
        
//         if(overlap = separating_axis_test_iteration(a,b, b_axis) < 0) return;
//         update_candidate_contact_members(p, overlap, VERTEX, -1, FACE, j-1);
//     }

//     //derive data from candidate
//     vec3 contact_point;
//     vec3 contact_normal;

//     update_potential_contact(   p,
//                                 contact_point,
//                                 contact_normal,
//                                 best_overlap);

//     VALIDATE_POTENTIAL_CONTACT(p)
// }

#define DESIRED_TYPE vec3
#include "../data_structures/linked_list/api.c"

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

    vec3_ll_node* axes = NULL;
    axes = ll_add_vec3_data(axes, NEXT, a_axes[0]);
    axes = ll_add_vec3_data(axes, NEXT, a_axes[1]);
    axes = ll_add_vec3_data(axes, NEXT, a_axes[2]);
    axes = ll_add_vec3_data(axes, NEXT, b_axes[0]);
    axes = ll_add_vec3_data(axes, NEXT, b_axes[1]);
    axes = ll_add_vec3_data(axes, NEXT, b_axes[2]);

    uint8_t i,j;
    for(i = 0; i < 3; i++)
    for(j = 0; j < 3; j++)
    {
        if(m_abs(vec_dot_product(a_axes[i], b_axes[j])) < 1)
        {
            axes = ll_add_vec3_data(axes, NEXT, vec_cross_product(a_axes[i], b_axes[j]));
        }
    }

    real least_overlap = REAL_MAX;
    vec3 best_axis = (vec3){0,0,0};
    uint8_t best_axis_index = 255;
    uint8_t axis_index = 0;
    
    vec3_ll_node* axis = axes;
    for(; axis; axis = axis->NEXT)
    {
        vec3 separating_axis = axis->data;
        real center_a = vec_dot_product(pos_a, separating_axis);
        real center_b = vec_dot_product(pos_b, separating_axis);

        real half_size_a = m_abs(vec_dot_product(vec_rotate_w_rotor(a->half_size, rotor_a), separating_axis));
        real half_size_b = m_abs(vec_dot_product(vec_rotate_w_rotor(b->half_size, rotor_b), separating_axis));

        vec3 overlap_on_axis = halfsize_separating_axis_test(center_a, half_size_a, center_b, half_size_b);
        
        if(overlap_on_axis.x < fl2real(0))
        {
            p->type = NO_COLLISION;
            free_vec3_ll_chain(axes, PREV);
            return;
        }

        if(overlap_on_axis.x < least_overlap)
        {
            least_overlap = overlap_on_axis.x;
            best_axis = separating_axis;
            best_axis_index = axis_index;
        }
        
        axis_index++;
    }
    
    if(best_axis_index == 255) return;
    
    //determine collision type and calculate point and normal
    vec3 contact_point = (vec3){0,0,0};
    vec3 contact_normal = (vec3){1,0,0};
    if(best_axis_index < 6)
    {
        collision_volume* other = a;
        vec3 center_delta = vec_subtract(pos_a, pos_b);

        //box 1 or box 2?
        if(best_axis_index < 3)
        {
            other = b;
            center_delta = vec_reverse(center_delta);
        }

        //which of the two faces?
        vec3 normal = best_axis;
        if(vec_dot_product(normal, center_delta) > 0)
        {
            normal = vec_reverse(normal);
        }

        contact_normal = normal;

        //local coordinates contact point
        contact_point = other->half_size;
        normal = vec_rotate_w_rotor(normal, rotor_reverse(*other->orientation));
        if(m_mul(contact_point.x, normal.x) < 0) contact_point.x *= -1;
        if(m_mul(contact_point.y, normal.y) < 0) contact_point.y *= -1;
        if(m_mul(contact_point.z, normal.z) < 0) contact_point.z *= -1;

        //transform to world coordinates
        contact_point = vec_add(vec_rotate_w_rotor(contact_point, *other->orientation), *other->position);
    }
    else
    {
        //edge-edge
        contact_normal = best_axis;
    }
    
    free_vec3_ll_chain(axes, PREV);
    
    update_potential_contact(   p,
                                contact_point,
                                contact_normal,
                                least_overlap);

    VALIDATE_POTENTIAL_CONTACT(p)
}

//TODO implement sphere to mesh
void sphere_to_mesh(collision_pair* p)
{
    collision_volume* a = p->members[0];
    collision_volume* b = p->members[1];

    vec3 center_diff = vec_subtract(*a->position, *b->position);
    center_diff = vec_rotate_w_rotor(center_diff, rotor_reverse(*b->orientation));
    vec3 direction = vec_normalize(center_diff);

    //find closest triangle
    real sameness = 0;
    uint32_t closest_t = -1;
    uint32_t t = 0;
    for(; t < b->mesh.face_count; t++)
    {
        real likeness = vec_dot_product(COLLISION_GET_TRI_DATA(b, t, COLLISION_TRI_NORMAL), direction);

        if(likeness > sameness)
        {
            sameness = likeness;
            closest_t = t;
        }
    }

    real to_closest_point = vec_dot_product(vec_subtract(center_diff, COLLISION_GET_TRI_DATA(b,closest_t, COLLISION_TRI_ORIGIN)), direction);

    //early out
    if(to_closest_point > a->radius)
    {
        p->type = NO_COLLISION;
        return;
    }

    vec3 contact_point = vec_subtract(center_diff, vec_scalar_multiply(vec_reverse(direction), to_closest_point));

    contact_point = vec_add(vec_rotate_w_rotor(contact_point, *b->orientation), *b->position);

    update_potential_contact(   p,
                                contact_point,
                                vec_normalize(vec_subtract(contact_point, *a->position)),
                                a->radius - to_closest_point);

    VALIDATE_POTENTIAL_CONTACT(p)
}

//TODO implement box to mesh
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
    //vertex-face
    uint8_t current_volume = 0;
    for(; current_volume < 2; current_volume++)
    {
        collision_volume* a = p->members[current_volume];
        collision_volume* b = p->members[1 - current_volume];

        uint32_t t_a = 0;
        for(; t_a < a->mesh.face_count; t_a++)
        {
            vec3 axis = COLLISION_GET_TRI_DATA(a, t_a, COLLISION_TRI_NORMAL);
            axis = vec_rotate_w_rotor(axis, *a->orientation);

            uint32_t t_b = 0;
            for(; t_b < b->mesh.face_count; t_b++)
            {
                vec3 origin = COLLISION_GET_TRI_DATA(b, t_b, COLLISION_TRI_ORIGIN);
                
                vec3 edge = (vec3){0};
                uint8_t e_b = 0;
                while(e_b < 3)
                {
                    {
                        vec3 vertex = vec_add(origin, edge);
                        vertex = vec_rotate_w_rotor(vertex, *b->orientation);
                        
                        vec3 offset = vec_rotate_w_rotor(COLLISION_GET_TRI_DATA(a, t_a, COLLISION_TRI_ORIGIN), *a->orientation);
                        offset = vec_scalar_multiply(axis, vec_dot_product(offset, axis));

                        update_potential_contact(p, vertex, axis, vertex_face_overlap(vertex, axis, offset));
                    }
                    //
                    edge = COLLISION_GET_TRI_DATA(b, t_b, COLLISION_TRI_EDGE_1 + e_b);
                    e_b++;
                }
            }
        }
    }

    //edge-edge
    collision_volume* a = p->members[0];
    collision_volume* b = p->members[1];
    uint32_t t_a = 0;
    for(; t_a < a->mesh.face_count; t_a++)
    {
        vec3 edge_a = vec_cross_product(COLLISION_GET_TRI_DATA(a, t_a, COLLISION_TRI_EDGE_1),
                                        COLLISION_GET_TRI_DATA(a, t_a, COLLISION_TRI_EDGE_2));
        uint8_t e_a = 0;
        while(e_a < 3)
        {
            edge_a = vec_rotate_w_rotor(edge_a, *a->orientation);

            uint32_t t_b = 0;
            for(; t_b < b->mesh.face_count; t_b++)
            {
                vec3 edge_b = vec_cross_product(COLLISION_GET_TRI_DATA(b, t_b, COLLISION_TRI_EDGE_1),
                                                COLLISION_GET_TRI_DATA(b, t_b, COLLISION_TRI_EDGE_2));
                uint8_t e_b = 0;
                while(e_b < 3)
                {
                    edge_b = vec_rotate_w_rotor(edge_b, *b->orientation);

                    {
                        vec3 axis = vec_cross_product(edge_a, edge_b);

                        rotor3 result = edge_edge_overlap(  vec_subtract(*b->position, *a->position),
                                                            vec_rotate_w_rotor(COLLISION_GET_TRI_DATA(a, t_a, COLLISION_TRI_ORIGIN), *a->orientation),
                                                            edge_a,
                                                            vec_rotate_w_rotor(COLLISION_GET_TRI_DATA(a, t_a, COLLISION_TRI_ORIGIN), *b->orientation),
                                                            edge_b);

                        update_potential_contact(p, result.T, axis, result.II);
                    }

                    //
                    edge_b = COLLISION_GET_TRI_DATA(b, t_b, COLLISION_TRI_EDGE_1 + e_b);
                    e_b++;
                }
            }

            //
            edge_a = COLLISION_GET_TRI_DATA(a, t_a, COLLISION_TRI_EDGE_1 + e_a);
            e_a++;
        }
    }

    VALIDATE_POTENTIAL_CONTACT(p)
}

void SWAP_AND_CALL(collision_pair* p);

//TODO consider getting rid of this and replacing it with a switch statement or something
void (*narrow_funcs[])(collision_pair*) = 
{               /*SPHERE        , BOX           , MESH*/
    /*SPHERE*/  sphere_to_sphere, sphere_to_box , sphere_to_mesh  ,
    /*BOX*/     SWAP_AND_CALL   , box_to_box    , box_to_mesh     ,
    /*MESH*/    SWAP_AND_CALL   , SWAP_AND_CALL , mesh_to_mesh
};

#define CALL_NARROW_FUNC(p) \
{uint8_t index = p->members[0]->type*NO_OF_VOLUME_TYPES + p->members[1]->type;\
/*if(narrow_funcs[index])*/ narrow_funcs[index](p);}

void collision_detect(collision_pair* p)
{
    collision_volume* a = p->members[0];
    collision_volume* b = p->members[1];

    reset_contact_data(p);

    if(a->type != GROUP && b->type != GROUP)
        CALL_NARROW_FUNC(p)

    //handle groups
    //FIXME wont work if b is GROUP and a is not
    // if(a->type == GROUP)
    // {
    //     ITERATE_LIST_START(a->volumes, volume_a)
    //         p->members[0] = volume_a;
    //         if(b->type == GROUP)
    //         {
    //             ITERATE_LIST_START(b->volumes, volume_b)
    //                 p->members[1] = volume_b;
    //                 CALL_NARROW_FUNC(p)
    //             ITERATE_LIST_END(NEXT, volume_b)
    //         }
    //         else
    //             CALL_NARROW_FUNC(p);
    //     ITERATE_LIST_END(NEXT, volume_a)
    // }
}

void SWAP_AND_CALL(collision_pair* p)
{
    collision_volume* temp = p->members[0];
    p->members[0] = p->members[1];
    p->members[1] = temp;

    CALL_NARROW_FUNC(p)
}
#undef CALL_NARROW_FUNC
#endif