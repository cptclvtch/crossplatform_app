#include "./backend/standard_includes.c"

#define SDL_IMPLEMENTATION
#define NUKLEAR_GUI
#include "backend/backend_implementation.c"

#define APP_NAME "app"

//Macros
#define CHECK_ERROR(test, message) \
  do \
	{ \
        if((test))\
        { \
            fprintf(stderr, "%s\n", (message)); \
            exit(1); \
        } \
  } while(0)

//API Variables
#include "variables.c"

//Helper functions
void setup_SDL()
{
    CHECK_ERROR(SDL_Init(SDL_INIT_VIDEO) != 0, SDL_GetError());

    window = SDL_CreateWindow(APP_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);
    CHECK_ERROR(window == NULL, SDL_GetError());

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    CHECK_ERROR(renderer == NULL, SDL_GetError());
}

void setup_NK()
{
  /* scale the renderer output for High-DPI displays */
  {
      int render_w, render_h;
      int window_w, window_h;
      float scale_x, scale_y;
      SDL_GetRendererOutputSize(renderer, &render_w, &render_h);
      SDL_GetWindowSize(window, &window_w, &window_h);
      scale_x = (float)(render_w) / (float)(window_w);
      scale_y = (float)(render_h) / (float)(window_h);
      SDL_RenderSetScale(renderer, scale_x, scale_y);
      font_scale = scale_y;
  }

  /* GUI */
  ctx = nk_sdl_init(window, renderer);
  /* Load Fonts: if none of these are loaded a default font will be used  */
  /* Load Cursor: if you uncomment cursor loading please hide the cursor */
  {
    struct nk_font_atlas *atlas;
    struct nk_font_config config = nk_font_config(0);
    struct nk_font *font;

    /* set up the font atlas and add desired font; note that font sizes are
      * multiplied by font_scale to produce better results at higher DPIs */
    nk_sdl_font_stash_begin(&atlas);
    font = nk_font_atlas_add_default(atlas, 13 * font_scale, &config);
    /*font = nk_font_atlas_add_from_file(atlas, "../../../extra_font/DroidSans.ttf", 14 * font_scale, &config);*/
    /*font = nk_font_atlas_add_from_file(atlas, "../../../extra_font/Roboto-Regular.ttf", 16 * font_scale, &config);*/
    /*font = nk_font_atlas_add_from_file(atlas, "../../../extra_font/kenvector_future_thin.ttf", 13 * font_scale, &config);*/
    /*font = nk_font_atlas_add_from_file(atlas, "../../../extra_font/ProggyClean.ttf", 12 * font_scale, &config);*/
    /*font = nk_font_atlas_add_from_file(atlas, "../../../extra_font/ProggyTiny.ttf", 10 * font_scale, &config);*/
    /*font = nk_font_atlas_add_from_file(atlas, "../../../extra_font/Cousine-Regular.ttf", 13 * font_scale, &config);*/
    nk_sdl_font_stash_end();

    /* this hack makes the font appear to be scaled down to the desired
      * size and is only necessary when font_scale > 1 */
    font->handle.height /= font_scale;
    /*nk_style_load_all_cursors(ctx, atlas->cursors);*/
    nk_style_set_font(ctx, &font->handle);
  }
}


//API Functions
void setup()
{
    setup_SDL();

    setup_NK();
}

#define EVENT SDL_Event
uint8_t poll_event(EVENT* event)
{
  return SDL_PollEvent(event);
}

void standard_loop()
{
  bool running = true;
  EVENT event;
  while(running)
  {
    // Process events
    while(poll_event(&event))
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
}

void close()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}