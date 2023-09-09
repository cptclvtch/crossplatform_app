#include "./backend/standard_includes.c"
#include "app_settings.c"

#define SDL_IMPLEMENTATION
#define SDL_MAIN_HANDLED
#include "backend/backend_implementation.c"

// Utility macros
#define CHECK_ERROR(test, message) \
    do \
	{ \
        if((test)) { \
            fprintf(stderr, "%s\n", (message)); \
            exit(1); \
        } \
    } while(0)
    
// Window dimensions
static const int width = 800;
static const int height = 600;

int main(int argc, char **argv)
{
    CHECK_ERROR(SDL_Init(SDL_INIT_VIDEO) != 0, SDL_GetError());

    SDL_Window *window = SDL_CreateWindow(APP_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);
    CHECK_ERROR(window == NULL, SDL_GetError());

    context = SDL_GL_CreateContext(window);
    CHECK_ERROR(context == NULL, SDL_GetError());

    initGL();

    bool running = true;
    SDL_Event event;
    while(running)
	{
        // Process events
        while(SDL_PollEvent(&event))
		{
            if(event.type == SDL_QUIT)
			{
                running = false;
            }
			else
			if(event.type == SDL_KEYDOWN)
			{
                const char *key = SDL_GetKeyName(event.key.keysym.sym);
				if(strcmp(key, "Q") == 0)
				{
                    running = false;
                }                    
            }
        }

        // Clear screen

        // Draw

        // Show what was drawn
    }

    // Release resources
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}