typedef struct s_planning_helper planning_helper;
#define PLANNING_HELPER_CLUSTER_SIZE 4
typedef struct s_planning_helper_node
{
    //state id, current value, direction
    goap_var_id vectors[PLANNING_HELPER_CLUSTER_SIZE][3];

    planning_helper* next;
}planning_helper_node;

enum
{
    STATE_ID, CURR_VALUE, DIRECTION
};

typedef struct s_planning_helper
{
    goap_agent* agent;
    planning_helper_node* root;

    uint8_t current_index;
    uint8_t count;
}planning_helper;

void free_planning_helper_node(planning_helper_node* node);

void free_planning_helper_node(planning_helper_node* node)
{
    if(node->next)
        free_planning_helper_node(node->next);
    else
        free(node);
}

void free_planning_helper(planning_helper* helper)
{
    free_planning_helper_node(helper->root);
    free(helper);
}

planning_helper* get_new_planning_helper(goap_agent* agent)
{
    planning_helper* to_return = (planning_helper*)calloc(1, sizeof(planning_helper));

    to_return->agent = agent;

    to_return->current_index = 0;
    to_return->count = 0;

    return to_return;
}

void add_state_to_planning_helper(planning_helper* helper, goap_var_id state)
{
    if(helper->root == NULL)
        helper->root = (planning_helper_node*)calloc(1, sizeof(planning_helper_node));

    planning_helper_node* current_node = helper->root;
    uint8_t cluster_index = 1;
    // #error here
    while(current_node->next)
    {
        current_node = current_node->next;
        cluster_index++;
    }

    if(cluster_index*PLANNING_HELPER_CLUSTER_SIZE <= helper->count)
    {
        current_node->next = (planning_helper_node*)calloc(1, sizeof(planning_helper_node));
        current_node = current_node->next;
    }

    uint8_t sub_index = helper->count%PLANNING_HELPER_CLUSTER_SIZE;
    current_node->vectors[sub_index][STATE_ID] = state;
    current_node->vectors[sub_index][CURR_VALUE] = helper->agent->current_state[state];
    helper->count++;   
}

goap_var_id* get_next_planning_helper_item(planning_helper* helper)
{
    // #error somewhere here
    if(helper->root == NULL) return NULL;

    planning_helper_node* current_node = helper->root;

    if(helper->current_index >= helper->count) return NULL;

    uint8_t cluster_index = helper->current_index / PLANNING_HELPER_CLUSTER_SIZE;
    uint8_t sub_index = helper->current_index % PLANNING_HELPER_CLUSTER_SIZE;    
    helper->current_index++;

    uint8_t index = 1;
    while(current_node->next && index < cluster_index)
    {
        current_node = current_node->next;
        index++;
    }

    return current_node->vectors[sub_index];
}
//-----------------------------------------------------

void calculate_goal_vectors(planning_helper* helper)
{
    if(helper->root == NULL) return;

    helper->current_index = 0;
    goap_var_id* item = NULL;
    while(item = get_next_planning_helper_item(helper))
    {
        if( helper->agent->goal_state->requirement_type >= EQUAL
            &&
            helper->agent->goal_state->required_value > get_var_value(helper->agent->current_state, item[STATE_ID]))
                item[DIRECTION] = STATE_INC;

        if( helper->agent->goal_state->requirement_type <= EQUAL
            &&
            helper->agent->goal_state->required_value < get_var_value(helper->agent->current_state, item[STATE_ID]))
                item[DIRECTION] = STATE_DEC;
    }
}

// uint8_t random_action = 0;
// #define GET_RANDOM_ACTION() random_action*123 + 13

goap_action* pick_best_action(planning_helper* helper)
{
    goap_action* chosen_action = NULL;
    uint16_t chosen_score = 0;

    uint16_t index = 0;
    for(; index < goap_action_count; index++)
    {
        uint8_t cluster_index = index / ACTION_CLUSTER_SIZE;
        uint8_t sub_index = index % ACTION_CLUSTER_SIZE;
        goap_action* action = &goap_all_actions[cluster_index][sub_index];

        printf("%s:\n", action->name);

        //
        uint16_t action_score = 0;
    
        helper->current_index = 0;
        goap_var_id* item = NULL;
        while(item = get_next_planning_helper_item(helper))
        {
            printf("\tState:%u value:%u dir:%s ",item[STATE_ID], item[CURR_VALUE], STATE_EFFECT_text[item[DIRECTION]]);
            uint8_t effect_index = 0;
            for(; effect_index < action->effect_max_index; effect_index++)
            {
                if(action->effects[effect_index].affected_state == item[STATE_ID])
                {
                    printf("+", item[STATE_ID]);
                    action_score++;
                    
                    if(action->effects[effect_index].effect_type == item[DIRECTION])
                    {
                        printf("*");
                        action_score++;
                    }
                }
            }
        }
        printf("\n");

        //
        if(action_score > chosen_score)
        {
            chosen_score = action_score;
            chosen_action = action;
        }
    }

    return chosen_action;
}

void make_plan(goap_agent* agent)
{
    printf("\tMaking plan\n");

    if(agent->goal_state == NULL) return;

    printf("\tCalculating vectors\n");

    planning_helper* helper = get_new_planning_helper(agent);

    if(agent->goal_state->requirement_type != NONE)
        add_state_to_planning_helper(helper, agent->goal_state->required_state);
    
    calculate_goal_vectors(helper);

    //pick best action
    while(helper->count)
    {
        printf("\tPicking action\n");
        action_stack_node* result = add_action_stack_node(agent->action_plan, pick_best_action(helper));
        if(result) agent->action_plan = result;
        
        //remove previously completed requirements
        uint8_t index = 0;
        planning_helper_node* current_node = helper->root;
        while(current_node->next)
        {
            for(index = 0; index < GOAP_CHUNK_SIZE; index++)
            {
                uint8_t effect_index = 0;
                for(; effect_index < result->effect_max_index; effect_index++)
                    if(current_node->vectors[index%4][STATE_ID] == result->effects[effect_index].affected_state)
                    {
                        //apply effect
                        switch(result->effects[effect_index].effect_type)
                        {
                            case STATE_DEC:
                            current_node->vectors[index%4][CURR_VALUE] -= result->effects[effect_index].effect_amount;
                            break;

                            case STATE_SET:
                            current_node->vectors[index%4][CURR_VALUE] = result->effects[effect_index].effect_amount;
                            break;

                            case STATE_INC:
                            current_node->vectors[index%4][CURR_VALUE] += result->effects[effect_index].effect_amount;
                            break;
                        }

                        //check for completion
                        switch(current_node->vectors[index%4][])
                        {
                            #error needs more work here
                            if(current_node->vectors[index%4][CURR_VALUE])
                        }
                    }
            }
            current_node = current_node->next;
        }

        //propagate requirements
    }

    printf("\tAgent plan:\n");
    print_action_stack(agent->action_plan);

    //cleanup
    free_planning_helper(helper);
}