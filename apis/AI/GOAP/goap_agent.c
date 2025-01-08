typedef struct s_goap_agent
{
    goap_state current_state;
    goap_state_requirement* goal_state;

    action_stack_node* action_plan;
}goap_agent;

void initialize_agent_memory(goap_agent* agent)
{
    agent->current_state = initialize_state_memory();
    agent->goal_state = (goap_state_requirement*)calloc(1, sizeof(goap_state_requirement));

    agent->action_plan = NULL;
}


#include "goap_agent_action_planning.c"