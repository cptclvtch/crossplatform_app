#include <stdlib.h>

#include "crossplatform_app/script_helper.c"

int main()
{
    set_cwd("crossplatform_app");
    system("git pull");
    system("git submodule update --remote");
}