#ifndef INCLUDE_IMPLEMENTATION
typedef struct
{
    //points and particles
    linked_list points;
    linked_list particle_bunches;

    //rigid bodies
    linked_list rigid_bodies;

    //force generators
    // linked_list force_generators;

    //constraints
    linked_list virtual_springs;

    //collision internals
    binary_tree* broad_phase_root;
    collision_list* collisions;
}phys_world;

phys_point* add_point(phys_world* world, vec3 offset);
cpu_particles* add_particle_bunch(phys_world* world, vec3 offset, uint16_t quantity);
phys_virtual_spring* add_virtual_spring(phys_world* world, phys_point* a, phys_point* b, float k, float r);
phys_rigid_body* add_rigid_body(phys_world* world, phys_point* center_of_mass /*, inverse inertia tensor*/);

void physics_update(phys_world* world, float dT);

void delete_phys_world(phys_world* world);

//----------------------------------
#else
//----------------------------------

#define ADD_PHYS_ITEM(type, list, get_new) {\
type* new_item = get_new;\
if(new_item) list.nodes = add_link_before(list.nodes, new_item);\
return new_item;\
}

phys_point* add_point(phys_world* world, vec3 offset)
{
    if(!world) return NULL;
    phys_point* a = (phys_point*)calloc(1, sizeof(phys_point));
    a->p = offset;
    ADD_PHYS_ITEM(phys_point, world->points, a)
}

cpu_particles* add_particle_bunch(phys_world* world, vec3 offset, uint16_t quantity)
{
    if(!world) return NULL;
    ADD_PHYS_ITEM(cpu_particles, world->particle_bunches, get_new_particle_bunch(offset, quantity))
}

phys_virtual_spring* add_virtual_spring(phys_world* world, phys_point* a, phys_point* b, float k, float r)
{
    //TODO reconsider phys_point inputs
    if(!world) return NULL;
    if(!a || !b) return NULL;

    world->points.nodes = add_link_before(world->points.nodes, a);
    world->points.nodes = add_link_before(world->points.nodes, b);
    ADD_PHYS_ITEM(phys_virtual_spring, world->virtual_springs, get_virtual_spring(a, b, k, r))
}

phys_rigid_body* add_rigid_body(phys_world* world, phys_point* center_of_mass /*, inverse inertia tensor*/)
{
    if(!world) return NULL;
    if(!center_of_mass) return NULL;

    world->points.nodes = add_link_before(world->points.nodes, center_of_mass);

    ADD_PHYS_ITEM(phys_rigid_body, world->rigid_bodies, (phys_rigid_body*)calloc(1,sizeof(phys_rigid_body)))
}

#undef ADD_PHYS_ITEM

void update_forces(phys_world* world)
{
    ITERATE_LIST_START(world->virtual_springs, s)
        apply_spring_force(s->data);
    ITERATE_LIST_END(NEXT, s)
}

void integrate_physics(phys_world* world, float dT)
{
    //rigid body angular integration
    ITERATE_LIST_START(world->rigid_bodies, b)
        angular_integration(b->data, dT);
    ITERATE_LIST_END(NEXT, b)

    //point linear integration
    ITERATE_LIST_START(world->points, p)
        linear_integration(p->data, dT);
    ITERATE_LIST_END(NEXT, p)

    ITERATE_LIST_START(world->particle_bunches, pb)
        integrate_cpu_particles((cpu_particles*)pb->data, dT);
    ITERATE_LIST_END(NEXT, pb)
}

void custom_broad_phase(binary_tree* root, collision_list* list)
{
    /*
    - static and dynamic split at root?
    - split static branch into permanent and dormant?
    */
    
}

void physics_update(phys_world* world, float dT)
{
    gather_contacts(world->broad_phase_root, world->collisions, NULL /*custom_broad_phase*/);
    resolve_contacts(world->collisions, dT);
    update_forces(world);
    integrate_physics(world, dT);
}

void delete_phys_world(phys_world* world)
{
    world->points.delete_func = NULL;
    world->particle_bunches.delete_func = &delete_particle_bunch;

    // world->virtual_springs.delete_func = &delete_virtual_spring;

    //bodies
    delete_list(&(world->rigid_bodies));

    //particles
    delete_list(&(world->points));
    delete_list(&(world->particle_bunches));

    //force generators
    delete_list(&(world->virtual_springs));
}
#endif