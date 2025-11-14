//Dependencies
#include "aabb.c"
#include <stdlib.h>

#ifndef _VOLUMES_H
    #define _VOLUMES_H
//Volume representation
enum
{
    SPHERE,
    BOX,
    // CYLINDER,
    MESH,

    NO_OF_VOLUME_TYPES,

    GROUP
};

//mesh triangle format
enum
{
    COLLISION_TRI_ORIGIN,
    COLLISION_TRI_EDGE_1,
    COLLISION_TRI_EDGE_2,
    COLLISION_TRI_NORMAL,

    COLLISION_TRI_FORMAT_SIZE
};
#define COLLISION_GET_TRI_DATA(volume_pointer, t, m) (volume_pointer->mesh.data[t*COLLISION_TRI_FORMAT_SIZE + m])

typedef struct collision_volume
{
    vec3* position;
    rotor3* orientation;
    uint8_t _is_domestic; //used for tracking domestic vs foreign pointers
    aabb _aabb_cache;

    uint8_t type;
    
    union
    {
        union
        {
            real radius;
            vec3 dimensions;
            vec3 half_size;
        };

        struct
        {
            //face normal calculated during mesh loading?
            uint32_t face_count;
            vec3* data;
        }mesh;

        struct collision_volume** volumes; //recursive structure
        uint8_t group_count;
    };

}collision_volume;

collision_volume* create_primitive_collision_volume(uint8_t type, vec3 dimensions, vec3* position, rotor3* orientation);
collision_volume* create_mesh_collision_volume(uint32_t face_count, vec3* data, vec3* position, rotor3* orientation);

void group_collision_volumes(collision_volume* a, collision_volume* b);

#define BASIC_CALCULATION 0
#define KEEP_MAX 1
#define NO_ROTATION 2
#define KEEP_CACHE 4
aabb calculate_aabb_from_volume(collision_volume* v, uint8_t mode);

void free_collision_volume(collision_volume* v);
#endif //_VOLUMES_H

//----------------------------------

#if defined(INCLUDE_IMPLEMENTATION) && !defined(_VOLUMES_C)
    #define _VOLUMES_C

collision_volume* create_primitive_collision_volume(uint8_t type, vec3 dimensions, vec3* position, rotor3* orientation)
{
    collision_volume* new_primitive = (collision_volume*)calloc(1, sizeof(collision_volume));

    new_primitive->position = position;
    new_primitive->orientation = orientation;

    if(type >= MESH) type = MESH-1;
    new_primitive->type = type;
    new_primitive->dimensions = dimensions;

    return new_primitive;
}

collision_volume* create_mesh_collision_volume(uint32_t face_count, vec3* data, vec3* position, rotor3* orientation)
{
    if(data == NULL || face_count == 0) return NULL;

    collision_volume* new_mesh = (collision_volume*)calloc(1, sizeof(collision_volume));

    new_mesh->position = position;
    new_mesh->orientation = orientation;
    new_mesh->type = MESH;

    new_mesh->mesh.data = data;
    new_mesh->mesh.face_count = face_count;

    return new_mesh;
}

#define POSITION_POINTER 1
#define ORIENTATION_POINTER 2

void _apply_domestic_pointer(collision_volume* v, uint8_t type, vec3 new_vec, rotor3 new_rot)
{
    switch(type)
    {
        case POSITION_POINTER:
        if(POSITION_POINTER & v->_is_domestic) free(v->position);
        v->position = (vec3*)calloc(1,sizeof(vec3));
        *(v->position) = new_vec;
        break;

        case ORIENTATION_POINTER:
        if(ORIENTATION_POINTER & v->_is_domestic) free(v->orientation);
        v->orientation = (rotor3*)calloc(1,sizeof(rotor3));
        *(v->orientation) = new_rot;
        break;

        return;
    }
    v->_is_domestic |= type;
}

void group_collision_volumes(collision_volume* a, collision_volume* b)
{
    if(a == NULL || b == NULL) return;

    if(a->type != GROUP)
    {
        collision_volume* volume_copy = (collision_volume*)calloc(1, sizeof(collision_volume));
        *volume_copy = *a; //TODO check that this works

        _apply_domestic_pointer(volume_copy, POSITION_POINTER, (vec3){0}, IDENTITY_ROTOR);
        _apply_domestic_pointer(volume_copy, ORIENTATION_POINTER, (vec3){0}, IDENTITY_ROTOR);
        
        a->type = GROUP;
        a->group_count = 1;
        a->volumes = (collision_volume**)calloc(1, sizeof(collision_volume*));
        a->volumes[0] = volume_copy;
    }
    
    _apply_domestic_pointer(b, POSITION_POINTER, vec_subtract(*b->position, *a->position), IDENTITY_ROTOR);
    _apply_domestic_pointer(b, ORIENTATION_POINTER, (vec3){0}, rotor_combine(*b->orientation, rotor_reverse(*a->orientation)));

    //resize group
    collision_volume** new_group = realloc(a->volumes, (a->group_count+1)*sizeof(collision_volume*));
    if(new_group)
    {
        a->volumes = new_group;
        a->volumes[a->group_count++] = b;
    }
}

aabb calculate_aabb_from_volume(collision_volume* v, uint8_t mode)
{
    aabb result = (aabb){0};
    if(v == NULL) return result;

    switch(v->type)
    {
        case SPHERE:
        result = (aabb){*(v->position), (vec3){v->radius, v->radius, v->radius}};
        break;

        case BOX:
        result = (aabb){*(v->position), v->half_size};
        break;

        case MESH:
        {
            vec3 min = (vec3){0};
            vec3 max = (vec3){0};
            uint32_t t = 0;
            for(; t < v->mesh.face_count; t++)
            {
                vec3 vertex[3];
                vertex[0] = COLLISION_GET_TRI_DATA(v, t, COLLISION_TRI_ORIGIN);
                vertex[1] = vec_add(COLLISION_GET_TRI_DATA(v, t, COLLISION_TRI_EDGE_1), vertex[0]);
                vertex[1] = vec_add(COLLISION_GET_TRI_DATA(v, t, COLLISION_TRI_EDGE_2), vertex[0]);
                uint8_t i = 0;
                for(;i < 3; i++)
                {
                    max = vec_max(max, vertex[i]);
                    min = vec_min(min, vertex[i]);
                }
            }
            max = vec_scalar_divide(max, 2);
            min = vec_scalar_divide(min, 2);
            result = aabb_union((aabb){min, min}, (aabb){max,max});
            break;
        }

        case GROUP:
        {
            uint8_t i = 0;
            for(; i < v->group_count; i++)
                result = aabb_union(result, calculate_aabb_from_volume(v->volumes[i], mode));
            break;
        }
    }
    
    //FIXME wrong calculation
    if(!(mode & NO_ROTATION)) result.half_size = vec_rotate_w_rotor(result.half_size, *(v->orientation));

    if(mode & KEEP_MAX) result = aabb_union(v->_aabb_cache, result);

    if(mode & KEEP_CACHE) v->_aabb_cache = (aabb){vec_subtract(result.center, *(v->position)), result.half_size};

    return result;
}

void free_collision_volume(collision_volume* v)
{
    if(v == NULL) return;

    if(v->type == GROUP)
    {
        uint8_t i = 0;
        for(; i < v->group_count; i++)
            free_collision_volume(v->volumes[i]);

        free(v->volumes);
    }

    if(POSITION_POINTER & v->_is_domestic) free(v->position);
    if(ORIENTATION_POINTER & v->_is_domestic) free(v->orientation);

    free(v);
}
#undef POSITION_POINTER
#undef ORIENTATION_POINTER
#endif