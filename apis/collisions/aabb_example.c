#include <stdio.h>

#include "aabb.c"
#define INCLUDE_IMPLEMENTATION
#include "aabb.c"

int main()
{
    aabb a,b;

    a.center = (vec3){0,0,0};
    a.half_size = (vec3){1,1,1};

    b.center = (vec3){1,1,1};
    b.half_size = a.half_size;

    printf("\nBoxes a and b are %s.\n", aabb_check(a,b)? "overlapping":"not overlapping");

    aabb size_of_the_two_boxes = aabb_for_pair(a,b, AABB_SIZE_CALCULATION);
    aabb size_and_location_of_boxes = aabb_for_pair(a,b, AABB_FULL_CALCULATION);
}