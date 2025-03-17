#ifndef API_IMPLEMENTATION_ONLY
typedef struct
{
    phys_real_spring internal_spring;

    float compression_limit;
    //hard constraint
}phys_real_spring;

phys_real_spring* get_real_spring();

void delete_spring(phys_real_spring* s);

//----------------------------------
#else
//----------------------------------

void delete_real_spring(phys_real_spring* s)
{
    if(s)
        free(s->internal_spring);
    free(s);
}
#endif