#include "app_configuration.c"
#include "crossplatform_app/api.c"

int main()
{
    setup();

    game_loop();

    close();
}