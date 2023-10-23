#include <stdlib.h>

#include "crossplatform_app/script_helper.c"

int main()
{
    set_cwd("crossplatform_app");
    system("git pull");
    system("git submodule update --remote");

    //Recursively copy the setup folder
    #ifdef WIN32
    system("xcopy /e /h /i .\\setup\\ ..\\");
    #endif

    #if defined __APPLE__ || __linux__
    system("cp -R ./setup/* ../");
    #endif
}