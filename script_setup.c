//#!/tools/tcc/win32
#include "script_helper.c"

int main()
{
    //install tcc
    #if defined _WIN32
    #endif
    #if defined __linux__
    // system("tools/tcc/linux/tcc_0.9.27-5_amd64.deb");
    #endif
    #if defined __APPLE__
    #endif

    //Recursively copy the setup folder
    fs_copy("./setup/*", "../", RECURSIVE);
    set_cwd("..");
    system("mkdir build");
}