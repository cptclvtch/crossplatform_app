#ifndef APP_NAME
#error Make sure you have an "app_configuration.c" file in your project folder. A default one can be found in the "setup" folder.
#endif
#include "backend/includes.c"

#define DEFINITION_MODE
#include "apis/includes.c"
#undef DEFINITION_MODE

#define IMPLEMENTATION_MODE
#include "apis/includes.c"
#undef IMPLEMENTATION_MODE