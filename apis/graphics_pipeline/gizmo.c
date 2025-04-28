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

void initialize_gizmos();

//----------------------------------
#else
//----------------------------------

void load_gizmo_mesh_to_gpu(gizmo_mesh* m)
{
    if(m->vertex_count == 0) return;

    //buffers
    glGenVertexArrays(1, &m->vertex_array);
    glGenBuffers(1, &m->vertex_buffer);
    glBindVertexArray(m->vertex_array);

    glBindBuffer(GL_ARRAY_BUFFER, m->vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, m->vertex_count * sizeof(gizmo_vertex), (float*)m->vertices, GL_STATIC_DRAW);

    //attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(gizmo_vertex), 0);
    
    //
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    m->loaded_on_gpu = 1;
}

void initialize_gizmos()
{
    gizmo_vertex v[2];
    v[0] = (gizmo_vertex){{0.0,0.0,0.0}};
    v[1] = (gizmo_vertex){{1.0,0.0,0.0}};
    g_mesh[0].vertices = &v;
    g_mesh[0].vertex_count = 2;

    load_gizmo_mesh_to_gpu(&g_mesh[0]);
}
#endif