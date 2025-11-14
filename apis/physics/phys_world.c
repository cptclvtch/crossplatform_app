#ifndef INCLUDE_IMPLEMENTATION
#include "../data_structures/linked_list/api.c"

typedef struct
{
    //points and particles
    ll_node* points;
    ll_node* particle_bunches;

    //rigid bodies
    ll_node* rigid_bodies;

    //force generators
    // ll_node* force_generators;

    //constraints
    ll_node* virtual_springs;

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

#define ADD_PHYS_ITEM(type, list, get_new) {\
type* new_item = get_new;\
if(new_item) list = ll_add_data(list, PREV, new_item);\
return new_item;\
}

phys_point* add_point(phys_world* world, vec3* pos)
{
    if(!world) return NULL;
    phys_point* a = (phys_point*)calloc(1, sizeof(phys_point));
    a->p = pos;
    ADD_PHYS_ITEM(phys_point, world->points, a)
}

cpu_particles* add_particle_bunch(phys_world* world, vec3* pos, uint16_t quantity)
{
    if(!world) return NULL;
    ADD_PHYS_ITEM(cpu_particles, world->particle_bunches, get_new_particle_bunch(pos, quantity))
}

phys_virtual_spring* add_virtual_spring(phys_world* world, phys_point* a, phys_point* b, float k, float r)
{
    //TODO reconsider phys_point inputs
    if(!world) return NULL;
    if(!a || !b) return NULL;

    world->points = ll_add_data(world->points, PREV, a);
    world->points = ll_add_data(world->points, PREV, b);
    ADD_PHYS_ITEM(phys_virtual_spring, world->virtual_springs, get_virtual_spring(a, b, k, r))
}

phys_rigid_body* add_rigid_body(phys_world* world, phys_point* center_of_mass, rotor3* orientation, collision_volume* shape/*, inverse inertia tensor*/)
{
    //TODO reconsider phys_point inputs
    if(!world) return NULL;
    if(!center_of_mass) return NULL;

    world->points = ll_add_data(world->points, PREV, center_of_mass);

    phys_rigid_body* new_item = (phys_rigid_body*)calloc(1, sizeof(phys_rigid_body));
    if(new_item)
    {
        new_item->representation = *shape;
        new_item->center_of_mass = center_of_mass;
        new_item->orientation = orientation;
        //TODO restitution?
        //TODO calculate moment of inertia and volume here?
        //TODO calculate either mass or density
        world->rigid_bodies = ll_add_data(world->rigid_bodies, PREV, new_item);
    }

    return new_item;
}

#undef ADD_PHYS_ITEM

void update_forces(phys_world* world)
{
    ITERATE_LIST_START(ll_node, world->virtual_springs, s)
        apply_spring_force(s->data);
    ITERATE_LIST_END(NEXT, s)
}

void integrate_physics(phys_world* world, float dT)
{
    //rigid body angular integration
    ITERATE_LIST_START(ll_node, world->rigid_bodies, b)
        angular_integration((phys_rigid_body*)(b->data), dT);
    ITERATE_LIST_END(NEXT, b)

    //point linear integration
    ITERATE_LIST_START(ll_node, world->points, p)
        linear_integration((phys_point*)(p->data), dT);
    ITERATE_LIST_END(NEXT, p)

    ITERATE_LIST_START(ll_node, world->particle_bunches, pb)
        integrate_cpu_particles((cpu_particles*)(pb->data), dT);
    ITERATE_LIST_END(NEXT, pb)
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
    //update bvh
    free_bvh_bt_node(world->broad_phase_root);

    ITERATE_LIST_START(ll_node, world->rigid_bodies, body)
        phys_rigid_body* rigid_body = body->data;
        bvh_bt_node* to_add = create_bvh_bt_node(bvh_data_from_volume(&(rigid_body->representation)));
        world->broad_phase_root = bvh_insert(world->broad_phase_root, to_add);
    ITERATE_LIST_END(NEXT, body)
    //

    gather_contacts(world->broad_phase_root, &(world->collisions), NULL /*custom_broad_phase*/);
    // resolve_contacts(world->collisions, dT);
    update_forces(world);
    integrate_physics(world, dT);
}

void delete_phys_world(phys_world* world)
{
    //bodies
    free_ll_chain(world->rigid_bodies, NEXT);

    //particles
    free_ll_chain(world->points, NEXT);
    free_ll_chain(world->particle_bunches, NEXT);

    //force generators
    free_ll_chain(world->virtual_springs, NEXT);

    free_bvh_bt_node(world->broad_phase_root);
}
#endif