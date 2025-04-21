#ifndef API_IMPLEMENTATION_ONLY
typedef enum
{
    G_FLOAT = 1,
    G_VEC2,
    G_VEC3,
    G_VEC4,
    G_SAMPLER_2D,
    G_INT,
    G_IVEC2,
    G_IVEC3,
    G_IVEC4,

    NO_OF_GPU_DATA_TYPES,
    EFFECTIVE_NO_OF_GPU_DATA_TYPES = NO_OF_GPU_DATA_TYPES - 1
}gpu_data_type;

char* gpu_data_type_text[] = 
{
    "",
    "float",
    "vec2",
    "vec3",
    "vec4",
    "sampler2D",
    "int",
    "ivec2",
    "ivec3",
    "ivec4"
};

typedef struct
{
    uint32_t id;
    char* name;
    
    gpu_data_type type;
    float *x,*y,*z,*w;
    int32_t *r,*g,*b,*a;
}uniform;

typedef enum
{
    VERT,
    FRAG,

    MAX_SHADER_STAGES
};

#define MAX_SHADER_UNIFORMS 16
typedef struct
{
    uint32_t id;

    char** strings;
    uint16_t no_of_strings;

    uniform uniforms[MAX_SHADER_UNIFORMS];
    char* cached_uniform_string;
}shader_stage;

typedef struct
{
    uint32_t id;

    shader_stage stages[MAX_SHADER_STAGES];
}shader;

SDL_GLContext* context;
void start_graphics();
void close_graphics();
char* read_shader_source(char* path);
uniform* declare_uniform(shader_stage* s, uint8_t id, char* name, gpu_data_type t);
uint32_t retrieve_uniform_id(shader* s, char* name);
//MACRO: void set_uniform(id, type, ...)
char* cache_uniform_string(shader_stage* stage);
void load_shader(shader* s);
shader* load_shader_from_file(shader* s, char* vertex_source, char* fragment_source);
void print_out_shader(shader* s);
void use_shader(shader* s);
void use_shader_and_sync(shader* s);
void unload_shader(shader* s);
#define COLOR_BIT GL_COLOR_BUFFER_BIT
#define DEPTH_BIT GL_DEPTH_BUFFER_BIT
#define clear_mask_type GLbitfield
#define clear_screen(bitfield) glClear(bitfield)

//----------------------------------
#else
//----------------------------------

void start_graphics()
{
    //Context
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

    context = SDL_GL_CreateContext(window);

    //V-Sync
    SDL_GL_SetSwapInterval(1);

    //Load functions
    if(open_gl())
    {
        gladLoadGLLoader(SDL_GL_GetProcAddress);
        close_gl();
    }

    //OpenGL state
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    glClearColor(0.1, 0.1, 0.1, 1.0);
}

void close_graphics()
{
    SDL_GL_DeleteContext(context);
}

//Shaders
#ifndef RELEASE
void print_shader_log(shader* s)
{
    if(s == NULL) return;

	if(!glIsProgram(s->id))
    {
        PRINT_FN("%d is not a shader\n", s->id);
        return;
    }

    int max_length = 0;
    
    //Get info string length
    glGetProgramiv(s->id, GL_INFO_LOG_LENGTH, &max_length);
    
    //Get info log
    int log_length = 0;
    char* info_log = calloc(1, max_length);
    glGetProgramInfoLog(s->id, max_length, &log_length, info_log);
    if(log_length) PRINT_FN("%s\n", info_log);
    
    free(info_log);
    return;
}

void print_stage_log(uint32_t stage)
{
	if(!glIsShader(stage))
	{
        PRINT_FN("%d is not a shader stage\n", stage);
        return;
    }

    int max_length = 0;
    
    //Get info string length
    glGetShaderiv(stage, GL_INFO_LOG_LENGTH, &max_length);
    
    //Get info log
    int log_length = 0;
    char* info_log = calloc(1, max_length);
    glGetShaderInfoLog(stage, max_length, &log_length, info_log);
    if(log_length) PRINT_FN("%s\n", info_log);

    free(info_log);
    return;
}
#else
#define print_shader_log(a)
#define print_stage_log(a)
#endif

