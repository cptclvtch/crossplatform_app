vec3 minmax_separating_axis_test(real min_a, real max_a, real min_b, real max_b)
{
    real min = (min_a > min_b)? min_a : min_b;
    real max = (max_a < max_b)? max_a : max_b;

    return (vec3){max-min, min, max};
}
vec3 halfsize_separating_axis_test(real center_a, real half_a, real center_b, real half_b)
{
    return minmax_separating_axis_test(center_a - half_a, center_a + half_a, center_b - half_b, center_b + half_b);
}

//TODO implement defferred axis tests later
// linked_list_node* prune_away_direction(linked_list_node* axes, vec3 dir)
// {
    
// }

// linked_list_node* prune_away_sameness(linked_list_node* axes)
// {
//     ITERATE_LIST_START(axes, axis_a)
//         if(m_abs(vec_dot_product(axis_a, axis_b)) < 1)
//         {

//         }
//     ITERATE_LIST_END(NEXT, axis_a)
// }

// linked_list_node* pick_test_axes(collision_volume* a, collision_volume* b)
// {
//     if(a->type != MESH || b->type != MESH) return NULL;

//     vec3 diff = vec_subtract(b->position, a->position);

//     linked_list_node* axes = NULL;

//     //only do faces facing the other volume

//     //face combinations
    
//     prune_away_direction(axes, diff);

//     //edge combinations

//     //prune parallels

//     return axes;
// }

FORCE_INLINE real vertex_face_overlap(vec3 point, vec3 face_normal, vec3 face_offset)
{
    return vec_dot_product(vec_subtract(point, face_offset), vec_reverse(face_normal));
}

rotor3 edge_edge_overlap(vec3 center_diff, vec3 a_origin, vec3 a_edge, vec3 b_origin, vec3 b_edge)
{
    vec3 axis = vec_cross_product(a_edge, b_edge);

    //convert b_origin to a's coordinates
    b_origin = vec_add(center_diff, b_origin);

    vec3 midpoint = vec_scalar_divide(  vec_add(vec_add(a_origin, vec_scalar_divide(a_edge, 2)),
                                                vec_add(b_origin, vec_scalar_divide(b_edge, 2))), 2);

    real overlap = vec_dot_product(a_origin, axis) - vec_dot_product(b_origin, axis);

    return (rotor3){.II = overlap,
                    .T  = midpoint};
}

// vec3 get_dimensions_on_axis(collision_volume* v, vec3 a)
// {
//     //sanitize?
//     //a = vec_normalize(a);

//     real center = vec_dot_product(*v->position, a);

//     real min = REAL_MAX;
//     real max = REAL_MIN;
//     switch(v->type)
//     {

//         case SPHERE:
//         {
//             min = center - v->radius;
//             max = center + v->radius;
//             break;
//         }

//         case BOX:
//         {
//             real extent = vec_dot_product(v->half_size, vec_rotate_w_rotor(a, rotor_reverse(*v->orientation)));
//             min = center - extent;
//             max = center + extent;
//             break;
//         }

//         case MESH:
//         {
//             uint32_t i = 0;
//             for(; i < v->mesh.face_count; i++)
//             {
//                 real projection = vec_dot_product(v->mesh.data[i], a);
//                 if(projection < min) min = projection;
//                 if(projection > max) max = projection;
//             }
//             min += center;
//             max += center;
//             break;
//         }
//     }

//     return (vec3){max-min, min, max};
// }

// real separating_axis_test_iteration(collision_volume* a, collision_volume* b, vec3 axis)
// {
//     vec3 min_max_a = get_dimensions_on_axis(a, axis);
//     vec3 min_max_b = get_dimensions_on_axis(b, axis);

//     vec3 overlap = minmax_separating_axis_test(min_max_a.y, min_max_a.z, min_max_b.y, min_max_b.z);
    
//     return overlap.x;
// }

void reset_contact_data(collision_pair* p)
{
    if(p == NULL) return;

    p->contact_count = MAX_CONTACT_POINTS;
    do
    {
        p->contact_count--;
        p->points[p->contact_count].penetration = REAL_MAX;
    }
    while(p->contact_count > 0);
}

void update_potential_contact(collision_pair* p, vec3 contact_point, vec3 contact_normal, real overlap)
{
    contact* potential_contact = &p->points[p->contact_count];

    if(overlap > potential_contact->penetration) return;

    potential_contact->point = contact_point;
    potential_contact->normal = contact_normal;
    potential_contact->penetration = overlap;
}

#define VALIDATE_POTENTIAL_CONTACT(p) \
if((p->points[p->contact_count]).penetration >= 0)\
{p->contact_count++; p->type = CONFIRMED_COLLISION;}