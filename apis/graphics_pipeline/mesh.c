#ifndef API_IMPLEMENTATION_ONLY
typedef uint32_t mesh_index_t;

typedef struct
{
    vec3 pos;
    vec3 normal;
    vec3 col;
    // float t_u, t_v;
}mesh_vertex;

typedef struct
{
    //CPU side
    mesh_vertex* vertices;
    mesh_index_t vertex_count;

    //GPU side
    uint8_t loaded_on_gpu;
    uint32_t vertex_array;
    uint32_t vertex_buffer;
}mesh;

//Helper functions
mesh load_mesh_from_path(char* path);
void load_mesh_to_gpu(mesh* m);

//----------------------------------
#else
//----------------------------------

mesh load_mesh_from_path(char* path)
{
    mesh loaded_mesh = (mesh){0};

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

    loaded_mesh.vertices = (mesh_vertex*)calloc(loaded_mesh.vertex_count, sizeof(mesh_vertex));

    SDL_RWread(file, loaded_mesh.vertices, sizeof(mesh_vertex), loaded_mesh.vertex_count);

    SDL_RWclose(file);

    return loaded_mesh;
}

void load_mesh_to_gpu(mesh* m)
{
    if(m->vertex_count == 0) return;

    //buffers
    glGenVertexArrays(1, &m->vertex_array);
    glGenBuffers(1, &m->vertex_buffer);
    glBindVertexArray(m->vertex_array);

    glBindBuffer(GL_ARRAY_BUFFER, m->vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, m->vertex_count * sizeof(mesh_vertex), (float*)m->vertices, GL_STATIC_DRAW);

    //attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(mesh_vertex), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(mesh_vertex), sizeof(vec3));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(mesh_vertex), sizeof(vec3)*2);
    
    //
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    m->loaded_on_gpu = 1;
}

void unload_mesh_from_gpu(mesh m)
{
    glDeleteBuffers(1, &m.vertex_buffer);
    glDeleteVertexArrays(1, &m.vertex_array);

    m.loaded_on_gpu = 0;
}

void deallocate_mesh(mesh m)
{
    if(m.loaded_on_gpu) unload_mesh_from_gpu(m);

    free(m.vertices);
    m.vertices = NULL;
}
#endif