#ifdef DEFINITION_MODE
float get_inverse_mass(float mass);
#endif


#ifdef IMPLEMENTATION_MODE
float get_inverse_mass(float mass)
{
    if(fabs(mass) < 0.00001)
        return 0.9999999;

    return 1.0/mass;
}
#endif