#ifndef INCLUDE_IMPLEMENTATION
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

void process_input(); //implemented by user
void update_app(); //implemented by user
void render_output(); //implemented by user

void close_app();

//----------------------------------
#else
//----------------------------------

void gui_setup(SDL_Window* w, SDL_Renderer* r, struct nk_context** c)
{
    /* scale the renderer output for High-DPI displays */
    {
        int render_w, render_h;
        int window_w, window_h;
        float scale_x, scale_y;
        SDL_GetRendererOutputSize(r, &render_w, &render_h);
        SDL_GetWindowSize(w, &window_w, &window_h);
        scale_x = (float)(render_w) / (float)(window_w);
        scale_y = (float)(render_h) / (float)(window_h);
        SDL_RenderSetScale(r, scale_x, scale_y);
        font_scale = scale_y;
    }

    #ifdef NK_SDL_GL3_IMPLEMENTATION
    *c = nk_sdl_init(w);
    #endif
    #ifdef NK_SDL_RENDERER_IMPLEMENTATION
    *c = nk_sdl_init(w, r);
    #endif
    
    CHECK_ERROR(*c == NULL, "Can't setup Nuklear GUI.\n");

    /* Load Fonts: if none of these are loaded a default font will be used  */
    /* Load Cursor: if you uncomment cursor loading please hide the cursor */
    {
        struct nk_font_atlas* atlas;
        struct nk_font_config config = nk_font_config(0);
        struct nk_font* font;

        /* set up the font atlas and add desired font; note that font sizes are
         * multiplied by font_scale to produce better results at higher DPIs */
        nk_sdl_font_stash_begin(&atlas);
        font = nk_font_atlas_add_default(atlas, 13 * font_scale, &config);
        /*font = nk_font_atlas_add_from_file(atlas, "../../../extra_font/DroidSans.ttf", 14 * font_scale, &config);*/
        nk_sdl_font_stash_end();

        /* this hack makes the font appear to be scaled down to the desired
         * size and is only necessary when font_scale > 1 */
        font->handle.height /= font_scale;
        /*nk_style_load_all_cursors(ctx, atlas->cursors);*/
        nk_style_set_font(*c, &font->handle);
    }
}

void setup_app()
{
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
}

void app_loop()
{
    while(running)
    {
        update_accumulator(SDL_GetTicks());

        //TODO research loop sleep() and the usefullness of loop_dt
        uint32_t loop_dt = SDL_GetTicks();
        {
            process_input();

            fixed_update(update_app);

            render_output();
        }
        loop_dt = SDL_GetTicks() - loop_dt;

        SDL_Delay((desired_ft - loop_dt)*(loop_dt < desired_ft));
    }
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

int8_t seed_8 = 0;
int8_t rand8()
{
    return seed_8 = 13*seed_8 + 121;
}
#endif