uint8_t compile_shader_stage(shader_stage* stage)
{
    if(!glIsShader(stage->id))
	{
        PRINT_FN("%u is not a shader stage\n", stage->id);
        return;
    }

	glShaderSource(stage->id, stage->no_of_strings, stage->strings, NULL);
	glCompileShader(stage->id);

	//Check shader for errors
	GLint shader_stage_compiled = GL_FALSE;
	glGetShaderiv(stage->id, GL_COMPILE_STATUS, &shader_stage_compiled);
	if(shader_stage_compiled != GL_TRUE)
	{
		PRINT_FN("Unable to compile shader stage %u!\n", stage->id);
		print_stage_log(stage->id);
	}

    return (uint8_t)shader_stage_compiled;
}

char* read_shader_source(char* path)
{
    char* to_return = NULL;

    SDL_RWops* file = SDL_RWFromFile(path, "r");
    if(file == NULL)
    {
        PRINT_FN("File error: %s\n", SDL_GetError());
        return to_return;
    }
    
    uint32_t file_size = SDL_RWsize(file);
    to_return = calloc(file_size + 1, 1);
    to_return[file_size] = '\0';

    SDL_RWread(file, to_return, file_size, 1);

    SDL_RWclose(file);

    return to_return;
}

const float empty_f = 0.0;
const int32_t empty_i = 0.0;

uniform* declare_uniform(shader_stage* s, uint8_t index, char* name, gpu_data_type type)
{
    if(index >= MAX_SHADER_UNIFORMS) return;
    if(type > EFFECTIVE_NO_OF_GPU_DATA_TYPES) return;
    
    uniform* u = &s->uniforms[index];
    u->name = name;
    
    u->type = type;
    u->x = &empty_f;
    u->y = &empty_f;
    u->z = &empty_f;
    u->w = &empty_f;
    
    u->r = &empty_i;
    u->g = &empty_i;
    u->b = &empty_i;
    u->a = &empty_i;

    return u;
}

uint32_t retrieve_uniform_id(shader* s, char* name)
{
    return glGetUniformLocation(s->id, name);
}

#define set_uniform(id, type, ...) glUniform ## type(id, __VA_ARGS__)

void sync_uniform(uniform u)
{
    switch(u.type)
    {
        case G_FLOAT:
        set_uniform(u.id, 1f, *u.x);
        break;

        case G_VEC2:
        set_uniform(u.id, 2f, *u.x, *u.y);
        break;

        case G_VEC3:
        set_uniform(u.id, 3f, *u.x, *u.y, *u.z);
        break;

        case G_VEC4:
        set_uniform(u.id, 4f, *u.x, *u.y, *u.z, *u.w);
        break;

        case G_SAMPLER_2D:
        case G_INT:
        set_uniform(u.id, 1i, *u.r);
        break;

        case G_IVEC2:
        set_uniform(u.id, 2i, *u.r, *u.g);
        break;

        case G_IVEC3:
        set_uniform(u.id, 3i, *u.r, *u.g, *u.b);
        break;
        
        case G_IVEC4:
        set_uniform(u.id, 4i, *u.r, *u.g, *u.b, *u.a);
        break;
    }
}

void sync_all_uniforms(shader* s)
{
    if(s == NULL) return;
    GLint current_program;
    glGetIntegerv(GL_CURRENT_PROGRAM, &current_program);
    if(s->id != current_program) return;

    uint8_t i = 0, stage = 0;

    for(stage = 0; stage < MAX_SHADER_STAGES; stage++)
        for(; i < MAX_SHADER_UNIFORMS; i++)
            sync_uniform(s->stages[stage].uniforms[i]);
}

