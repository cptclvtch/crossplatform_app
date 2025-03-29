#define ERROR_LOG_FUNC(string) SDL_LogError(SDL_LOG_CATEGORY_ERROR, string)
#define ASSERT_FUNC SDL_assert
#include "linked_list/api.c"
#include "frame_timing/api.c"
#include "standard_application_includes.c"

#include "geometric_algebra/api.c"

#include "graphics_pipeline/api.c"
#include "asset_manager/api.c"

// #include "AI/GOAP/api.c"

#ifdef INCLUDE_PHYSICS_API
#include "physics/api.c"
#endif

#include "input_system/api.c"

#ifdef INCLUDE_GAME_ENGINE_API
#include "game_engine/api.c"
#endif