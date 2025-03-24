#ifndef API_IMPLEMENTATION_ONLY
typedef struct
{
    vec3 p; //m
    vec3 v; //m/s
    vec3 a; //m/s^2
    float inverse_mass; //1/kg

    vec3 force_accumulator; //N
}phys_point;

//----------------------------------
#else
//----------------------------------

void integrate_phys_point(phys_point* p, float dT /*seconds*/)
{
    if(p->inverse_mass <= 0.0) return;

    float particle_damping = .99;

    //update position
    p->p = vec_add(p->p, vec_scalar_multiply(p->v, dT));
    // p->p = vec_add(p->p, vec_scalar_multiply(p->a, dT*dT)); // acceleration component node needed

    //determine acceleration
    p->a = vec_add(p->a, vec_scalar_multiply(p->force_accumulator, p->inverse_mass));
    p->a = vec_add(p->a, (vec3){0,0,-GRAVITY});

    //update velocity
    p->v = vec_add(vec_scalar_multiply(p->v, particle_damping), vec_scalar_multiply(p->a, dT));

    //clear accumulator
    p->force_accumulator = (vec3){0};
    p->a = (vec3){0};
}
#endif