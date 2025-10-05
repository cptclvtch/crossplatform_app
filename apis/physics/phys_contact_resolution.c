#ifdef INCLUDE_IMPLEMENTATION
real calculate_separating_velocity(phys_rigid_body* a, phys_rigid_body* b, vec3 normal)
{
    vec3 relative_velocity = vec_subtract(a->center_of_mass->v, b->center_of_mass->v);

    return vec_dot_product(relative_velocity, normal);
}

void resolve_velocity(collision_pair* p, real dT)
{
    if(p->type == NO_COLLISION) return;

    //TODO replace upcasting?
    phys_rigid_body* a = (phys_rigid_body*)p->members[0];
    phys_rigid_body* b = (phys_rigid_body*)p->members[1];
    
    if(a->center_mass->inverse_mass + b->center_mass->inverse_mass <= 0) return;

    real separating_velocity = calculate_separating_velocity(a, b, p->points[0].normal);

    if(separating_velocity > 0) return;

    real inverse_mass_sum = a->center_mass->inverse_mass + b->center_mass->inverse_mass;

    if(inverse_mass_sum <= 0) return;

    real pair_restitution = m_div(a->k_restitution + b->k_restitution, 2);

    real new_sep_velocity = m_mul(-separating_velocity, pair_restitution);

    //resting contact correction
    // vec3 acc = vec_subtract(a->center_of_mass->a, b->center_of_mass->a);
    // float acc_caused_velocity = m_mul(vec_dot_product(acc, p->points[0].normal), dT);

    // if(acc_caused_velocity < 0)
    // {
    //     new_sep_velocity += m_mul(acc_caused_velocity, pair_restitution);

    //     if(new_sep_velocity < 0) new_sep_velocity = 0;
    // }
    //

    real total_impulse = m_div(new_sep_velocity - separating_velocity, inverse_mass_sum);

    //FIXME does this make sense?
    // vec3 impulse_per_inverse_mass = vec_scalar_multiply(p->points[0]->normal, total_impulse);
    // possible correction: 
    vec3 impulse_per_inverse_mass = vec_scalar_multiply(p->points[0]->normal, m_mul(total_impulse, inverse_mass_sum));

    a->v = vec_add(a->v, vec_scalar_multiply(impulse_per_inverse_mass, a->inverse_mass));
    b->v = vec_add(b->v, vec_scalar_multiply(impulse_per_inverse_mass, -b->inverse_mass));
}

rotor3 resolve_interpenetration(phys_rigid_body* a, phys_rigid_body* b, contact* c)
{
    if(c->penetration <= 0) return;

    real inverse_mass_sum = a->center_mass->inverse_mass + b->center_mass->inverse_mass;

    if(inverse_mass_sum <= 0) return;

    real scale_a = m_div(a->center_mass->inverse_mass, inverse_mass_sum);

    vec3 correction_a = vec_scalar_multiply(c->normal, m_mul(-c->penetration, scale_a));

    a->p = vec_add(a->p, correction_a);
    b->p = vec_add(b->p, vec_scalar_multiply(correction_a, scale_a - 1));

    return (rotor3){.T = correction_a, .II = scale_a - 1};
}

//
void resolve_contacts(collision_list* list, real dT)
{
    uint8_t pair_index = 0;
    for(; pair_index < list->count; pair_index++)
    {
        collision_pair* pair = &(list->pairs[pair_index]);

        //TODO replace upcasting?
        phys_rigid_body* a = (phys_rigid_body*)pair->members[0];
        phys_rigid_body* b = (phys_rigid_body*)pair->members[1];

        uint8_t iteration = 0;
        uint8_t max_iterations = pair->contact_count/* *2 */;

        while(iteration < max_iterations)
        {
            real min_sep_velocity = REAL_MAX;
            uint8_t min_contact_index = 0;

            uint8_t contact_index = 0;
            for(; contact_index < pair->contact_count; contact_index++)
            {
                real sep_velocity = calculate_separating_velocity(a, b, pair->points[contact_index].normal);
                if( sep_velocity < min_sep_velocity
                    &&
                    (sep_velocity < 0 || pair->points[contact_index]->penetration > 0))//FIXME does the penetration check make sense?
                {
                    min_sep_velocity = sep_velocity;
                    min_contact_index = contact_index;
                }
            }

            if(min_contact_index == pair->contact_count) break;

            resolve_velocity(contact, dT);
            rotor3 correction = resolve_interpenetration(contact);

            //TODO update interpenetrations

            iteration++;
        }
    }
}
#endif