#ifndef API_IMPLEMENTATION_ONLY
#ifndef MAX_VSYNC_MULTIPLES
#error Please include frame_timing/api.c
#endif
#ifndef GRAPHICS_PIPELINE_INCLUDED
#error Please include graphics_pipeline/api.c
#endif
//Variables
//Functions
void game_setup();
void update_game();

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

    #ifndef RELEASE
    initialize_gizmos();
    #endif
}

void game_loop()
{
    while(running)
    {
        update_accumulator(SDL_GetTicks());

        //TODO research loop sleep() and the usefullness of loop_dt
        uint32_t loop_dt = SDL_GetTicks();
        {
            process_input();

            fixed_update(update_game);

            render_output();
        }
        loop_dt = SDL_GetTicks() - loop_dt;

        SDL_Delay((desired_ft - loop_dt)*(loop_dt < desired_ft));
    }
}
#endif