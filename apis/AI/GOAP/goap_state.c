typedef uint8_t goap_var_id;
#define WORLD_VAR_COUNT 0xFF
typedef uint16_t goap_var_value;

typedef goap_var_value* goap_state;
#define var_array(state) state
#define var_value(state, id) var_array(state)[id]

#define GOAP_CHUNK_SIZE 5
#define MAX_CHUNKS 20

goap_var_value* initialize_state_memory()
{
    return (goap_var_value*)calloc(WORLD_VAR_COUNT+1, sizeof(goap_var_value));
}

//
enum STATE_REQUIREMENT
{
    NONE,
    NEAR,
    LESS,
    L_EQUAL,
    EQUAL,
    G_EQUAL,
    GREATER,

    REQUIREMENT_TYPE_COUNT
};

const char* STATE_REQUIREMENT_text[REQUIREMENT_TYPE_COUNT] =
{
    "-/-",
    "near",
    "< ",
    "<=",
    "==",
    ">=",
    "> "
};

typedef struct s_goap_state_requirement
{
    goap_var_id required_state;
    enum STATE_REQUIREMENT requirement_type;
    goap_var_value required_value;
}goap_state_requirement;

//chunked list type
typedef struct s_requirement_list
{
    goap_state_requirement* chunks[MAX_CHUNKS];

    uint16_t size;
}requirement_list;

requirement_list* start_requirement_list()
{
    requirement_list* new = (requirement_list*)calloc(1, sizeof(requirement_list));

    new->chunks[0] = (goap_state_requirement*)calloc(GOAP_CHUNK_SIZE, sizeof(goap_state_requirement));
    new->size = 0;

    return new;
}

void add_requirement_to_list(requirement_list* current_requirements, goap_state_requirement* new_requirement)
{
    if(current_requirements->size == MAX_CHUNKS*GOAP_CHUNK_SIZE) return;
    
    current_requirements->size++;
    
    if(current_requirements->size%GOAP_CHUNK_SIZE == 0)
        current_requirements->chunks[current_requirements->size/GOAP_CHUNK_SIZE] = (goap_state_requirement*)calloc(GOAP_CHUNK_SIZE, sizeof(goap_state_requirement));

    current_requirements->chunks[current_requirements->size/GOAP_CHUNK_SIZE][current_requirements->size%GOAP_CHUNK_SIZE] = *new_requirement;
}

void iterate_over_requirements(requirement_list* list, void(*iterator_function)(goap_state_requirement*))
{
    uint16_t index = 0;

    while(list->chunks[index/GOAP_CHUNK_SIZE] != NULL && index < list->size)
    {
        iterator_function(&list->chunks[index/GOAP_CHUNK_SIZE][index%GOAP_CHUNK_SIZE]);
        index++;
    }
}

//
enum STATE_EFFECT
{
    STATE_SET,
    STATE_INC,
    STATE_DEC,

    EFFECT_TYPE_COUNT
};

const char* STATE_EFFECT_text[EFFECT_TYPE_COUNT] =
{
    " = ",
    "^^^",
    "vvv"
};

typedef struct s_goap_state_effect
{
    //goap_state* affected_realities[8];
    goap_var_id affected_state;
    enum STATE_EFFECT effect_type;
    goap_var_value effect_amount;
}goap_state_effect;

//chunked list type
typedef struct s_effect_list
{
    goap_state_effect* chunks[MAX_CHUNKS];

    uint16_t size;
}effect_list;

effect_list* start_effect_list()
{
    effect_list* new = (effect_list*)calloc(1, sizeof(effect_list));

    new->chunks[0] = (goap_state_effect*)calloc(GOAP_CHUNK_SIZE, sizeof(goap_state_effect));
    new->size = 0;

    return new;
}

void add_effect_to_list(effect_list* current_effects, goap_state_effect* new_effect)
{
    if(current_effects->size == MAX_CHUNKS*GOAP_CHUNK_SIZE) return;
    
    current_effects->size++;
    
    if(current_effects->size%GOAP_CHUNK_SIZE == 0)
        current_effects->chunks[current_effects->size/GOAP_CHUNK_SIZE] = (goap_state_effect*)calloc(GOAP_CHUNK_SIZE, sizeof(goap_state_effect));

    current_effects->chunks[current_effects->size/GOAP_CHUNK_SIZE][current_effects->size%GOAP_CHUNK_SIZE] = *new_effect;
}

void iterate_over_effects(effect_list* list, void(*iterator_function)(goap_state_effect*))
{
    uint16_t index = 0;

    while(list->chunks[index/GOAP_CHUNK_SIZE] != NULL && index < list->size)
    {
        iterator_function(&list->chunks[index/GOAP_CHUNK_SIZE][index%GOAP_CHUNK_SIZE]);
        index++;
    }
}

// TODO implement iterators for lists and iterators between lists?