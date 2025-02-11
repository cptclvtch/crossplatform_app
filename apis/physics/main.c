#include "api.c"

#define APP_NAME "app"

#define SDL_IMPLEMENTATION

#define NK_IMPLEMENTATION
    // #define NK_SDL_GL3_IMPLEMENTATION
    #define NK_SDL_RENDERER_IMPLEMENTATION

#define INCLUDE_GAME_ENGINE_API
    // #define USE_GAME_OBJECT_COMPONENTS
#include "../../api.c"



void process_input()
{

}

int main()
{
    setup();

    game_loop();

    close();
}
