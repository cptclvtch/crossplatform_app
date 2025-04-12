#ifndef API_IMPLEMENTATION_ONLY
typedef uint8_t gizmo_index_t;
typedef struct
{
    vec3 pos;
    vec3 col;
}gizmo_vertex;

typedef struct
{
    //CPU side
    gizmo_vertex* vertices;
    gizmo_index_t visible_count;

    //GPU side
    uint8_t loaded_on_gpu;
    uint32_t vertex_array;
    uint32_t vertex_buffer;
}gizmo_mesh;

gizmo_mesh g_mesh = {0};

void delete_gizmo_mesh();

//----------------------------------
#else
//----------------------------------

void initialize_gizmos()
{
    // gizmo_mesh gizmo = (gizmo_mesh){0};
    // gizmo_vertex v[6] =
    // {
    //     {
    //         {0.0,0.0,0.0},
    //         {0.5, 0.0, 0.0}
    //     },
    //     {
    //         {1.0,0.0,0.0},
    //         {1.0,0.0,0.0}
    //     },
    //     {
    //         {0.0,0.0,0.0},
    //         {0.0, 0.5, 0.0}
    //     },
    //     {
    //         {0.0,1.0,0.0},
    //         {0.0, 1.0, 0.0}
    //     },
    //     {
    //         {0.0,0.0,0.0},
    //         {0.0, 0.0, 0.5}
    //     },
    //     {
    //         {0.0,0.0,1.0},
    //         {0.0, 0.0, 1.0}
    //     }
    // };
    // gizmo.vertices = &v;
    // gizmo.vertex_count = 6;

    // load_mesh_to_gpu(&gizmo);

    // return gizmo;
}

void delete_gizmo_mesh()
{

}
#endif