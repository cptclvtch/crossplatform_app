#include "crossplatform_app/api.c"

void (*event_handling)(EVENT* event)
{
      if(event.type == SDL_QUIT)
      {
        running = false;
      }
      else
      if(event.type == SDL_KEYDOWN)
      {
        const char *key = SDL_GetKeyName(event.key.keysym.sym);
        if(strcmp(key, "Q") == 0)
        {
          running = false;
        }
      }
}

int main()
{
    setup();

    standard_loop(event_handling);

    close();
}