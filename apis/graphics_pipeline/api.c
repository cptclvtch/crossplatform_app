#include "abstract_gl.c"
#ifndef GEOMETRIC_ALGEBRA
#error Please include "geometric_algebra/api.c" first
#endif
#include "mesh.c"
#include "particle.c"
#ifndef RELEASE
    #include "gizmo.c"
#endif

#ifndef API_IMPLEMENTATION_ONLY
#define GRAPHICS_PIPELINE_INCLUDED

SDL_GLContext* context;
uint32_t triangles_drawn;

uint32_t load_texture_from_path(char* path); // returns id + 1

mesh load_mesh_from_path(char* path);
void load_mesh_to_gpu(mesh* m);
void unload_mesh_from_gpu(mesh m);

void deallocate_mesh(mesh m);

//----------------------------------
#else
//----------------------------------

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
#endif