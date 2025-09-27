//Boilerplate
#define DEV_WINDOW_NAME APP_NAME
SDL_Window* dev_window;
uint16_t dev_width = 600, dev_height = 600;
SDL_Renderer* dev_renderer;
struct nk_context* dev_ctx;

//--------------------------------
void dev_window_setup()
{
    dev_window = SDL_CreateWindow(DEV_WINDOW_NAME, 0, 25, dev_width, dev_height, SDL_WINDOW_OPENGL);
    CHECK_ERROR(dev_window == NULL, SDL_GetError());

    dev_renderer = SDL_CreateRenderer(dev_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    CHECK_ERROR(dev_renderer == NULL, SDL_GetError());

    /* scale the renderer output for High-DPI displays */
    {
        int render_w, render_h;
        int window_w, window_h;
        float scale_x, scale_y;
        SDL_GetRendererOutputSize(dev_renderer, &render_w, &render_h);
        SDL_GetWindowSize(dev_window, &window_w, &window_h);
        scale_x = (float)(render_w) / (float)(window_w);
        scale_y = (float)(render_h) / (float)(window_h);
        SDL_RenderSetScale(dev_renderer, scale_x, scale_y);
        font_scale = scale_y;
    }

    #ifdef NK_SDL_GL3_IMPLEMENTATION
    dev_ctx = nk_sdl_init(dev_window);
    #endif
    #ifdef NK_SDL_RENDERER_IMPLEMENTATION
    dev_ctx = nk_sdl_init(dev_window, dev_renderer);
    #endif
    
    CHECK_ERROR(dev_ctx == NULL, "Can't setup Nuklear GUI.\n");

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
        nk_style_set_font(dev_ctx, &font->handle);
    }

    initialize_gizmos();
}

void bvh_print(binary_tree* node, uint8_t tab_level);
void bvh_print(binary_tree* node, uint8_t tab_level)
{
    if(node == NULL) return;

    PRINT_FN("\n");
    uint8_t i = 0;
    for(; i < tab_level; i++)
        PRINT_FN("\t");

    aabb box = ((bvh_data*)node->data)->box;
    PRINT_FN("[%f,%f,%f] [%f,%f,%f]", box.center.x, box.center.y, box.center.z, box.half_size.x, box.half_size.y, box.half_size.z);

    bvh_print(node->child[0], tab_level + 1);
    bvh_print(node->child[1], tab_level + 1);
}

enum{Broad, Narrow, NO_OF_DEV_TABS};
uint8_t current_tab = Narrow;
void dev_window_loop()
{
    char text[255];
    #ifdef NK_SDL_RENDERER_IMPLEMENTATION
    SDL_RenderClear(dev_renderer);
    #endif
    #ifdef NK_SDL_GL3_IMPLEMENTATION
    glClear(GL_COLOR_BUFFER_BIT);
    #endif

    if (nk_begin(dev_ctx, "Dev", nk_rect(0, 0, dev_width, dev_height), 0))
    {
        char* tab_strings[NO_OF_DEV_TABS] = {"Broad Phase","Narrow Phase"};
        uint8_t tab_height = 30;
        
        nk_layout_row_static(dev_ctx, tab_height, dev_width/NO_OF_DEV_TABS-9, NO_OF_DEV_TABS);
        uint8_t i;
        for(i = 0; i < NO_OF_DEV_TABS; i++)
            if(nk_button_label(dev_ctx, tab_strings[i])) current_tab = i;

        nk_label(dev_ctx, "", NK_TEXT_CENTERED);
        
        switch(current_tab)
        {
            case Broad:
            {
                nk_layout_row_dynamic(dev_ctx, 20, 1);
            }
            break;

            case Narrow:
            {
                uint8_t i;

                nk_layout_row_dynamic(dev_ctx, 20, 2);
                nk_property_float(dev_ctx, "Animation speed:", 0.0, &anim_speed, 1.0, .05, .01);
                if(nk_button_label(dev_ctx, debug_paused == 0 ? "|>":"||")) debug_paused = 1 - debug_paused;

                char* volume_strings[NO_OF_VOLUME_TYPES] = {"Sphere", "Box", "Mesh"};
                
                for(i = 0; i < 2; i++)
                {
                    nk_layout_row_dynamic(dev_ctx, 20, 2);
                    nk_label(dev_ctx, "", 0); nk_label(dev_ctx, "", 0);
                    sprintf(text, "Volume %c", 'A' + i);
                    nk_label(dev_ctx, text, NK_TEXT_LEFT);
                    collision_volume* v = p.members[i];
                    v->type = nk_combo(dev_ctx, volume_strings, NO_OF_VOLUME_TYPES-1, v->type, 30, nk_vec2(200,200));

                    nk_layout_row_dynamic(dev_ctx, 20, 4);
                    nk_label(dev_ctx, "Position:", NK_TEXT_LEFT);
                    nk_property_float(dev_ctx, "X:", -10.0, &v->position->x, 10.0, .05, .01);
                    nk_property_float(dev_ctx, "Y:", -10.0, &v->position->y, 10.0, .05, .01);
                    nk_property_float(dev_ctx, "Z:", -10.0, &v->position->z, 10.0, .05, .01);

                    nk_layout_row_dynamic(dev_ctx, 20, 5);
                    nk_label(dev_ctx, "Orientation:", NK_TEXT_LEFT);
                    nk_property_float(dev_ctx, "T.X:", 0.0, &v->orientation->T.x, 1.0, .05, .01);
                    nk_property_float(dev_ctx, "T.Y:", 0.0, &v->orientation->T.y, 1.0, .05, .01);
                    nk_property_float(dev_ctx, "T.Z:", 0.0, &v->orientation->T.z, 1.0, .05, .01);
                    nk_property_float(dev_ctx, "II: ", 0.0, &v->orientation->II , 1.0, .05, .01);
                    *v->orientation = rotor_normalize(*v->orientation);
                    
                    nk_layout_row_dynamic(dev_ctx, 20, 4);                    
                    nk_label(dev_ctx, "Size:", NK_TEXT_LEFT);
                    nk_property_float(dev_ctx, "X:", 0.0, &v->dimensions.x, 10.0, .05, .01);
                    nk_property_float(dev_ctx, "Y:", 0.0, &v->dimensions.y, 10.0, .05, .01);
                    nk_property_float(dev_ctx, "Z:", 0.0, &v->dimensions.z, 10.0, .05, .01);
                }

                nk_layout_row_dynamic(dev_ctx, 20, 1);
                nk_label(dev_ctx, "", 0);
                if(p.type == NO_COLLISION)
                    sprintf(text, "No collisions");
                else
                    sprintf(text, "Interpenetrations (%u)", p.contact_count);

                nk_label(dev_ctx, text, NK_TEXT_LEFT);
                for(i = 0; i < p.contact_count; i++)
                {
                    sprintf(text, "  %f", p.points[i].penetration);
                    nk_label(dev_ctx, text, NK_TEXT_LEFT);
                }
            }
            break;
        }
    }
    nk_end(dev_ctx);

    #ifdef NK_SDL_RENDERER_IMPLEMENTATION
    nk_sdl_render(NK_ANTI_ALIASING_ON);
    SDL_RenderPresent(dev_renderer);
    #endif
    #ifdef NK_SDL_GL3_IMPLEMENTATION
    nk_sdl_render(NK_ANTI_ALIASING_ON, NK_MAX_VERTEX_MEMORY, NK_MAX_ELEMENT_MEMORY);
    SDL_GL_SwapWindow(dev_window);
    #endif
}