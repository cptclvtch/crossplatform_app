#include "vec.c"
#include "mesh.c"
#include "abstract_gl.c"
#define GRAPHICS_PIPELINE_INCLUDED

SDL_GLContext* context;
uint32_t triangles_drawn;

uint32_t load_texture_from_path(char* path); // returns id + 1

mesh load_mesh_from_path(char* path);
void load_mesh_to_gpu(mesh* m);
void unload_mesh_from_gpu(mesh m);

void deallocate_mesh(mesh m);

#ifdef API_IMPLEMENTATION_ONLY
uint32_t load_texture_from_path(char* path)
{
    // PRINT_FN("Loading %s... ", path);

    uint32_t texture = 0;

    //Load image at specified path
    SDL_Surface* surface = SDL_LoadBMP(path);

    if(surface == NULL)
    {
        PRINT_FN("\nUnable to load image %s!\n SDL_image Error: %s\n", path, SDL_GetError());
        SDL_FreeSurface(surface);
        return 0;
    }

    uint8_t gl_error;
    
    glGenTextures(1, &texture);
    gl_error = glGetError();
    if(gl_error != GL_NO_ERROR) printf("\nError %u generating texture\n", gl_error);
    
    glBindTexture(GL_TEXTURE_2D, texture);
    gl_error = glGetError();
    if(gl_error != GL_NO_ERROR) printf("\nError %u binding texture\n", gl_error);

    uint8_t mode = GL_RGB;
    if(surface->format->BytesPerPixel == 4)
        mode = GL_RGBA;

    // glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    printf("\nglTexImage(mode:%u, width:%u, height:%u), MAX_TEX_SIZE:%u\n",mode, surface->w, surface->h, GL_MAX_TEXTURE_SIZE);
    glTexImage2D(GL_TEXTURE_2D, 0, mode, surface->w, surface->h, 0, mode, GL_UNSIGNED_BYTE, surface->pixels);
    gl_error = glGetError();
    if(gl_error != GL_NO_ERROR) printf("\nError %u loading texture\n", gl_error);

    glGenerateMipmap(GL_TEXTURE_2D);
    SDL_FreeSurface(surface);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, NULL);

    // PRINT_FN("DONE.\n");

    return texture + 1; //returns id + 1
}

void unload_texture(uint32_t* id)
{
    glDeleteTextures(1, id);
}

mesh load_mesh_from_path(char* path)
{
    // printf("Loading %s... ", path);

    mesh loaded_mesh = (mesh){
                                vertices: NULL,
                                vertex_count: 0,
                                loaded_on_gpu: 0,
                                vertex_array: 0,
                                vertex_buffer: 0
                            };

    SDL_RWops* file = SDL_RWFromFile(path, "rb");
    if(file == NULL)
    {
        PRINT_FN("File error: %s\n", SDL_GetError());
        return loaded_mesh;
    }

    //load mesh into buffer
    uint32_t i = 0;
    SDL_RWread(file, &(loaded_mesh.vertex_count), 4, 1);
    
    if(loaded_mesh.vertex_count == 0)
    {
        PRINT_FN("Failed. No vertices found.\n");
        return loaded_mesh;
    }

    loaded_mesh.vertices = (mesh_vertex*)malloc(loaded_mesh.vertex_count*sizeof(mesh_vertex));

    SDL_RWread(file, loaded_mesh.vertices, sizeof(mesh_vertex), loaded_mesh.vertex_count);

    SDL_RWclose(file);

    // PRINT_FN("\nVertex count: %u\n", loaded_mesh.vertex_count);
    // uint32_t index = 0;
    // for(; index < loaded_mesh.vertex_count; index++)
    // {
    //     PRINT_FN("%f, %f, %f \t: %f, %f, %f\n",
    //             loaded_mesh.vertices[index].pos.x,
    //             loaded_mesh.vertices[index].pos.y,
    //             loaded_mesh.vertices[index].pos.z,
    //             // loaded_mesh.vertices[index].t_u,
    //             // loaded_mesh.vertices[index].t_v
    //             loaded_mesh.vertices[index].col.x,
    //             loaded_mesh.vertices[index].col.y,
    //             loaded_mesh.vertices[index].col.z
    //              );
    // }

    // PRINT_FN("DONE.\n");

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