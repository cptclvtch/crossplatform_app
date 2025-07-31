#ifndef GEOMETRIC_ALGEBRA
#error Please include "geometric_algebra/api.c" first
#endif

#include "contacts.c"
#include "aabb.c"
#include "bvh.c"

#ifndef API_IMPLEMENTATION_ONLY
#define COLLISIONS
//Volume representation
typedef struct
{
    uint32_t vertex_count;
    vec3* vertices; //triangle list
    vec3 inverse_inertia;
}collision_mesh;

enum
{
    SPHERE,
    BOX,
    CYLINDER,
    CAPSULE,

    NO_OF_PRIMITIVE_TYPES
};

typedef struct
{
    uint8_t type;
    vec3 center_of_mass;
    vec3 corner;
    vec3 inverse_inertia;
}collision_primitive;

typedef struct
{
    linked_list meshes;
    // collision_mesh* meshes;
    // uint8_t no_of_meshes;
    linked_list primitives;
    // collision_primitive* primitives;
    // uint8_t no_of_primitives;

    vec3 center_of_mass;
    vec3 min, max; //axis aligned bounding box
}collision_volume;

void collision_volume_add_mesh(collision_volume* v, vec3* vertices, uint32_t vertex_count);
void collision_volume_add_primitive(collision_volume* v, uint8_t type, vec3 center_of_mass, vec3 corner);
//void load_collision_volume(collision_volume* v, char* path);

// uint8_t point_2_mesh_collision(vec3 p, collision_mesh m, collision_data* data);

//----------------------------------
#else
//----------------------------------

void collision_volume_add_mesh(collision_volume* v, vec3* vertices, uint32_t vertex_count)
{
    if(v == NULL || vertices == NULL || vertex_count == 0) return;

    collision_mesh* new_mesh = add_link_after(v->meshes.nodes, (collision_mesh*)calloc(1, sizeof(collision_mesh)));

    new_mesh->vertices = vertices;
    new_mesh->vertex_count = vertex_count;

    uint32_t vertex = 0;
    for(; vertex < new_mesh->vertex_count; vertex++)
    {
        v->min = vec_min(v->min, new_mesh->vertices[vertex]);
        v->max = vec_max(v->max, new_mesh->vertices[vertex]);
    }
}

// void collision_volume_add_mesh(collision_volume* v, char* path)
// {
//     if(v == NULL) return;

//     collision_mesh* new_mesh = add_link_after(v->meshes.nodes, (collision_mesh*)calloc(1, sizeof(collision_mesh)));

//     //read file
//     SDL_RWops* file = SDL_RWFromFile(path, "rb");
//     if(file == NULL)
//     {
//         PRINT_FN("File error: %s\n", SDL_GetError());
//         return;
//     }

//     //load mesh into buffer
//     SDL_RWread(file, &(new_mesh->vertex_count), 4, 1);
    
//     if(new_mesh->vertex_count == 0)
//     {
//         PRINT_FN("Failed. No vertices found.\n");
//         return;
//     }

//     new_mesh->vertices = (vec3*)calloc(new_mesh->vertex_count, sizeof(vec3));

//     mesh_vertex input_vertex = {0};
//     uint32_t vertex = 0;
//     for(; vertex < new_mesh->vertex_count; vertex++)
//     {
//         SDL_RWread(file, &input_vertex, sizeof(mesh_vertex), 1);

//         new_mesh->vertices[vertex] = input_vertex.pos;

//         v->min = vec_min(v->min, input_vertex.pos);
//         v->max = vec_max(v->max, input_vertex.pos);
//     }

//     SDL_RWclose(file);
// }

void collision_volume_add_primitive(collision_volume* v, uint8_t type, vec3 center_of_mass, vec3 corner)
{
    if(v == NULL) return;

    collision_primitive* new_primitive = add_link_after(v->primitives.nodes, (collision_primitive*)calloc(1, sizeof(collision_primitive)));

    new_primitive->type = type;
    new_primitive->center_of_mass = center_of_mass;
    new_primitive->corner = corner;

    switch(type)
    {
        //TODO implement primitive bounding box calculations
        case SPHERE:
            v->min = vec_min(v->min, vec_subtract(new_primitive->center_of_mass, new_primitive->corner));
            v->max = vec_max(v->max, vec_add(new_primitive->center_of_mass, new_primitive->corner));
        break;

        case BOX:

        break;

        case CYLINDER:
        
        break;

        case CAPSULE:
        
        break;
    }
}

//void load_collision_volume(collision_volume* v, char* path)
//{}

// uint8_t point_2_mesh_collision(vec3 p, collision_mesh m, collision_data* data)
// {
//     //TODO optimize this
//     //TODO octree implementation?

//     // iterate over triangles
//     uint32_t t = 0;
//     for(;t < m.vertex_count; t += 3)
//     {
//         // find closest triangle
//         vec3 local = vec_subtract(p, m.vertices[t]);

        
//     }

//     // find perpendicular component of the point on the triangle
//     // if(vec_dot_product(component,normal) > 0) return 1;
//     return 1;
// }
#endif