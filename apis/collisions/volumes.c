//Dependencies
#include "../data_structures/linked_list/api.c"

#ifndef _VOLUMES_H
    #define _VOLUMES_H
//Volume representation
enum
{
    SPHERE,
    BOX,
    // CYLINDER,
    // CAPSULE,
    MESH,

    NO_OF_VOLUME_TYPES,

    GROUP
};

typedef struct
{
    vec3* position;
    rotor3* orientation;

    uint8_t type;
    
    union
    {
        union
        {
            real radius;
            // struct
            // {
                vec3 dimensions;
            // }primitive;
        };

        struct
        {
            uint32_t vertex_count;
            vec3* vertices; //triangle list
        }mesh;

        linked_list_node* volumes;
    };

}collision_volume;

collision_volume* collision_group_new(collision_volume* first_item);
collision_volume* collision_group_add_new_mesh(collision_volume* g, vec3* vertices, uint32_t vertex_count, vec3* position, rotor3* orientation);
collision_volume* collision_group_add_new_primitive(collision_volume* g, uint8_t type, vec3 dimensions, vec3* position, rotor3* orientation);

void collision_group_move(collision_volume* g, vec3 translation, rotor3 rotation);
#endif //_VOLUMES_H

//----------------------------------

#if defined(INCLUDE_IMPLEMENTATION) && !defined(INCLUDE_IMPLEMENTATION)
    #define _VOLUMES_C

collision_volume* collision_group_new(collision_volume* first_item)
{
    collision_volume* new_group = (collision_volume*)calloc(1, sizeof(collision_volume));

    new_group->type = GROUP;

    new_group->volumes = add_link_before(new_group->volumes, first_item);

    return new_group;
}

collision_volume* collision_group_add_new_mesh(collision_volume* g, vec3* vertices, uint32_t vertex_count, vec3* position, rotor3* orientation)
{
    if(g == NULL || vertices == NULL || vertex_count == 0) return;

    collision_volume* new_mesh = add_link_after(g->volumes.nodes, (collision_volume*)calloc(1, sizeof(collision_volume)));

    new_mesh->position = position;
    new_mesh->orientation = orientation;
    new_mesh->type = MESH;

    new_mesh->mesh.vertices = vertices;
    new_mesh->mesh.vertex_count = vertex_count;

    // uint32_t vertex = 0;
    // for(; vertex < new_mesh->vertex_count; vertex++)
    // {
    //     v->min = vec_min(g->min, new_mesh->vertices[vertex]);
    //     v->max = vec_max(g->max, new_mesh->vertices[vertex]);
    // }

    return new_mesh;
}

collision_volume* collision_group_add_new_primitive(collision_volume* g, uint8_t type, vec3 dimensions, vec3* position, rotor3* orientation)
{
    if(g == NULL) return;

    collision_volume* new_primitive = add_link_after(g->volumes.nodes, (collision_volume*)calloc(1, sizeof(collision_volume)));

    new_primitive->position = position;
    new_primitive->orientation = orientation;

    new_primitive->type = type;
    new_primitive->dimensions = dimensions;

    // switch(type)
    // {
    //     //TODO implement primitive bounding box calculations
    //     case SPHERE:
    //        g->min = vec_min(g->min, vec_subtract(new_primitive->center_of_mass, new_primitive->corner));
    //        g->max = vec_max(g->max, vec_add(new_primitive->center_of_mass, new_primitive->corner));
    //     break;

    //     case BOX:

    //     break;

    //     case CYLINDER:
        
    //     break;

    //     case CAPSULE:
        
    //     break;
    // }

    return new_primitive;
}

void collision_group_move(collision_volume* g, vec3 translation, rotor3 rotation)
{
    if(g->type == GROUP)
    {
        ITERATE_LIST_START(g->volumes, current_volume)
            //FIXME check this
            //rotation
            rotor3* rot = ((collision_volume*)current_volume)->orientation;
            *rot = rotor_combine(*rot, rotation);
            //translation
            ((collision_volume*)current_volume)->offset = vec_add(*(g->position), translation);
        ITERATE_LIST_END(NEXT, current_volume)
    }
}
#endif