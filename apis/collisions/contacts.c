//Dependencies
#include "../geometric_algebra/api.c"
#include "volumes.c"

#ifndef INCLUDE_IMPLEMENTATION
//Collision data
typedef struct
{
    vec3 point;
    vec3 normal;
    real penetration;
}contact;

enum
{
    NO_COLLISION,
    RAYCAST_COLLISION,
    CONFIRMED_COLLISION,
    POTENTIAL_COLLISION
};
#define MAX_CONTACT_POINTS 6
typedef struct
{
    uint8_t type;
    collision_volume* members[2];

    //contacts
    contact points[MAX_CONTACT_POINTS];
    uint8_t contact_count;
}collision_pair;

//Collision list
#define COLLISION_CHUNK_SIZE 100
typedef struct
{
    collision_pair pairs[COLLISION_CHUNK_SIZE];
    uint32_t count;
}collision_list;

collision_list* get_new_collision_list();
void clear_collision_list(collision_list* list);

//----------------------------------
#else
//----------------------------------

collision_list* get_new_collision_list()
{
    return (collision_list*)calloc(1, sizeof(collision_list));
}

void clear_collision_list(collision_list* list)
{
    for(; list->count > 0; list->count--)
        list->pairs[list->count].type = NO_COLLISION;
}
#endif