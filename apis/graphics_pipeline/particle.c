#ifndef API_IMPLEMENTATION_ONLY
typedef uint16_t particle_index_t;

typedef struct
{
    vec3 pos;
    vec3 col;
    float alpha;
}particle_vertex;

#define PARTICLE_RENDER_CHUNK_SIZE 200
typedef struct
{
    //CPU side
    particle_vertex* vertices;
    particle_index_t visible_count;

    //GPU side
    uint8_t loaded_on_gpu;
    uint32_t vertex_array;
    uint32_t vertex_buffer;
}particle_mesh;

particle_mesh p_mesh = {0};

//Helper functions
void delete_particle_mesh();

//----------------------------------
#else
//----------------------------------

void initialize_particle_mesh()
{
    if(p_mesh.loaded_on_gpu) return;

    p_mesh.vertices = (particle_vertex*)calloc(PARTICLE_RENDER_CHUNK_SIZE, sizeof(particle_vertex));
    if(p_mesh.vertices == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Not enough memory to initialize particle mesh.");
        exit(1);
    }

    //buffers
    glGenVertexArrays(1, &p_mesh.vertex_array);
    glGenBuffers(1, &p_mesh.vertex_buffer);
    glBindVertexArray(p_mesh.vertex_array);

    glBindBuffer(GL_ARRAY_BUFFER, p_mesh.vertex_buffer);
    
    //attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(particle_vertex), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(particle_vertex), sizeof(vec3));
    
    //
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    p_mesh.loaded_on_gpu = 1;
}

void delete_particle_mesh()
{
    if(!p_mesh.loaded_on_gpu) return;

    glDeleteBuffers(1, &p_mesh.vertex_buffer);
    glDeleteVertexArrays(1, &p_mesh.vertex_array);

    //TODO test this free
    free(p_mesh.vertices);
    p_mesh.vertices = NULL;
    
    p_mesh.loaded_on_gpu = 0;
}

#endif