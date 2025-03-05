#include "../unit_testing.c"
#include <stdio.h>
#include <string.h>

#include "api.c"
#define API_IMPLEMENTATION_ONLY
#include "api.c"

typedef struct s_test
{
    int a;
}test;

int read_int(char* path)
{
    return 2;
}

void load_int(void* i, char* path)
{
    // printf("%s\n", path);
    *(int*)i = read_int(path);
}

void unload_int()
{}

void callback(uint8_t error_code)
{}

int main()
{
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    //test every function
    #define TITLE "palette creation"
    POST_TITLE
    #define SUBTITLE "Creating palettes - Zero assets"
    create_palette(0,0,sizeof(char), ".", "txt", &callback, &load_int, &unload_int);
    VERIFY_SINGLE_VALUE(palettes[0].max_no_of_assets, ==, 0)
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "Creating palettes - Best Case Scenario"
    create_palette(0,1,1, ".","txt", &callback, &load_int, &unload_int);
    VERIFY_SINGLE_VALUE(palettes[0].max_no_of_assets, ==, 1)
    COLLECT_FINDINGS
    
    #undef SUBTITLE
    #define SUBTITLE "Creating palettes - Creating existing palette"
    create_palette(0,2,1, ".","txt", &callback, &load_int, &unload_int);
    VERIFY_SINGLE_VALUE(palettes[0].max_no_of_assets, !=, 2)
    VERIFY_SINGLE_VALUE(palettes[0].max_no_of_assets, !=, 0)
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "Creating palettes - Zero asset size"
    create_palette(1,1,0, ".","txt", &callback, &load_int, &unload_int);
    VERIFY_SINGLE_VALUE(palettes[1].asset_size, ==, 1)
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "Creating palettes - NULL directory"
    create_palette(2,1,1, NULL,"txt", &callback, &load_int, &unload_int);
    SET_CRITICAL
    VERIFY_SINGLE_VALUE(palettes[2].asset_directory, !=, NULL)
    RESET_CRITICAL
    VERIFY_SINGLE_VALUE(strcmp(palettes[2].asset_directory, "."), ==, 0)
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "Creating palettes - NULL extension"
    create_palette(3,1,1, ".", NULL, &callback, &load_int, &unload_int);
    SET_CRITICAL
    VERIFY_SINGLE_VALUE(palettes[3].asset_file_extension, !=, NULL)
    RESET_CRITICAL
    VERIFY_SINGLE_VALUE(strcmp(palettes[3].asset_file_extension, ""), ==, 0)
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "Creating palettes - NULL callback"
    create_palette(4,1,1, ".", "txt", NULL, &load_int, &unload_int);
    VERIFY_SINGLE_VALUE(palettes[4].addition_callback, ==, default_callback)
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "Creating palettes - NULL load function"
    create_palette(5,1,1, ".","txt", &callback, NULL, &unload_int);
    VERIFY_SINGLE_VALUE(palettes[5].load_asset, ==, NULL)
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "Creating palettes - NULL unload function"
    create_palette(6,1,1, ".","txt", &callback, &load_int, NULL);
    VERIFY_SINGLE_VALUE(palettes[6].unload_asset, ==, NULL)
    COLLECT_FINDINGS

    ADD_SEPARATOR

    #undef TITLE
    #define TITLE "asset addition"
    POST_TITLE
    #undef SUBTITLE
    #define SUBTITLE "Adding asset - Best Case Scenario"
    add_asset_to_palette(0, "asset 1");
    VERIFY_SINGLE_VALUE(palettes[0].no_of_assets, ==, 1)
    VERIFY_SINGLE_VALUE(strcmp(palettes[0].asset_names[palettes[0].no_of_assets - 1], "asset 1"), ==, 0)
    COLLECT_FINDINGS
    
    #undef SUBTITLE
    #define SUBTITLE "Adding asset - NULL name"
    add_asset_to_palette(1, NULL);
    VERIFY_SINGLE_VALUE(palettes[1].no_of_assets, ==, 0)
    COLLECT_FINDINGS

    ADD_SEPARATOR

    #undef TITLE
    #define TITLE "palette loading"
    POST_TITLE
    #undef SUBTITLE
    #define SUBTITLE "Loading one palette - Best Case Scenario"
    result = load_palette(0);
    VERIFY_SINGLE_VALUE(result, ==, 0)
    COLLECT_FINDINGS
    
    #undef SUBTITLE
    #define SUBTITLE "Loading one palette - Repeated use"
    result = load_palette(0);
    VERIFY_SINGLE_VALUE(result, ==, 0)
    COLLECT_FINDINGS
    
    #undef SUBTITLE
    #define SUBTITLE "Loading one palette - NULL load function"
    // palettes[1].load_func = NULL;
    result = load_palette(5);
    VERIFY_SINGLE_VALUE(result, ==, 0)
    COLLECT_FINDINGS
    // palettes[1].load_func = &load_int;

    ADD_SEPARATOR
    #undef TITLE
    #define TITLE "unloading palettes"
    POST_TITLE
    #undef SUBTITLE
    #define SUBTITLE "Unloading one palette - Best Case Scenario"
    unload_palette(0);
    VERIFY_SINGLE_VALUE(palettes[0].data, ==, NULL)
    COLLECT_FINDINGS

    #undef SUBTITLE
    #define SUBTITLE "Unloading one palette - NULL unload function"
    // palettes[1].unload_func = NULL;
    unload_palette(6);
    VERIFY_SINGLE_VALUE(palettes[6].data, ==, NULL)
    COLLECT_FINDINGS
    // palettes[1].unload_func = &unload_int;

    #undef SUBTITLE
    #define SUBTITLE "Unloading all palettes - Best Case Scenario"
    unload_all_palettes();
    VERIFY_ARRAY_OF_VALUES(palettes[test_iterator].data, ==, NULL, 255)
    COLLECT_FINDINGS

    ADD_SEPARATOR
    #undef TITLE
    #define TITLE "loading all assets"
    POST_TITLE
    #undef SUBTITLE
    #define SUBTITLE "Loading all palettes - Best Case Scenario"

    load_all_palettes();
    VERIFY_SINGLE_VALUE(ASSET_AT(test, 0,0).a, == , 2)
    COLLECT_FINDINGS

    DEBRIEF
}