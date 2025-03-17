#ifndef API_IMPLEMENTATION_ONLY
typedef struct
{
    // vec3* position_cache;
    phys_point* array;
    uint16_t max_particle_index;

    float lifetime;
}cpu_particles;

//gpu particles 256x256 textures (64x256 particles, 4 vec3's per particle)?

cpu_particles* get_new_particle_bunch(vec3 offset, uint16_t max_index);

//presets
void apply_particle_explosion_preset(cpu_particles* bunch, float force);

void delete_particle_bunch(cpu_particles* bunch);

void integrate_cpu_particles(cpu_particles* bunch, float dT /*seconds*/);

//----------------------------------
#else
//----------------------------------
int32_t iseed = 0;
float frand()
{
    iseed = iseed*214013 + 2531011;
    return (float)iseed/UINT32_MAX;
}

cpu_particles* get_new_particle_bunch(vec3 offset, uint16_t max_index)
{
    cpu_particles* particles = (cpu_particles*)calloc(1,sizeof(cpu_particles));
    particles->array = (phys_point*)calloc(max_index + 1, sizeof(phys_point));
    particles->max_particle_index = max_index;

    uint16_t index = 0;
    for(; index <= particles->max_particle_index; index++)
    {
        particles->array[index].p = offset;
        particles->array[index].inverse_mass = get_inverse_mass(0.0);
    }
    return particles;
}

//presets
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
    if(!bunch) return;

    free(bunch->array);
    free(bunch);
}

void integrate_cpu_particles(cpu_particles* bunch, float dT)
{
    uint16_t index = 0;
    for(; index <= bunch->max_particle_index; index++)
        integrate_phys_point(&bunch->array[index], dT);
}
#endif