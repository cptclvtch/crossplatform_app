/*Contact generation priority
- vertex-face, edge-edge(non-parallel)
- edge-face, face-face
- vertex-edge, vertex-vertex, edge-edge(parallel)
*/

#ifdef INCLUDE_IMPLEMENTATION

#include "narrow_phase_helper.c"

void sphere_to_sphere(collision_pair* p)
{
    collision_volume* sphere_a = p->members[0];
    collision_volume* sphere_b = p->members[1];
    vec3 pos_a = *sphere_a->position;
    vec3 pos_b = *sphere_b->position;

    vec3 mid = vec_subtract(pos_b, pos_a);
    real gap = vec_length(mid);
    real overlap = sphere_a->radius + sphere_b->radius - gap;

    if(overlap < fl2real(0))
    {
        p->type = NO_COLLISION;
        return;
    }
    
    p->points[p->contact_count] =   (contact)
                                    {
                                        .point = vec_scalar_divide(vec_add(pos_a, pos_b), fl2real(2.0)),
                                        .normal = vec_scalar_divide(mid, gap),
                                        .penetration = overlap
                                    };
    p->type = CONFIRMED_COLLISION;
    p->contact_count++;
}

void sphere_to_box(collision_pair* p)
{
    collision_volume* sphere = p->members[0];
    vec3 pos_a = sphere->position;

    collision_volume* box = p->members[1];
    vec3 pos_b = box->position;
    rotor3 rot_b = box->orientation;

    //find sphere position in box space
    vec3 relative = vec_rotate_w_rotor(vec_subtract(pos_a, pos_b), rotor_reverse(rot_b));
    vec3 half_size = vec_scalar_divide(box->dimensions, 2);

    //early out
    if( relative.x > half_size.x + sphere->radius ||
        relative.y > half_size.y + sphere->radius ||
        relative.z > half_size.z + sphere->radius)
    {
        p->type = NO_COLLISION;
        return;
    }

    //find closest point
    vec3 closest = vec_max(relative, vec_reverse(half_size));
    closest = vec_min(closest, half_size);

    real overlap = vec_length(vec_subtract(pos_a,relative)) - sphere.radius;
    if(overlap < fl2real(0))
    {
        p->type = NO_COLLISION;
        return;
    }

    //convert closest to world coords
    closest = closest;//FIXME

    p->points[p->contact_count] =   (contact)
                                    {
                                        .point = vec_scalar_divide(vec_add(closest, pos_a), fl2real(2.0)),
                                        .normal = vec_normalize(relative),
                                        .penetration = overlap
                                    };
    p->type = CONFIRMED_COLLISION;
    p->contact_count++;
}

void box_to_sphere(collision_pair* p)
{
    collision_volume* temp = p->members[0];
    p->members[0] = p->members[1];
    p->members[1] = temp;

    sphere_to_box(p);
}

void box_to_box(collision_pair* p)
{
    //TODO get positions and rotations
    vec3 pos_a,pos_b;
    rotor3 rotor_a, rotor_b;

}

void mesh_to_mesh(collision_pair* p)
{
    //TODO get positions and rotations
    vec3 pos_a,pos_b;
    rotor3 rotor_a, rotor_b;


}

//FIXME
// void (*narrow_funcs)(collision_pair*)[] = 
// {               /*SPHERE        , BOX           , MESH*/
//     /*SPHERE*/  sphere_to_sphere, box_to_sphere , NULL  ,
//     /*BOX*/     sphere_to_box   , box_to_box    , NULL  ,
//     /*MESH*/    NULL            , NULL          , mesh_to_mesh
// };

// void collision_detect(collision_pair* p)
// {
//     //TODO handle groups here? most definitely
//     uint8_t index = p->members[0]->type + p->members[1]->type*NO_OF_VOLUME_TYPES;
//     if(narrow_funcs[index])
//         narrow_funcs[index](p);
// }
#endif