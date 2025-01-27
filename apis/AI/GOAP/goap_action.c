#define MAX_ACTION_REQUIREMENT_COUNT 8
#define MAX_ACTION_EFFECT_COUNT 8
typedef struct s_goap_action
{
    goap_state_requirement  requirements[MAX_ACTION_REQUIREMENT_COUNT];
    uint8_t requirement_max_index;
    goap_state_effect       effects[MAX_ACTION_EFFECT_COUNT];
    uint8_t effect_max_index;

    char* name;

    // goap_var_value subject, object;

    // void (*execute)();
}goap_action;

#define ACTION_CLUSTER_SIZE 4
#define ACTION_CLUSTER_COUNT 32
goap_action* goap_all_actions[ACTION_CLUSTER_COUNT];
uint16_t goap_action_count = 0;
goap_action* goap_active_action;

void set_active_action(uint16_t a)
{
    if(a < goap_action_count) goap_active_action = goap_all_actions[a];
}

uint16_t add_action(char* name)
{
    if((goap_action_count+1) > ACTION_CLUSTER_SIZE*ACTION_CLUSTER_COUNT) return;

    uint8_t cluster_index = goap_action_count/ACTION_CLUSTER_SIZE;
    if(goap_all_actions[cluster_index] == NULL)
    {
        goap_action* new_cluster = (goap_action*)calloc(ACTION_CLUSTER_SIZE, sizeof(goap_action));
        if(new_cluster == NULL) return;

        goap_all_actions[cluster_index] = new_cluster;
        uint8_t index = 0;
        for(;index < ACTION_CLUSTER_SIZE; index++)
        {
            new_cluster[index].requirement_max_index = 0;
            new_cluster[index].effect_max_index = 0;
        }
    }

    goap_active_action = &goap_all_actions[cluster_index][goap_action_count%ACTION_CLUSTER_SIZE];
    if(name)
        goap_active_action->name = name;
    else
        goap_active_action->name = "-No-name-";
    goap_action_count++;
}

void add_action_requirement(goap_state_requirement r)
{
    if(goap_active_action == NULL) return;
    if(goap_active_action->requirement_max_index+1 > MAX_ACTION_REQUIREMENT_COUNT-1) return;

    goap_active_action->requirements[goap_active_action->requirement_max_index++] = r;
}

void add_action_effect(goap_state_effect e)
{
    
    if(goap_active_action == NULL) return;
    if(goap_active_action->effect_max_index+1 > MAX_ACTION_EFFECT_COUNT-1) return;

    goap_active_action->effects[goap_active_action->effect_max_index++] = e;
}

uint8_t execute_action(goap_state truth, goap_action action)
{
    //check requirements
    uint8_t index = 0;
    for(; index < 8; index++)
    {
        switch(action.requirements[index].requirement_type)
        {
            goap_var_value a = var_value(truth, action.requirements[index].required_state);
            goap_var_value b = action.requirements[index].required_value;

            case NEAR:
            if(a != b) return 0;
            break;

            case LESS:
            if(a >= b) return 0;
            break;

            case L_EQUAL:
            if(a > b) return 0;
            break;

            case EQUAL:
            if(a != b) return 0;
            break;

            case G_EQUAL:
            if(a < b) return 0;
            break;

            case GREATER:
            if(a <= b) return 0;
            break;
        }
    }

    for(index = 0; index < 8; index++)
    {
        goap_var_id a = action.effects[index].affected_state;
        goap_var_value b = action.effects[index].effect_amount;

        switch(action.effects[index].effect_type)
        {
            goap_var_value result;
            case STATE_SET:
            var_value(truth, a) = b;
            // var_value(agent_list[action.subject].current_state, a) = b;
            break;

            case STATE_DEC:
            result = a + b;
            if(result < a) return 0;
            var_value(truth, a) = result;
            // var_value(agent_list[action.subject].current_state, a) = result;
            break;

            case STATE_INC:
            result = a - b;
            if(result > a) return 0;
            var_value(truth, a) = result;
            // var_value(agent_list[action.subject].current_state, a) = result;
            break;
        }
    }

    return 1;
}

void print_all_actions(char** state_names)
{
    char state_name[64];

    printf("\tTotal number of actions: %u\n\n", goap_action_count);
    uint16_t index = 0;
    for(;index < goap_action_count; index++)
    {    
        uint8_t cluster_index = index/ACTION_CLUSTER_SIZE;
        uint8_t sub_index = index%ACTION_CLUSTER_SIZE;

        goap_action* action = &goap_all_actions[cluster_index][sub_index];
        printf("\t%s (ID:%u, location: %u,%u)", action->name, index, cluster_index, sub_index);
        if(action == goap_active_action) printf(" (Active)");
        printf("\n");

            uint8_t requirement_index = 0;
            printf("\t\t%u action requirements:\n", action->requirement_max_index);
            for(;requirement_index < action->requirement_max_index; requirement_index++)
            {
                if(state_names)
                    sprintf(state_name, "%s", state_names[action->requirements[requirement_index].required_state]);
                else
                    sprintf(state_name, "State ID:%u", action->requirements[requirement_index].required_state);

                printf("\t\t  %s %s %u\n",
                        state_name,
                        STATE_REQUIREMENT_text[action->requirements[requirement_index].requirement_type],
                        action->requirements[requirement_index].required_value);
            }
            printf("\n");

            uint8_t effect_index = 0;
            printf("\t\t%u action effects:\n", action->effect_max_index);
            for(;effect_index < action->effect_max_index; effect_index++)
            {
                if(state_names)
                    sprintf(state_name, "%s", state_names[action->effects[effect_index].affected_state]);
                else
                    sprintf(state_name, "State ID:%u", action->effects[effect_index].affected_state);

                printf("\t\t  %s %s %u\n",
                        state_name,
                        STATE_EFFECT_text[action->effects[effect_index].effect_type],
                        action->effects[effect_index].effect_amount);
            }
            printf("\n");
    }
}

//Action stack
typedef struct s_action_stack_node action_stack_node;

typedef struct s_action_stack_node
{
    goap_action* action;
    action_stack_node* next;
}action_stack_node;

action_stack_node* add_action_stack_node(action_stack_node* node, goap_action* action)
{
    action_stack_node* active_node = node;

    if(active_node == NULL)
        active_node = (action_stack_node*)calloc(1, sizeof(action_stack_node));

    //find last node
    while(active_node->next)
        active_node = active_node->next;

    //add new node
    active_node = active_node->next = (action_stack_node*)calloc(1, sizeof(action_stack_node));
    active_node->action = action;

    return active_node;
}

void free_action_stack(action_stack_node* root);

void free_action_stack(action_stack_node* root)
{
    if(root->next)
        free_action_stack(root->next);
    else
        free(root);
}

void print_action_stack(action_stack_node* node)
{
    action_stack_node* current_node = node;
    while(current_node)
    {
        if(current_node->action == NULL) break;
        
        if(current_node->action->name)
            printf("%s\n", current_node->action->name);
        else
            printf("-no-action-name\n");

        current_node = current_node->next;
    };
}