#ifndef API_IMPLEMENTATION_ONLY
typedef struct
{
    //list of points
    linked_list points;

    //list of particle bunches
    linked_list particle_bunches;

    //force generators
    // linked_list force_generators;
    linked_list virtual_springs;

}phys_world;

phys_point* add_point(phys_world* world, vec3 offset);
cpu_particles* add_particle_bunch(phys_world* world, vec3 offset, uint16_t quantity);
phys_virtual_spring* add_virtual_spring(phys_world* world, phys_point* a, phys_point* b, float k, float r);

void physics_update(phys_world* world, float dT);

void delete_phys_world(phys_world* world);

//----------------------------------
#else
//----------------------------------

#define ADD_PHYS_ITEM(type, list, get_new) {\
type* new_item = get_new;\
if(!new_item) return NULL;\
\
world->list.nodes = add_link_before(world->list.nodes, new_item);\
\
return new_item;\
}

phys_point* add_point(phys_world* world, vec3 offset)
{
    if(!world) return NULL;
    phys_point* a = (phys_point*)calloc(1, sizeof(phys_point));
    a->p = offset;
    ADD_PHYS_ITEM(phys_point, points, a)
}

cpu_particles* add_particle_bunch(phys_world* world, vec3 offset, uint16_t quantity)
{
    if(!world) return NULL;
    ADD_PHYS_ITEM(cpu_particles, particle_bunches, get_new_particle_bunch(offset, quantity))
}

phys_virtual_spring* add_virtual_spring(phys_world* world, phys_point* a, phys_point* b, float k, float r)
{
    //TODO reconsider phys_point inputs
    if(!world) return NULL;
    if(!a || !b) return NULL;

    world->points.nodes = add_link_before(world->points.nodes, a);
    world->points.nodes = add_link_before(world->points.nodes, b);
    ADD_PHYS_ITEM(phys_virtual_spring, virtual_springs, get_virtual_spring(a, b, k, r))
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
    //points
    ITERATE_LIST_START(world->points, p)
        integrate_phys_point(p->data, dT);
    ITERATE_LIST_END(NEXT, p)

    ITERATE_LIST_START(world->particle_bunches, pb)
        integrate_cpu_particles((cpu_particles*)pb->data, dT);
    ITERATE_LIST_END(NEXT, pb)
}

void physics_update(phys_world* world, float dT)
{
    update_forces(world);
    integrate_physics(world, dT);
}

void delete_phys_world(phys_world* world)
{
    world->points.delete_func = NULL;
    world->particle_bunches.delete_func = &delete_particle_bunch;

    world->virtual_springs.delete_func = &delete_virtual_spring;

    //particles
    delete_list(&(world->points));
    delete_list(&(world->particle_bunches));

    //force generators
    delete_list(&(world->virtual_springs));
}
#endif