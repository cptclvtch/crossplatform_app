#ifndef API_IMPLEMENTATION_ONLY
typedef struct
{
    float x,y,z;
    float vx,vy,vz;
    float ax,ay,az;
    float inverse_mass;
    // float damping;
}particle;

void particle_integrator(particle* p);
#endif

#ifdef API_IMPLEMENTATION_ONLY

f = G*m1*m2/(r*r);
#define GRAVITY 10.0

void particle_integrator(particle* p, uint16_t no_of_timesteps)
{
    if(p->inverse_mass <= 0.0) return;

    
}
#endif