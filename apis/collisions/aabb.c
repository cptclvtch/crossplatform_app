//Dependencies
#include "../geometric_algebra/api.c"

#ifndef _AABB_H
    #define _AABB_H

typedef struct
{
    vec3 center;
    vec3 half_size; //(vec3){0} should be treated same as a non-existent aabb
} aabb;

uint8_t aabb_is_invalid(aabb a);
uint8_t aabb_aabb_check(aabb a, aabb b);
uint8_t aabb_vec3_check(aabb a, vec3 b);
aabb aabb_union(aabb a, aabb b);
aabb aabb_intersection(aabb a, aabb b);
#endif //_AABB_H

//----------------------------------

#if defined(INCLUDE_IMPLEMENTATION) && !defined(_AABB_C)
    #define _AABB_C

static inline uint8_t aabb_is_invalid(aabb a)
{
    return  a.half_size.x < M_EPSILON &&
            a.half_size.y < M_EPSILON &&
            a.half_size.z < M_EPSILON;
}

#define CHECK_CALCULATION \
return (center_diff.x <= total_size.x &&\
        center_diff.y <= total_size.y &&\
        center_diff.z <= total_size.z);

uint8_t aabb_aabb_check(aabb a, aabb b)
{
    if(aabb_is_invalid(a)) return 0;
    if(aabb_is_invalid(b)) return 0;

    vec3 total_size = vec_add(a.half_size, b.half_size);
    vec3 center_diff = vec_abs(vec_subtract(a.center, b.center));
    CHECK_CALCULATION
}

uint8_t aabb_vec3_check(aabb a, vec3 b)
{
    if(aabb_is_invalid(a)) return 0;

    vec3 total_size = a.half_size;
    vec3 center_diff = vec_abs(vec_subtract(a.center, b));
    CHECK_CALCULATION
}
#undef CHECK_CALCULATION

aabb aabb_union(aabb a, aabb b)
{
    if(aabb_is_invalid(a)) return b;
    if(aabb_is_invalid(b)) return a;

    vec3 min = vec_min( vec_subtract(a.center, a.half_size),
                        vec_subtract(b.center, b.half_size));

    vec3 max = vec_max( vec_add(a.center, a.half_size),
                        vec_add(b.center, b.half_size));
    
    min = vec_scalar_divide(min,2);
    max = vec_scalar_divide(max,2);

    return  (aabb)
            {
                vec_add(max, min),
                vec_subtract(max, min)
            };
}

aabb aabb_intersection(aabb a, aabb b)
{
    if(aabb_is_invalid(a)) return a;
    if(aabb_is_invalid(b)) return b;

    vec3 min =  vec_max(vec_subtract(a.center, a.half_size),
                        vec_subtract(b.center, b.half_size));

    vec3 max = vec_min( vec_add(a.center, a.half_size),
                        vec_add(b.center, b.half_size));

    min = vec_scalar_divide(min,2);
    max = vec_scalar_divide(max,2);

    return  (aabb)
            {
                vec_add(max,min),
                vec_subtract(max,min)
            };
}
#endif