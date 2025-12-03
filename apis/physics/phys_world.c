#ifndef INCLUDE_IMPLEMENTATION

#define MAX_PHYS_OBJECTS 100
typedef struct
{
    //points and particles
    phys_point points[MAX_PHYS_OBJECTS];
    uint32_t point_count;

    cpu_particles* particle_bunches[MAX_PHYS_OBJECTS];
    uint32_t particle_bunch_count;

    //rigid bodies
    phys_rigid_body rigid_bodies[MAX_PHYS_OBJECTS];
    uint32_t rigid_body_count;

    //force generators
    // ll_node* force_generators;

    //constraints
    phys_virtual_spring* virtual_springs[MAX_PHYS_OBJECTS];
    uint32_t virtual_spring_count;

    //collision internals
    bvh_bt_node* broad_phase_root;
    collision_list collisions;
}phys_world;

phys_point* add_point(phys_world* world, vec3* pos);
cpu_particles* add_particle_bunch(phys_world* world, vec3* pos, uint16_t quantity);
phys_virtual_spring* add_virtual_spring(phys_world* world, phys_point* a, phys_point* b, float k, float r);
phys_rigid_body* add_rigid_body(phys_world* world, phys_point* center_of_mass, rotor3* orientation, collision_volume* shape/*, inverse inertia tensor*/);

void physics_update(phys_world* world, float dT);

void delete_phys_world(phys_world* world);

//----------------------------------
#else
//----------------------------------

phys_point* add_point(phys_world* world, vec3* pos)
{
    if(!world) return NULL;
    if(world->point_count >= MAX_PHYS_OBJECTS) return NULL;

    if(!pos) pos = (vec3*)calloc(1,sizeof(vec3));
    
    world->points[world->point_count] = (phys_point){.p = pos};
    return &(world->points[world->point_count++]);
}

cpu_particles* add_particle_bunch(phys_world* world, vec3* pos, uint16_t quantity)
{
    if(!world) return NULL;
    if(world->particle_bunch_count >= MAX_PHYS_OBJECTS) return NULL;
    
    cpu_particles* to_add = get_new_particle_bunch(pos, quantity);
    if(to_add)
    world->particle_bunches[world->particle_bunch_count++] = to_add;

    return to_add;
}

phys_virtual_spring* add_virtual_spring(phys_world* world, phys_point* a, phys_point* b, float k, float r)
{
    //TODO reconsider phys_point inputs
    if(!world) return NULL;
    if(world->virtual_spring_count >= MAX_PHYS_OBJECTS) return NULL;
    if(!a || !b) return NULL;

    world->virtual_springs[world->virtual_spring_count] = get_virtual_spring(a,b,k,r);
    
    return &(world->virtual_springs[world->virtual_spring_count++]);
}

phys_rigid_body* add_rigid_body(phys_world* world, phys_point* center_of_mass, rotor3* orientation, collision_volume* shape/*, inverse inertia tensor*/)
{
    //TODO reconsider phys_point inputs
    /* phys_point* input
    pros:
        - can be used to attach physics objects to each other
        - reinforces the idea that a rigid body is attached to a phys_point
    cons:
        - confusing for user
    
    vec3 and mass input
    pros:
        - easy to use
    cons:
        - hard to attach physics objects to each other*/
    if(!world) return NULL;
    if(world->rigid_body_count >= MAX_PHYS_OBJECTS) return NULL;
    if(!center_of_mass) return NULL;
    if(!orientation)
    {
        orientation = (rotor3*)calloc(1,sizeof(rotor3));
        *orientation = IDENTITY_ROTOR;
    }

    //link transforms to collision volume if necessary
    if(!shape->position) shape->position = center_of_mass->p;
    if(!shape->orientation) shape->orientation = orientation;

    //TODO restitution?
    //TODO calculate moment of inertia and volume here?
    //TODO calculate either mass or density
    phys_rigid_body* to_return = &world->rigid_bodies[world->rigid_body_count];
    *to_return = (phys_rigid_body){ .representation = *shape,
                                    .center_of_mass = center_of_mass,
                                    .orientation = orientation};

    world->rigid_body_count++;
    
    return to_return;
}

void update_forces(phys_world* world)
{
    uint32_t index = 0;
    
    for(;index < world->virtual_spring_count; index++)
        apply_spring_force(world->virtual_springs[index]);
}

void integrate_physics(phys_world* world, float dT)
{
    uint32_t index;
    
    //rigid body angular integration
    for(index = 0; index < world->rigid_body_count; index++)
        angular_integration(&(world->rigid_bodies[index]), dT);

    // printf("------\n");

    //point linear integration
    for(index = 0; index < world->point_count; index++)
        linear_integration(&(world->points[index]), dT);

    // for(index = 0; index < world->particle_bunch_count; index++)
    //     integrate_cpu_particles(world->particle_bunches[index], dT);
}

void custom_broad_phase(bvh_bt_node* root, collision_list* list)
{
    /*
    - static and dynamic split at root?
    - split static branch into permanent and dormant?
    */
    
}

void physics_update(phys_world* world, float dT)
{
    printf("------\n");
    //update bvh
    free_bvh_bt_node(world->broad_phase_root);
    world->broad_phase_root = NULL;

    uint32_t index = 0;
    for(; index < world->rigid_body_count; index++)
    {
        phys_rigid_body* rigid_body = &(world->rigid_bodies[index]);
        // printf("[%f, %f, %f]\n", rigid_body->center_of_mass->p->x, rigid_body->center_of_mass->p->y, rigid_body->center_of_mass->p->z);
        bvh_bt_node* to_add = create_bvh_bt_node(bvh_data_from_volume(&(rigid_body->representation)));
        printf("[%f, %f, %f]:(%p)\n", to_add->data.box.center.x, to_add->data.box.center.y, to_add->data.box.center.z, rigid_body->center_of_mass->p);
        world->broad_phase_root = bvh_insert(world->broad_phase_root, to_add);
    }

    //
    // gather_contacts(world->broad_phase_root, &(world->collisions), NULL /*custom_broad_phase*/);
    // resolve_contacts(world->collisions, dT);
    update_forces(world);
    integrate_physics(world, dT);
}

void delete_phys_world(phys_world* world)
{
    free_bvh_bt_node(world->broad_phase_root);
}
#endif