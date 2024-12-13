#if !defined(DEFINITION_MODE) && !defined(IMPLEMENTATION_MODE)
#define DEFINITION_MODE
#endif

#ifdef DEFINITION_MODE

#ifndef uint8_t
#include <stdint.h>
#endif
#ifndef malloc
#include <stdlib.h>
#endif

#define ASSET_AT(type,x,y) ((type*)palettes[x].data)[y]
#define ASSET_NAME_AT(x,y) palettes[x].asset_names[y]

typedef void(*asset_load_function)(void*, char*);
typedef void(*asset_unload_function)();

typedef struct s_asset_palette
{
    void* data;
    uint8_t asset_size;
    uint16_t no_of_assets;
    uint16_t max_no_of_assets;
    
    char* asset_directory;
    char** asset_names;
    char* asset_file_extension;

    void (*addition_callback)(uint8_t error_code);

    asset_load_function load_asset;
    asset_unload_function unload_asset;
}asset_palette;

void    create_palette( uint8_t index,
                        uint16_t max_no_of_assets, uint8_t asset_size,
                        char* asset_directory, char* extension, void (*callback)(),
                        asset_load_function , asset_unload_function);
void    add_asset_to_palette(uint8_t index, char* name);
uint8_t load_palette(uint8_t palette_index);
uint8_t load_all_palettes();

void  unload_palette(uint8_t palette_index);
void  unload_all_palettes();
#endif

#ifdef IMPLEMENTATION_MODE
asset_palette palettes[256];

void default_callback(uint8_t error_code){}

void create_palette(uint8_t index,
                    uint16_t max_no_of_assets, uint8_t asset_size,
                    char* dir, char* extension, void (*callback)(),
                    asset_load_function load_func, asset_unload_function unload_func)
{
    if(max_no_of_assets == 0) return;

    if(palettes[index].max_no_of_assets != 0) return;

    if(asset_size)
        palettes[index].asset_size = asset_size;
    else
        palettes[index].asset_size = 1;

    palettes[index].no_of_assets = 0;
    palettes[index].max_no_of_assets = max_no_of_assets;

    if(dir)
        palettes[index].asset_directory = dir;
    else
        palettes[index].asset_directory = ".";
    
    if(extension)
        palettes[index].asset_file_extension = extension;
    else
        palettes[index].asset_file_extension = "";

    palettes[index].asset_names = (char**)malloc(palettes[index].max_no_of_assets * sizeof(char*));

    if(callback)
        palettes[index].addition_callback = callback;
    else
        palettes[index].addition_callback = default_callback;

    palettes[index].load_asset = load_func;
    palettes[index].unload_asset = unload_func;
}

void add_asset_to_palette(uint8_t index, char* name)
{
    if(!name) return;

    if(palettes[index].no_of_assets == (uint16_t)-1)
    {
        palettes[index].addition_callback(1);
        return;
    }

    if(palettes[index].no_of_assets + 1  > palettes[index].max_no_of_assets)
    {
        char** new_list = (char**)realloc(palettes[index].asset_names, palettes[index].max_no_of_assets*2);
        if(new_list)
        {
            palettes[index].asset_names = new_list;
            palettes[index].max_no_of_assets *= 2;
        }
        else
        {
            palettes[index].addition_callback(1);
            return;
        }
        palettes[index].addition_callback(2);
    }

    palettes[index].asset_names[palettes[index].no_of_assets] = name;
    palettes[index].no_of_assets++;
}

//returns 0 when successful, otherwise, returns 1
uint8_t load_palette(uint8_t palette_index)
{
    uint8_t success = 0;
    asset_palette* current_palette = &palettes[palette_index];

    if(current_palette->data != NULL) return 0;

    //alocate data and set to 0
    current_palette->data = malloc(current_palette->no_of_assets * current_palette->asset_size);
    uint32_t i = 0;
    for(; i < current_palette->asset_size * current_palette->no_of_assets; i++)
        (uint8_t*)(current_palette->data)[i] = 0;

    uint16_t index = 0;
    for(; index < current_palette->no_of_assets; index++)
    {
        //run load function
        if(current_palette->load_asset != NULL)
        {
            char path[256];
            sprintf(path, "%s/%s.%s",   current_palette->asset_directory,
                                        current_palette->asset_names[index],
                                        current_palette->asset_file_extension);
            current_palette->load_asset(&current_palette->data[current_palette->asset_size*index],
                                        path);
        }
        else
            success = 1;
    }

    return success;
}

//returns 0 when succesful, otherwise, returns last faulty palette index
uint8_t load_all_palettes()
{
    uint8_t success = 0;

    uint16_t index = 0;
    for(; index < 256; index++)
        if(palettes[index].max_no_of_assets)
        {
            success = load_palette(index);
        }
        else
            success = index;

    return success;
}

void unload_palette(uint8_t palette_index)
{
    asset_palette* current_palette = &palettes[palette_index];

    if(current_palette->unload_asset != NULL)
        current_palette->unload_asset();

    free(current_palette->data);
    current_palette->data = NULL;
}

void unload_all_palettes()
{
    uint16_t index = 0;
    for(; index < 256; index++)
        if(palettes[index].max_no_of_assets)
            unload_palette(index);
}
#endif