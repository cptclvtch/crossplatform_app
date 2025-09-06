#ifndef INCLUDE_IMPLEMENTATION
typedef uint16_t gizmo_index_t;
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

enum
{
    GIZMO_LINE,
    GIZMO_DIAGONAL_LINE,
    GIZMO_CENTERED_CUBE,
    GIZMO_OFFSET_CUBE,
    GIZMO_SPHERE,

    NO_OF_GIZMOS
};
gizmo_mesh g_mesh[NO_OF_GIZMOS];

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

gizmo_mesh load_gizmo_mesh_from_path(char* path)
{
    gizmo_mesh loaded_mesh = (gizmo_mesh){0};

    SDL_RWops* file = SDL_RWFromFile(path, "rb");
    if(file == NULL)
    {
        PRINT_FN("File error: %s\n", SDL_GetError());
        return loaded_mesh;
    }

    //load mesh into buffer
    SDL_RWread(file, &(loaded_mesh.vertex_count), 4, 1);
    
    if(loaded_mesh.vertex_count == 0)
    {
        PRINT_FN("Failed. No vertices found.\n");
        return loaded_mesh;
    }

    loaded_mesh.vertices = (gizmo_vertex*)calloc(loaded_mesh.vertex_count, sizeof(gizmo_vertex));

    SDL_RWread(file, loaded_mesh.vertices, sizeof(gizmo_vertex), loaded_mesh.vertex_count);

    SDL_RWclose(file);

    return loaded_mesh;
}

void initialize_gizmos()
{
    gizmo_vertex v[2];
    v[0] = (gizmo_vertex){{0.0,0.0,0.0}};
    v[1] = (gizmo_vertex){{1.0,0.0,0.0}};
    g_mesh[GIZMO_LINE].vertices = &v;
    g_mesh[GIZMO_LINE].vertex_count = 2;

    load_gizmo_mesh_to_gpu(&g_mesh[GIZMO_LINE]);

    v[0] = (gizmo_vertex){{0.0,0.0,0.0}};
    v[1] = (gizmo_vertex){{1.0,1.0,1.0}};
    g_mesh[GIZMO_DIAGONAL_LINE].vertices = &v;
    g_mesh[GIZMO_DIAGONAL_LINE].vertex_count = 2;

    load_gizmo_mesh_to_gpu(&g_mesh[GIZMO_DIAGONAL_LINE]);

    g_mesh[GIZMO_CENTERED_CUBE] = load_gizmo_mesh_from_path("assets/meshes/cube_centered.gizmo_mesh");
    load_gizmo_mesh_to_gpu(&g_mesh[GIZMO_CENTERED_CUBE]);
    g_mesh[GIZMO_OFFSET_CUBE] = load_gizmo_mesh_from_path("assets/meshes/cube_offset.gizmo_mesh");
    load_gizmo_mesh_to_gpu(&g_mesh[GIZMO_OFFSET_CUBE]);
    g_mesh[GIZMO_SPHERE] = load_gizmo_mesh_from_path("assets/meshes/sphere.gizmo_mesh");
    load_gizmo_mesh_to_gpu(&g_mesh[GIZMO_SPHERE]);
}
#endif