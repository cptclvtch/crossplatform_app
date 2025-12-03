#include "app_configuration.c"

#include <stdlib.h>
#include STRINGIFY(CROSSPLATFORM_APP_FOLDER/script_helper.c)

int main()
{
    #define START ""
    #ifdef _WIN32
        #undef START
        #define START "start"
    #endif

    #ifndef RELEASE
    #undef START
    #define START "gdb -q"
    #endif
    
    set_cwd("./build");

    char executable_name[256] = EXECUTABLE;
    replace_characters(executable_name, ' ', '_');
    
    char command[256];
    sprintf(command, START " %s", executable_name);
    system(command);

    set_cwd("..");
}