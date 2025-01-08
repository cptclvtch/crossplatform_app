#include <stdio.h>
#include "api.c"

//user specified action
enum
{
    IDLE,
    CHECK_FRIDGE,
    MAKE_SANDWICH,
    EAT_SANDWICH,
    BUY_BREAD,
    BUY_HAM,
    GO_TO_GROCERY_STORE,
    GO_TO_APARTMENT,

    ACTION_COUNT
};

//user specified states
enum
{
    JONS_LOCATION,
    JONS_HUNGER_LEVEL,
    BREAD_SLICES_IN_JONS_FRIDGE,
    HAM_SLICES_IN_JONS_FRIDGE,
    SANDWICHES_IN_JONS_FRIDGE,

    CASHIERS_LOCATION,

    TOTAL_NUMBER_OF_STATES
};

char* state_names[] =
{
    "Jon's location",
    "Jon's hunger level",
    "Bread slices in fridge",
    "Ham slices in fridge",
    "Sandwiches in fridge",

    "Cashier's location"
};

//agents
enum
{
    JON,
    CASHIER,

    AGENT_COUNT
};

//locations
enum
{
    JONS_APARTMENT,
    GROCERY_STORE,

    LOCATION_COUNT
};

int main()
{
    //create characters
    goap_agent* agents = (goap_agent*)calloc(AGENT_COUNT, sizeof(goap_agent));
    uint8_t index = 0;
    for(; index < AGENT_COUNT; index++)
    {
        initialize_agent_memory(&agents[index]);
    }

    //JON
    agents[JON].goal_state = &(goap_state_requirement){JONS_HUNGER_LEVEL, L_EQUAL, 2};
    agents[JON].current_state[JONS_HUNGER_LEVEL] = 10;

    //initialize world state
    printf("Initializing world state\n");
    goap_state world_truth;
    get_var_array(world_truth) = initialize_state_memory();
    get_var_value(world_truth, JONS_HUNGER_LEVEL) = 5;
    get_var_value(world_truth, JONS_LOCATION) = JONS_APARTMENT;
    get_var_value(world_truth, CASHIERS_LOCATION) = GROCERY_STORE;
    get_var_value(world_truth, BREAD_SLICES_IN_JONS_FRIDGE) = 0;
    get_var_value(world_truth, HAM_SLICES_IN_JONS_FRIDGE) = 1;

    //create actions
    printf("Initializing action list\n");
    //IDLE
    add_action("Idle");
    
    //CHECK_FRIDGE
    add_action("Check fridge");
        add_action_requirement((goap_state_requirement){JONS_LOCATION, NEAR, JONS_APARTMENT});
        // add_action_effect((goap_state_effect){BREAD_SLICES_IN_JONS_FRIDGE, STATE_SET, BREAD_SLICES_IN_JONS_FRIDGE});
        // add_action_effect((goap_state_effect){HAM_SLICES_IN_JONS_FRIDGE, STATE_SET, HAM_SLICES_IN_JONS_FRIDGE});

    //MAKE SANDWICH
    add_action("Make sandwich");
        add_action_requirement((goap_state_requirement){JONS_LOCATION, NEAR, JONS_APARTMENT});
        add_action_requirement((goap_state_requirement){BREAD_SLICES_IN_JONS_FRIDGE, G_EQUAL, 1});
        add_action_requirement((goap_state_requirement){HAM_SLICES_IN_JONS_FRIDGE, G_EQUAL, 2});
        add_action_effect((goap_state_effect){BREAD_SLICES_IN_JONS_FRIDGE, STATE_DEC, 1});
        add_action_effect((goap_state_effect){HAM_SLICES_IN_JONS_FRIDGE, STATE_DEC, 2});
        add_action_effect((goap_state_effect){SANDWICHES_IN_JONS_FRIDGE, STATE_INC, 1});

    //EAT SANDWICH
    add_action("Eat sandwich");
        add_action_requirement((goap_state_requirement){JONS_LOCATION, NEAR, JONS_APARTMENT});
        add_action_requirement((goap_state_requirement){SANDWICHES_IN_JONS_FRIDGE, G_EQUAL, 1});
        add_action_effect((goap_state_effect){SANDWICHES_IN_JONS_FRIDGE, STATE_DEC, 1});
        add_action_effect((goap_state_effect){JONS_HUNGER_LEVEL, STATE_DEC, 1});

    //BUY BREAD
    add_action("Buy bread");
        add_action_requirement((goap_state_requirement){JONS_LOCATION, NEAR, GROCERY_STORE});
        add_action_requirement((goap_state_requirement){CASHIERS_LOCATION, NEAR, GROCERY_STORE});
        add_action_effect((goap_state_effect){BREAD_SLICES_IN_JONS_FRIDGE, STATE_INC, 10});

    //BUY HAM
    add_action("Buy ham");
        add_action_requirement((goap_state_requirement){JONS_LOCATION, NEAR, GROCERY_STORE});
        add_action_requirement((goap_state_requirement){CASHIERS_LOCATION, NEAR, GROCERY_STORE});
        add_action_effect((goap_state_effect){HAM_SLICES_IN_JONS_FRIDGE, STATE_INC, 4});

    //GO TO
    add_action("Go to Grocery Store");
        add_action_effect((goap_state_effect){JONS_LOCATION, STATE_SET, GROCERY_STORE});
    add_action("Go to Apartment");
        add_action_effect((goap_state_effect){JONS_LOCATION, STATE_SET, JONS_APARTMENT});

    //action list debrief
    print_all_actions(state_names);

    printf("Running simulation\n");
    //run planning
    // uint16_t simulation_turns = 300;
    // while(simulation_turns--)
    // {
        if(agents[JON].action_plan == NULL)
            make_plan(&agents[JON]);
        else
        {
            //turn code below into action stack code
            // uint8_t index = 0;
            // for(; index < agents[JON].action_plan_count; index++)
            //     execute_action(world_truth, agents, agents[JON].action_plan[0]);
        }
    // }

    printf("Simulation done!\n");
}