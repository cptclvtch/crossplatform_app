#ifdef DEFINITION_MODE
typedef uint32_t mesh_index_t;

typedef struct mesh_vertex
{
    vec3 pos;
    vec3 normal;
    vec3 col;
    // float t_u, t_v;
}mesh_vertex;

typedef struct s_mesh
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
mesh mesh_copy(mesh m);

void move_mesh(mesh m, vec3 amount);
void rotate_mesh(mesh m, vec3 amount);
void scale_mesh(mesh m, vec3 amount);
#endif

#ifdef IMPLEMENTATION_MODE
mesh mesh_copy(mesh m)
{
    mesh to_return = {};
    
    to_return.vertex_count = m.vertex_count;
    size_t memory_amount = sizeof(mesh_vertex) * to_return.vertex_count;
    to_return.vertices = (mesh_vertex*)calloc(1, memory_amount);
    if(to_return.vertices == NULL) return;

    memcpy((void*)to_return.vertices, (void*)m.vertices, memory_amount);

    return to_return;
}

void move_mesh(mesh m, vec3 amount)
{
    if(m.vertices == NULL) return;

    mesh_index_t iterator = 0;
    for(; iterator < m.vertex_count; iterator++)
        m.vertices[iterator].pos = vec_add(m.vertices[iterator].pos, amount);
}

void rotate_mesh(mesh m, vec3 amount)
{
    if(m.vertices == NULL) return;

    mesh_index_t iterator = 0;
    for(; iterator < m.vertex_count; iterator++)
        m.vertices[iterator].pos = vec_rotate(m.vertices[iterator].pos, amount);
}

void scale_mesh(mesh m, vec3 amount)
{
    if(m.vertices == NULL) return;

    mesh_index_t iterator = 0;
    for(; iterator < m.vertex_count; iterator++)
        m.vertices[iterator].pos = vec_multiply(m.vertices[iterator].pos, amount);
}
#endif