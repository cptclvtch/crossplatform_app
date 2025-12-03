//Dependencies
#include "../geometric_algebra/api.c"
#include "volumes.c"

#ifndef _CONTACT_H
    #define _CONTACT_H
//Collision data
typedef struct
{
    vec3 point; // point of other volume with deepest penetration
    vec3 normal; // the direction in which the objects will push each other away
    real penetration; //longest overlap along the normal
}contact;

enum
{
    NO_COLLISION,
    // RAYCAST_COLLISION,
    CONFIRMED_COLLISION,
    POTENTIAL_COLLISION
};

/*ordered by contact generation priority
VERTEX_FACE,    EDGE_EDGE,
EDGE_FACE,      FACE_FACE,
VERTEX_EDGE,    VERTEX_VERTEX */

enum
{
    VERTEX,
    EDGE,
    FACE,

    NO_OF_CONTACT_TYPES
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

void reset_contact_data(collision_pair* p);
collision_list* create_collision_list();
void clear_collision_list(collision_list* list);
#endif //_CONTACT_H

#if defined(INCLUDE_IMPLEMENTATION) && !defined(_CONTACT_C)
#define _CONTACT_C
//----------------------------------

void reset_contact_data(collision_pair* p)
{
    if(p == NULL) return;

    p->contact_count = MAX_CONTACT_POINTS;
    do
    {
        p->contact_count--;
        p->points[p->contact_count].penetration = REAL_MAX;
    }
    while(p->contact_count > 0);
}

collision_list* create_collision_list()
{
    collision_list* new_list = (collision_list*)calloc(1, sizeof(collision_list));

    // uint32_t i = 0;
    // for(; i < new_list->count; i++)
    //     reset_contact_data(&new_list->pairs[i]);

    return new_list;
}

void clear_collision_list(collision_list* list)
{
    for(; list->count > 0; list->count--)
        list->pairs[list->count].type = NO_COLLISION;
}
#endif //_CONTACT_C