#ifndef API_IMPLEMENTATION_ONLY
typedef struct
{
    vec3 p;
    vec3 v;
    vec3 a;
    float inverse_mass;
}cpu_particle;

float particle_damping;

typedef struct
{
    // vec3* position_cache;
    cpu_particle* array;
    uint16_t max_particle_index;
}cpu_particles;

cpu_particles* get_new_particle_bunch(uint16_t max_index);

void apply_particle_explosion_preset(cpu_particles* bunch, float force);

void delete_particle_bunch(cpu_particles* bunch);

void integrate_cpu_particles(cpu_particles* bunch, float dT /*seconds*/);

//ballistics helpers


//----------------------------------
#else
//----------------------------------
int32_t iseed = 0;
float frand()
{
    iseed = iseed*214013 + 2531011;
    return (float)iseed/UINT32_MAX;
}

cpu_particles* get_new_particle_bunch(uint16_t max_index)
{
    cpu_particles* particles = (cpu_particles*)calloc(1,sizeof(cpu_particles));
    particles->array = (cpu_particle*)calloc(max_index + 1, sizeof(cpu_particle));
    particles->max_particle_index = max_index;

    uint16_t index = 0;
    for(; index <= particles->max_particle_index; index++)
        particles->array[index].inverse_mass = get_inverse_mass(0.0);

    return particles;
}

void apply_particle_explosion_preset(cpu_particles* bunch, float force)
{
    uint16_t index;   
    for(index = 0; index <= bunch->max_particle_index; index++)
    {
        float acc = force * bunch->array[index].inverse_mass;
        bunch->array[index].a = (vec3){acc*frand(), acc*frand(), acc*frand()};
    }
}

void delete_particle_bunch(cpu_particles* bunch)
{
    free(bunch->array);
    bunch->array = NULL;
}

void integrate_cpu_particle(cpu_particle* p, float dT /*seconds*/)
{
    if(p->inverse_mass <= 0.0) return;

    //update position
    p->p.x += p->v.x*dT;// + p->a.x*dT*dT; //acceleration component not needed
    p->p.y += p->v.y*dT;// + p->a.y*dT*dT;
    p->p.z += p->v.z*dT;// + p->a.z*dT*dT;

    //determine acceleration

    //update velocity
    p->v.x = p->v.x*particle_damping + p->a.x*dT;
    p->v.y = p->v.y*particle_damping + p->a.y*dT;
    p->v.z = p->v.z*particle_damping + p->a.z*dT;

    //clear accumulator
}

void integrate_cpu_particles(cpu_particles* bunch, float dT /*seconds*/)
{
    particle_damping = .99;
    //iterate over particles
    uint16_t index = 0;
    for(; index <= bunch->max_particle_index; index++)
        integrate_cpu_particle(&bunch->array[index], dT);
}
#endif