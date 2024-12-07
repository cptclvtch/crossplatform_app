#define APP_NAME "app"

// #define RELEASE

#define SDL_IMPLEMENTATION
#define INCLUDE_GAME_ENGINE_API
    // #define USE_GAME_OBJECT_COMPONENTS

/*----------------------MACRO-POST-PROCESSING-------------------------*/
#ifdef _WIN32
#define EXECUTABLE APP_NAME".exe"
#else
#define EXECUTABLE APP_NAME
#endif