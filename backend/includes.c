#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include <string.h>
#include <math.h>
// #include <time.h>

#ifdef __ANDROID__
    #include <android/log.h>
    #define PRINT_FN(...) __android_log_print(ANDROID_LOG_VERBOSE, "|", __VA_ARGS__)
#endif

#ifndef PRINT_FN
#define PRINT_FN(...) printf(__VA_ARGS__)
#endif

#ifdef RELEASE
#undef PRINT_FN
#define PRINT_FN(...) ;
#endif

#define ASSERT_AGAINST_MEMORY_RANGE(target, array_start, no_of_items)\
if(target < array_start) PRINT_FN("%p...[%p...%p]", target, array_start, array_start + no_of_items);\
else if(target >= array_start + no_of_items) PRINT_FN("[%p...%p]...%p", array_start, array_start + no_of_items, target);\
else PRINT_FN("[%p...%p...%p]", array_start, target, array_start + no_of_items);

//----------------------------
#include "glad/glad.c"

#ifdef SDL_IMPLEMENTATION
    #define SDL_MAIN_HANDLED
    #include "SDL2/include/SDL.h"

    SDL_Window* window;
#else

#endif

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_SDL_RENDERER_IMPLEMENTATION
#include "Nuklear/nuklear.h"
#include "nuklear_sdl_renderer.h"