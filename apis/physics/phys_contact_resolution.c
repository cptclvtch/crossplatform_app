#ifndef INCLUDE_IMPLEMENTATION
typedef struct{
    phys_point* primary;
    phys_point* secondary;

    float restitution;

    vec3 normal;
    float penetration;
}phys_contact;

//----------------------------------
#else
//----------------------------------

void resolve_contact(phys_contact* contact)
{
    resolve_velocity(contact);
    resolve_interpenetration(contact);
}

float calculate_separating_velocity(phys_contact* contact)
{
    vec3 relative_velocity = contact->primary->v;

    if(contact->secondary) relative_velocity = vec_subtract(relative_velocity, contact->secondary->v);

    return vec_dot_product(relative_velocity, contact->normal);
}

void resolve_velocity(phys_contact* contact, float dT)
{
    float separating_velocity = calculate_separating_velocity(contact);

    if(separating_velocity > 0) return;

    phys_point* primary = contact->primary;
    phys_point* secondary = contact->secondary;

    float total_inverse_mass = primary->inverse_mass;
    if(secondary) total_inverse_mass += secondary->inverse_mass;

    if(total_inverse_mass <= 0) return;

    float new_sep_velocity = -separating_velocity * contact->restitution;

    //resting contact correction
    vec3 acc = primary->a;
    if(secondary) acc = vec_subtract(acc, secondary->a);
    float acc_caused_velocity = vec_dot_product(acc, contact->normal)*dT;

    if(acc_caused_velocity < 0)
    {
        new_sep_velocity += contact->restitution*acc_caused_velocity;

        if(new_sep_velocity < 0) new_sep_velocity = 0;
    }
    //

    float impulse = (new_sep_velocity - separating_velocity)/total_inverse_mass;

    vec3 impulse_per_inverse_mass = vec_scalar_multiply(contact->normal, impulse);

    primary->v = vec_add(primary->v, vec_scalar_multiply(impulse_per_inverse_mass, primary->inverse_mass));

    if(secondary)
        secondary->v = vec_add(secondary->v, vec_scalar_multiply(impulse_per_inverse_mass, -secondary->inverse_mass));
}

void resolve_interpenetration(phys_contact* contact)
{
    if(contact->penetration <= 0) return;

    phys_point* primary = contact->primary;
    phys_point* secondary = contact->secondary;

    float total_inverse_mass = primary->inverse_mass;
    if(secondary) total_inverse_mass += secondary->inverse_mass;

    if(total_inverse_mass <= 0) return;

    vec3 move_per_inverse_mass = vec_scalar_multiply(contact->normal, contact->penetration*total_inverse_mass);

    primary->p = vec_add(primary->p, vec_scalar_multiply(move_per_inverse_mass, primary->inverse_mass));

    if(secondary)
        secondary->p = vec_add(secondary->p, vec_scalar_multiply(move_per_inverse_mass, secondary->inverse_mass));
}
#endif