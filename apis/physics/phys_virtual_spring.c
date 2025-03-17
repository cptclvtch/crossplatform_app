#ifndef API_IMPLEMENTATION_ONLY
typedef struct
{
    phys_point* a;
    phys_point* b;

    float k;

    float rest_length;
}phys_virtual_spring;

phys_virtual_spring* get_virtual_spring(phys_point* a, phys_point* b, float k, float r);

void delete_virtual_spring(phys_virtual_spring* s);

void apply_spring_force(phys_virtual_spring* s);

//----------------------------------
#else
//----------------------------------

phys_virtual_spring* get_virtual_spring(phys_point* a, phys_point* b, float k, float r)
{
    if(!a || !b) return NULL;

    phys_virtual_spring* new_spring = (phys_virtual_spring*)calloc(1, sizeof(phys_virtual_spring));
    if(!new_spring) return NULL;

    new_spring->a = a;
    new_spring->b = b;

    if(k > 0.0)
        new_spring->k = k;
    else
        new_spring->k = 1.0;

    new_spring->rest_length = r;

    return new_spring;
}

void delete_virtual_spring(phys_virtual_spring* s)
{
    free(s);
}

void apply_spring_force(phys_virtual_spring* s)
{
    vec3 delta = vec_subtract(s->a->p, s->b->p);

    //-k*(delta - r*direction)
    delta = vec_subtract(delta, vec_scalar_multiply(vec_normalize(delta), s->rest_length));
    vec3 force = vec_scalar_multiply(delta, -s->k);

    s->a->force_accumulator = vec_add(s->a->force_accumulator, force);
    s->b->force_accumulator = vec_add(s->b->force_accumulator, vec_scalar_multiply(force, -1));

    // printf("b->force_accumulator: %f,%f,%f\n", s->b->force_accumulator.x, s->b->force_accumulator.y, s->b->force_accumulator.z);
}
#endif