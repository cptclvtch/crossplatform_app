#ifndef INCLUDE_IMPLEMENTATION

// f = G*m1*m2/(r*r);
// #define GRAVITY 10.0

float get_inverse_mass(float mass);

//energy conservation
//E = mass*speed^2
float energy_conserved_mass_scaling(float e, float ds);
float energy_conserved_speed_scaling(float e, float dm);

//----------------------------------
#else
//----------------------------------

#ifndef GRAVITY
#define GRAVITY 10.0
#endif

float get_inverse_mass(float mass)
{
    if(fabs(mass) < 0.00001)
        return 0.9999999;

    return 1.0/mass;
}

//energy conservation
float energy_conserved_mass_scaling(float energy, float dspeed)
{
    return energy/(dspeed*dspeed);
}

float energy_conserved_speed_scaling(float energy, float dmass)
{
    return sqrt(energy/dmass);
}

float gravity_scaling(float dspeed)
{
    return GRAVITY/dspeed;
}
#endif