char* cache_uniform_string(shader_stage* stage)
{
    uint16_t i = 0;

    uint16_t uniform_count = 0;
    for(i = 0; i < MAX_SHADER_UNIFORMS; i++)
        if(stage->uniforms[i].type) uniform_count++;

    if(stage->cached_uniform_string != NULL)
        free(stage->cached_uniform_string);
    stage->cached_uniform_string = (char*)calloc(uniform_count*128, sizeof(char));

    char* current_uniform = stage->cached_uniform_string;
    for(i = 0; i < MAX_SHADER_UNIFORMS; i++)
    {
        uniform* u = &stage->uniforms[i];
        if(u->type)
        {
            sprintf(current_uniform, "uniform %s %s;\n", gpu_data_type_text[u->type], u->name);
            current_uniform += strlen(current_uniform);
        }
    }

    return stage->cached_uniform_string;
}

void load_shader(shader* s)
{
    if(s == NULL) return;

    uint16_t i = 0, stage = 0;

    for(stage = 0; stage < MAX_SHADER_STAGES; stage++)
    {
        shader_stage* current_stage = &s->stages[stage];
        if(current_stage->strings)
        {
            uint8_t result;
            uint32_t stage_ids[] = {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER};
            current_stage->id = glCreateShader(stage_ids[stage]);
            result = compile_shader_stage(current_stage);

            if(result != GL_TRUE)return;
        }
    }

	//Generate program
	s->id = glCreateProgram();

    for(stage = 0; stage < MAX_SHADER_STAGES; stage++)
        glAttachShader(s->id, s->stages[stage].id);

    glLinkProgram(s->id);

    //Check for errors
    GLint shader_success = GL_TRUE;
    glGetProgramiv(s->id, GL_LINK_STATUS, &shader_success);
    if(shader_success != GL_TRUE)
    {
        PRINT_FN("Error linking shader stages %d!\n", s->id);
        print_shader_log(s);
        return;
    }

    //uniform id caching and stage cleanup
    for(stage = 0; stage < MAX_SHADER_STAGES; stage++)
    {
        shader_stage* current_stage = &s->stages[stage];

        glDeleteShader(current_stage->id);

        //cache uniform ids
        for(i = 0; i < MAX_SHADER_UNIFORMS; i++)
        {
            if(current_stage->uniforms[i].type)
                current_stage->uniforms[i].id = retrieve_uniform_id(s, current_stage->uniforms[i].name);
        }
    }

    PRINT_FN("Shader %u loaded!\n", s->id);
}

//TODO switch this to source array
shader* load_shader_from_file(shader* s, char* vertex_source, char* fragment_source)
{
    if(s == NULL)
        s = (shader*)calloc(1,sizeof(shader));

    shader_stage* current_stage = &s->stages[0];

    current_stage->strings = (char**)calloc(1, sizeof(char*));
    current_stage->strings[0] = read_shader_source(vertex_source);
    if(current_stage->strings[0] == NULL) return;
    current_stage->no_of_strings = 1;

    current_stage = &s->stages[1];

    current_stage->strings = (char**)calloc(1, sizeof(char*));
    current_stage->strings[0] = read_shader_source(fragment_source);
    if(current_stage->strings[0] == NULL) return;
    current_stage->no_of_strings = 1;

    load_shader(s);
    
    return s;
}

void print_out_shader(shader* s)
{
    if(s == NULL) return;

    uint16_t stage = 0;
    for(stage = 0; stage < MAX_SHADER_STAGES; stage++)
    {
        shader_stage* current_stage = &s->stages[stage];
        uint16_t i;
        PRINT_FN("Stage %u:\n", stage);
        for(i = 0; i < current_stage->no_of_strings; i++)
            if(current_stage->strings[i])
                PRINT_FN("%s", current_stage->strings[i]);
            else
                PRINT_FN("Shader source no longer in memory.");
    }
}

void use_shader(shader* s)
{
    if(s == NULL) return;

    glUseProgram(s->id);
}

void use_shader_and_sync(shader* s)
{
    use_shader(s);
    sync_all_uniforms(s);
}

void unload_shader(shader* s)
{
    if(s == NULL) return;

    glDeleteProgram(s->id);
}
#endif