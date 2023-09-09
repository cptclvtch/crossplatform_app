#ifdef SDL_IMPLEMENTATION
    #define SDL_MAIN_HANDLED
    #include "SDL2/include/SDL.h"

    SDL_Window* window;
    SDL_Renderer* renderer;
#else

#endif

#ifdef NUKLEAR_GUI
    #define NK_INCLUDE_FIXED_TYPES
    #define NK_INCLUDE_STANDARD_IO
    #define NK_INCLUDE_STANDARD_VARARGS
    #define NK_INCLUDE_DEFAULT_ALLOCATOR
    #define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
    #define NK_INCLUDE_FONT_BAKING
    #define NK_INCLUDE_DEFAULT_FONT
    #define NK_IMPLEMENTATION
    #ifdef SDL_IMPLEMENTATION
        #define NK_SDL_RENDERER_IMPLEMENTATION
    #endif

    #include "Nuklear/nuklear.h"
    
    #ifdef SDL_IMPLEMENTATION
        #include "nuklear_sdl_renderer.h"
    #endif

    struct nk_context* ctx;
    struct nk_colorf bg;
    
    float font_scale = 1;
#endif