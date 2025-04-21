#ifndef API_IMPLEMENTATION_ONLY
typedef uint8_t gizmo_index_t;
typedef struct
{
    vec3 pos;
}gizmo_vertex;

typedef struct
{
    //CPU side
    gizmo_vertex* vertices;
    gizmo_index_t vertex_count;

    //GPU side
    uint8_t loaded_on_gpu;
    uint32_t vertex_array;
    uint32_t vertex_buffer;
}gizmo_mesh;

gizmo_mesh g_mesh[3];

void delete_gizmo_mesh();

//----------------------------------
#else
//----------------------------------

void initialize_gizmos()
{
    gizmo_vertex v[2];
    v[0] = (gizmo_vertex){{0.0,0.0,0.0}};
    v[1] = (gizmo_vertex){{1.0,0.0,0.0}};
    g_mesh[0].vertices = &v;
    g_mesh[0].vertex_count = 2;

    load_mesh_to_gpu(&g_mesh[0]);

    v[1] = (gizmo_vertex){{0.0,1.0,0.0}};
    g_mesh[1].vertices = &v;
    g_mesh[1].vertex_count = 2;

    load_mesh_to_gpu(&g_mesh[1]);

    v[1] = (gizmo_vertex){{0.0,0.0,1.0}};
    g_mesh[2].vertices = &v;
    g_mesh[2].vertex_count = 2;

    load_mesh_to_gpu(&g_mesh[2]);
}

void delete_gizmo_mesh()
{

}
#endif