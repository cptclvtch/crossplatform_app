#ifndef INCLUDE_IMPLEMENTATION
typedef struct
{
    //Dynamics
        vec3* p; //m
        vec3 v; //m/s
        vec3 a; //m/s^2
        float inverse_mass; //1/kg

    //d'alembert cache
        vec3 force_accumulator; //N
}phys_point;

//----------------------------------
#else
//----------------------------------

void linear_integration(phys_point* p, float dT /*seconds*/)
{
    float linear_damping = .99;

    if(p->inverse_mass <= 0.0) return;

    //update position
    *(p->p) = vec_add(*(p->p), vec_scalar_multiply(p->v, dT));
    // p->p = vec_add(p->p, vec_scalar_multiply(p->a, dT*dT)); //if acceleration component needed

    //determine acceleration
    p->a = vec_add(p->a, vec_scalar_multiply(p->force_accumulator, p->inverse_mass));
    p->a = vec_add(p->a, (vec3){0,0,-GRAVITY});

    //update velocity
    p->v = vec_add(vec_scalar_multiply(p->v, linear_damping), vec_scalar_multiply(p->a, dT));

    //clear accumulator
    p->force_accumulator = (vec3){0};
    p->a = (vec3){0};
}
#endif