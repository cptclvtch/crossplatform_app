//#!/tools/tcc/win32
#include "script_helper.c"

#define _STRINGIFY(x) #x
#define STRINGIFY(x) _STRINGIFY(x)
#ifndef PROJECT_FOLDER
#define PROJECT_FOLDER ../
#else
#warning Dont forget to update CROSSPLATFORM_APP_FOLDER in your projects app_configuration.c
#endif

int main()
{
    //Recursively copy the setup folder
    fs_copy("./setup/*", STRINGIFY(PROJECT_FOLDER), RECURSIVE);
    set_cwd(STRINGIFY(PROJECT_FOLDER));
    system("mkdir build");
}