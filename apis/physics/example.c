#include <stdio.h>

#include "api.c"
#define INCLUDE_IMPLEMENTATION
#include "api.c"

void print_points(phys_world* w)
{
    uint32_t index;
    for(index = 0; index < w->point_count; index++)
    {
        vec3* p = w->points[index].p;
        printf("%f, %f, %f\n", p->x, p->y, p->z);
    }
}

int main()
{
    phys_world sim = (phys_world){0};

    phys_point* test_point = add_point(&sim, NULL);
    add_rigid_body( &sim,
                    test_point,
                    NULL,
                    create_primitive_collision_volume(SPHERE, (vec3){1.0,1.0,1.0}, NULL, NULL));

    sim.points[0].inverse_mass = 1.0;

    add_rigid_body( &sim,
                    add_point(&sim, NULL),
                    NULL,
                    create_primitive_collision_volume(SPHERE, (vec3){1.0,1.0,1.0}, NULL, NULL));

    //
    
    uint32_t frame = 0;
    while(frame++ < 100)
    {
        printf("----%u----\n", frame);
        
        sim.points[0].force_accumulator = (vec3){5,0,0};
        physics_update(&sim, 1.0/60.0);

        print_points(&sim);
    }
    
}
