typedef struct s_goap_agent
{
    goap_state current_state;
    requirement_list goal_state;

    action_stack_node* action_plan;
}goap_agent;

void initialize_agent_memory(goap_agent* agent)
{
    agent->current_state = initialize_state_memory();
    agent->goal_state = *start_requirement_list();

    agent->action_plan = NULL;
}


#include "goap_agent_action_planning.c"