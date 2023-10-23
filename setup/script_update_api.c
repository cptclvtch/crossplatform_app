#include "crossplatform_app/script_helper.c"

int main()
{
    set_cwd("crossplatform_app");
    system("git pull");
    system("git submodule update --remote");

    //Recursively copy the setup folder
    #ifdef WIN32
    system("xcopy /y /q .\\setup\\script_build.c ..\\");
    system("xcopy /y /q .\\setup\\script_run.c ..\\");
    system("xcopy /y /q .\\setup\\script_update_api.c ..\\");
    #endif

    #if defined __APPLE__ || __linux__
    system("cp ./setup/script_build.c ../");
    system("cp ./setup/script_run.c ../");
    system("cp ./setup/script_update_api.c ../");
    #endif
}