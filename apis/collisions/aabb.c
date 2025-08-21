//Dependencies
#include "../geometric_algebra/api.c"

#ifndef _AABB_H
    #define _AABB_H

typedef struct
{
    vec3 center;
    vec3 half_size; //(vec3){0} should be treated same as a non-existent aabb
} aabb;

enum{
    AABB_FULL_CALCULATION,
    AABB_SIZE_CALCULATION
};
uint8_t aabb_check(aabb a, aabb b);
aabb aabb_for_pair(aabb a, aabb b, uint8_t size_only);
#endif //_AABB_H

//----------------------------------

#if defined(INCLUDE_IMPLEMENTATION) && !defined(_AABB_C)
    #define _AABB_C

uint8_t aabb_check(aabb a, aabb b)
{
    //just like spheres, but even easier
    vec3 center_diff = vec_abs(vec_subtract(a.center, b.center));
    vec3 total_size = vec_add(a.half_size, b.half_size);
    return (center_diff.x <= total_size.x &&
            center_diff.y <= total_size.y &&
            center_diff.z <= total_size.z);
}

aabb aabb_for_pair(aabb a, aabb b, uint8_t size_only)
{
    if(a.half_size.x == fl2real(0.0) && a.half_size.y == fl2real(0.0) && a.half_size.z == fl2real(0.0))
        return b;

    if(b.half_size.x == fl2real(0.0) && b.half_size.y == fl2real(0.0) && b.half_size.z == fl2real(0.0))
        return a;

    vec3 max_corner = vec_max(  vec_add(a.center, a.half_size),
                                vec_add(b.center, b.half_size));

    vec3 min_corner = vec_min(  vec_subtract(a.center, a.half_size),
                                vec_subtract(b.center, b.half_size));

    aabb new = {0};
    new.half_size = vec_scalar_divide(vec_subtract(max_corner, min_corner), 2);
    if(size_only) return new;

    new.center = vec_add(min_corner, new.half_size);

    return new;
}
#endif