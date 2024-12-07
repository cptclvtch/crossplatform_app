#ifndef GOAP_SYSTEM
#define GOAP_SYSTEM

#ifndef uint16_t
#include <stdint.h>
#endif

#ifndef calloc
#include <stdlib.h>
#endif

#define CLEAR_SCREEN {printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");} //ghetto cls
#define ENTER_TO_CONTINUE {printf("\nPress Enter to continue...\n");while(getchar() != '\n');}

#include "goap_state.c"
#include "goap_action.c"
#include "goap_agent.c"
#endif