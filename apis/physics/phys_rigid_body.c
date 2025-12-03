#ifndef INCLUDE_IMPLEMENTATION
typedef struct
{
    //Representation
        collision_volume representation;
        real volume; //m^3
        // real surface_area; //m^2

    //Dynamics
        real restitution;

        //linear part
        phys_point* center_of_mass;

        //angular part
        rotor3* orientation;
        vec3 angular_velocity; //scaled axis representation
        vec3 inertia_tensor; //TODO figure out units for inertia
        vec3 inverse_inertia_tensor;

        //d'alembert cache
        vec3 torque_accumulator; //N * m
}phys_rigid_body;

//----------------------------------
#else
//----------------------------------

void calculate_center_of_mass(phys_rigid_body* b)
{
    // vec3 center_of_mass = (vec3){0,0,0};
    // // iterate over grid cells of bounding box
    // if(point_2_mesh_collision(point, mesh, NULL))
    // {
    //     center_of_mass += cell center
    //     no_of_cells++
    // }
    // //check for primitive collisions as well
    
    // return vec_scalar_divide(center_of_mass, no_of_cells);
}

//TODO consider transformations of frames of reference for inertia tensor
//TODO proper angular acceleration formula
void angular_integration(phys_rigid_body* b, float dT/*seconds*/)
{
    //FIXME zero length rotor?
    real angular_damping = fl2real(0.99);

    vec3 angular_acceleration; //scaled axis representation
    angular_acceleration = b->torque_accumulator;
    // angular_acceleration = vec_multiply(b->inverse_inertia_tensor, b->torque_accumulator);

    b->angular_velocity = vec_add(b->angular_velocity, vec_scalar_multiply(angular_acceleration, fl2real(dT)));

    b->angular_velocity = vec_scalar_multiply(b->angular_velocity, angular_damping);

    *(b->orientation) = rotor_combine(  *(b->orientation),
                                        rotor_from_scaled_axis_angle(vec_scalar_multiply(b->angular_velocity, m_div(fl2real(dT),2))));

    //calculate derived data
    *(b->orientation) = rotor_normalize(*(b->orientation));
    //calculate transform matrix
    //convert local inertia tensor to world inverse inverse tensor

    //clear d'alembert caches
    b->torque_accumulator = (vec3){0};
}

// void calculate_moment_of_inertia_around_axis(phys_rigid_body* body, vec3 axis, real sample_size)
// {
//     //TODO use octree implementation?
//     if(!body) return;
//     if(body->mesh_representation.vertex_count == 0) return;
//     if(sample_size < M_EPSILON) return;

//     body->inertia = fl2real(0.0);
//     axis = vec_normalize(axis);

//     rotor3 rotation = rotor_from_vectors(axis, (vec3){1,0,0});
//     // if mesh is not already centered around center of mass, find center of mass

//     // make translated and rotated copy of mesh
//     collision_mesh mesh_copy;
//     mesh_copy.vertex_count = body->mesh_representation.vertex_count;
//     mesh_copy.vertices = (vec3*)calloc(mesh_copy.vertex_count, sizeof(vec3));
    
//     vec3 min = (vec3){0,0,0};
//     vec3 max = min;

//     mesh_index_t i = 0;
//     for(;i < mesh_copy.vertex_count; i++)
//     {
//         vec3 p = body->mesh_representation.vertices[i];
//         //p = vec_add(p, center_of_mass);
//         mesh_copy.vertices[i] = vec_rotate_w_rotor(p, rotation);

//         min = vec_min(min, mesh_copy.vertices[i]);
//         max = vec_max(max, mesh_copy.vertices[i]);
//     }

//     real dm = m_mul(body->density, m_mul(sample_size,m_mul(sample_size,sample_size)));

//     // iterate over grid cells of bounding box
//     p_mesh.visible_count = 0;
//     vec3 sample_coord;
//     for(sample_coord.x = min.x + m_div(sample_size,fl2real(1)); sample_coord.x < max.x; sample_coord.x += sample_size)
//     for(sample_coord.y = min.y + m_div(sample_size,fl2real(1)); sample_coord.y < max.y; sample_coord.y += sample_size)
//     for(sample_coord.z = min.z + m_div(sample_size,fl2real(1)); sample_coord.z < max.z; sample_coord.z += sample_size)
//     {
//         if(point_2_mesh_collision(sample_coord, mesh_copy, NULL))
//         {
//             // vec3 d_inertia = moment_of_inertia_for_a_box((vec3){sample_size,sample_size,sample_size}, dm, axis);
//             // body->inertia += d_inertia.x;
//             real r = vec_length(vec_subtract(sample_coord, vec_scalar_multiply(axis, vec_dot_product(sample_coord, axis))));
//             body->inertia += m_mul(dm, m_mul(r,r)); //use cube moment of inertia instead?
            
//             if(p_mesh.visible_count <= PARTICLE_RENDER_CHUNK_SIZE)
//                 p_mesh.vertices[p_mesh.visible_count++].pos = sample_coord;
//         }
//     }

//     free(mesh_copy.vertices);
// }

//TODO reconsider API design (either MOI around axis or, just principal axes)
inline real moment_of_inertia_for_a_point(vec3 p, real m, vec3 axis)
{
    //p is relative to center of mass
    vec3 p_rejection = vec_subtract(axis, vec_scalar_multiply(axis,vec_dot_product(p, axis)));
    real r = vec_length(p_rejection);
    real inertia = m_mul(m, m_mul(r,r));

    return inertia;
}

vec3 moment_of_inertia_for_a_rod(real l, real m, vec3 axis)
{
    real inertia = m_div(m_mul(m, m_mul(l,l)), 3);

    return (vec3)
    {
        inertia,
        inertia,
        fl2real(0.0)
    };
}

vec3 moment_of_inertia_for_a_disk(real r, real m, vec3 axis)
{
    real inertia = m_div(m_mul(m, m_mul(r,r)), 2);
    
    return (vec3)
    {
        m_div(inertia, 2),
        m_div(inertia, 2),
        inertia
    };
}

vec3 moment_of_inertia_for_a_sphere(real r, real m)
{
    real inertia = m_mul(m_mul(m_div(2,5), m), m_mul(r, r));

    return (vec3)
    {
        inertia,
        inertia,
        inertia
    };
}

vec3 moment_of_inertia_for_a_box(vec3 size, real m, vec3 axis)
{
    return (vec3)
    {
        m_div(m_mul(m, m_mul(size.y, size.y) + m_mul(size.z, size.z)), 12),
        m_div(m_mul(m, m_mul(size.x, size.x) + m_mul(size.z, size.z)), 12),
        m_div(m_mul(m, m_mul(size.y, size.y) + m_mul(size.x, size.x)), 12)
    };
}

vec3 moment_of_inertia_for_a_cylinder(real h, real r, real m, vec3 axis)
{
    return  vec_add(moment_of_inertia_for_a_disk(r, m, axis)
                    ,
                    moment_of_inertia_for_a_rod(h, m, axis));
}
#endif