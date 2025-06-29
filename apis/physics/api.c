#ifndef UINT8_MAX
#include <stdint.h>
#endif
#ifndef M_PI
#include <math.h>
#endif
#ifndef RAND_MAX
#include <stdlib.h>
#endif

#ifndef LINKED_LISTS
#error Please include "linked_list/api.c" first
#endif

#ifndef GEOMETRIC_ALGEBRA
#error Please include "geometric_algebra/api.c" first
#endif

#ifndef COLLISIONS
#error Please include "collision/api.c" first
#endif

#include "common.c"
#include "phys_point.c"
#include "particle_bunch.c"

#include "phys_rigid_body.c"

// #include "force_generator.c"
#include "phys_virtual_spring.c"

#include "phys_world.c"