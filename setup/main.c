#include "app_configuration.c"
#include "crossplatform_app/api.h"

int main()
{
    setup();

    standard_loop();

    close();
}