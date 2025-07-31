#ifndef API_IMPLEMENTATION_ONLY
//Collision data
enum
{
    NO_COLLISION,
    RAYCAST_COLLISION,
    CONFIRMED_COLLISION,
    POTENTIAL_COLLISION
};
typedef struct
{
    uint8_t type;
    void* data[2];

    vec3 normal;
    real penetration;
}collision_data;

//Collision list
#define COLLISION_CHUNK_SIZE 100
typedef struct
{
    collision_data pairs[COLLISION_CHUNK_SIZE];
    uint32_t last_index;
}collision_list;

void clear_collision_list(collision_list* list);

//----------------------------------
#else
//----------------------------------


void clear_collision_list(collision_list* list)
{
    uint8_t i = 0;
    for(; i < COLLISION_CHUNK_SIZE; i++)
        list->pairs[i].type = NO_COLLISION;

    list->last_index = 0;
}
#endif