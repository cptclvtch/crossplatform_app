#ifndef API_IMPLEMENTATION_ONLY

#ifndef VEC3
typedef struct
{
    float x,y,z;
}vec3;
#endif

// f = G*m1*m2/(r*r);
#define GRAVITY 10.0

float get_inverse_mass(float mass);

//energy conservation
//E = mass*speed^2
float energy_conserved_mass_scaling(float e, float ds);
float energy_conserved_speed_scaling(float e, float dm);

//----------------------------------
#else
//----------------------------------

float get_inverse_mass(float mass)
{
    if(fabs(mass) < 0.00001)
        return 0.9999999;

    return 1.0/mass;
}

//energy conservation
float energy_conserved_mass_scaling(float e, float ds)
{
    return e/(ds*ds);
}

float energy_conserved_speed_scaling(float e, float dm)
{
    return sqrt(e/dm);
}

float gravity_scaling(float ds)
{
    return GRAVITY/ds;
}
#endif