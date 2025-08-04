#ifndef API_IMPLEMENTATION_ONLY
#define CHECK_ERROR(test, message) \
do \
{ \
    if((test))\
    { \
        fprintf(stderr, "%s\n", (message)); \
        exit(1); \
    } \
} while(0)

//Variables
uint8_t running = 1;

#define RESOLUTION_SCALE 80
uint16_t width = 16*RESOLUTION_SCALE;
uint16_t height = 9*RESOLUTION_SCALE;

float font_scale = 1.0;

//Functions
void setup_app();

void process_input();
void render_output();

void close_app();

//----------------------------------
#else
//----------------------------------

void setup_app()
{
    PRINT_FN("-----------SETUP----------\n");
    CHECK_ERROR(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0, SDL_GetError());
    
    window = SDL_CreateWindow(APP_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);
    CHECK_ERROR(window == NULL, SDL_GetError());
    
    float framerate = 60.0;
    SDL_DisplayMode current_display_mode;
    if(SDL_GetCurrentDisplayMode(SDL_GetWindowDisplayIndex(window), &current_display_mode) == 0)
    {
        framerate = (float)current_display_mode.refresh_rate;
    }
    frame_timing_setup(framerate);

    start_graphics();

    PRINT_FN("--------------------------------\n");
}

void close_app()
{
    #ifndef RELEASE
    nk_sdl_shutdown();
    #endif
    close_gl();
    SDL_DestroyWindow(window);
    SDL_Quit();
}

uint8_t seed_8 = 0;
uint8_t rand8()
{
    return seed_8 = 13*seed_8 + 121;
}
#endif