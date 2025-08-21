/*
An input context holds:

- a 3 layer input description:
    - actual input
    - perceived input
    - action

    - can be edited

- a 3 layer runtime:
    - actual input:
        - press, release, hold
        - stored in a buffer (ring buffer? linked list?)
    - perceived input:
        - stored in a stack
            - simple passthrough (1 to 1: actual to perceived)
            - complex combos
    - action:
        - void (uint8_t*) function (passes its own running flag [perhaps implemented as a static function instead])
        - can run over multiple frames (holds a "running" flag)
        - 
*/
#ifndef _INPUT_SYSTEM_H
    #define _INPUT_SYSTEM_H
typedef struct
{
    //inputs sources:
    //- keyboard
    //- touch button (vr/mobile)
    //- analog derivative thresholds (smashes)

    //input types:
    //- pressed
    //- released
    //- hold

    
}input_actual;

typedef struct
{
    //- double tap
    //- joystick flick
    //- charge motion (hold one input then press a trigger input)
    //- negative edge (treat press as release)

    uint8_t action_id;
}input_perceived;

typedef struct
{
    uint8_t id;

    uint8_t running;
    void (*execute)(uint8_t* running);
}input_action;

//input buffer
    //- input consumption
    //  - marked with tags?
    //  - unlinking from linked list?
    //  - marking as zero?

//contexts
typedef struct
{
    //description
    

    //runtime

    //tuple list
    // input_actual* //linked list? ring buffer?
    uint8_t max_actual_inputs;

    //input_perceived stack
    uint8_t max_perceived_inputs;

    input_action actions[10];
}input_context;

input_context* current_input_context;

void interpret_input();
#endif //_INPUT_SYSTEM_H

//----------------------------------

#if defined(INCLUDE_IMPLEMENTATION) && !defined(_INPUT_SYSTEM_C)
    #define _INPUT_SYSTEM_C

void interpret_input()
{

}
#endif