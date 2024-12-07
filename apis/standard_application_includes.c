#define CHECK_ERROR(test, message) \
do \
{ \
    if((test))\
    { \
        fprintf(stderr, "%s\n", (message)); \
        exit(1); \
    } \
} while(0)

#ifdef DEFINITION_MODE
//Variables
uint8_t running = 1;

uint32_t dt = 0;
uint32_t past_reading;
uint8_t min_fps = 10, target_fps = 60, max_fps = 120;

#define RESOLUTION_SCALE 80
uint16_t width = 16*RESOLUTION_SCALE;
uint16_t height = 9*RESOLUTION_SCALE;

float font_scale = 1.0;

//Functions
void setup();

void process_input();
void render_output();

void close();
#endif

#ifdef IMPLEMENTATION_MODE
void setup()
{
    PRINT_FN("-----------SETUP----------\n");
    CHECK_ERROR(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0, SDL_GetError());
    
    window = SDL_CreateWindow(APP_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);
    CHECK_ERROR(window == NULL, SDL_GetError());
    
    start_graphics();

    past_reading = SDL_GetTicks();
    dt = 0;

    PRINT_FN("--------------------------------\n");
}

void close()
{
    #ifndef RELEASE
        #ifdef NUKLEAR_GUI
    nk_sdl_shutdown();
        #endif
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