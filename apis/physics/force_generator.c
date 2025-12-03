#ifndef INCLUDE_IMPLEMENTATION
typedef struct
{
    //affected objects
    linked_list particles;
    linked_list particle_bunches;

    //function
    void (*apply_force)(void* self, void* inOutBody);
}force_generator;

void delete_force_generator(force_generator* fg);

void apply_forces(force_generator* fg);

//----------------------------------
#else
//----------------------------------

force_generator* get_new_force_generator(void (*force_func)(void*,void*))
{
    force_generator* fg = (force_generator*)calloc(1, sizeof(force_generator));

    fg->particles = create_new_list(&dont_delete_contents);
    fg->particle_bunches = create_new_list(&dont_delete_contents);

    fg->apply_force = force_func;

    return fg;
}

void delete_force_generator(force_generator* fg)
{
    delete_list(fg->particles);
    delete_list(fg->particle_bunches);
}

void apply_forces(force_generator* fg)
{
    ITERATE_LIST_START(fg->particles, p)
        if(fg->apply_force) fg->apply_force(fg, p); else break;
    ITERATE_LIST_END(NEXT, p)

    ITERATE_LIST_START(fg->particles, pb)
        if(fg->apply_force) fg->apply_force(fg, pb); else break;
    ITERATE_LIST_END(NEXT, pb)
}
#endif