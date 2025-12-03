#include <stdio.h>

#define INCLUDE_IMPLEMENTATION
#include "../aabb.c"

int main()
{
    aabb a,b;

    a.center = (vec3){0,0,0};
    a.half_size = (vec3){1,1,1};

    b.center = (vec3){1,1,1};
    b.half_size = a.half_size;

    printf("\nBoxes a and b are %s.\n", aabb_aabb_check(a,b)? "overlapping":"not overlapping");

    aabb combo = aabb_union(a,b);
    aabb intersection = aabb_intersection(a,b);
}