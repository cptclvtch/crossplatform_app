//#!/tools/tcc/win32
#include "script_helper.c"

int main()
{
    //setup environment variables
    //cmdline tools
    add_value_to_environment_variable("tools/android/clit", "PATH")
    //gradle //add bin to PATH
    add_value_to_environment_variable("tools/android/gradle-8.13/bin", "PATH");
    //android sdk
    //android ndk

    //Recursively copy the setup folder
    fs_copy("./setup/*", "../", RECURSIVE);
    set_cwd("..");
    system("mkdir build");
}