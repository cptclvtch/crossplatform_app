#ifndef API_IMPLEMENTATION_ONLY
typedef enum
{
    G_NONE,
    G_FLOAT,
    G_VEC2,
    G_VEC3,
    G_VEC4,
    G_SAMPLER_2D,
    G_INT,
    G_IVEC2,
    G_IVEC3,
    G_IVEC4,

    NO_OF_GPU_DATA_TYPES
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

typedef enum
{
    VERT,
    FRAG,

    NO_OF_SHADER_STAGES
}gpu_stage;

typedef struct
{
    uint8_t id;
    gpu_stage stage;
    char* name;
    
    gpu_data_type type;
    float *x,*y,*z,*w;
    int32_t *r,*g,*b,*a;
}uniform;

#define MAX_SHADER_UNIFORMS 16
typedef struct
{
    uint32_t id;

    char** vert_strings;
    uint16_t no_of_vert_strings;

    char** frag_strings;
    uint16_t no_of_frag_strings;

    uniform uniforms[MAX_SHADER_UNIFORMS];
}shader;

SDL_GLContext* context;
void start_graphics();
void close_graphics();
char* read_shader_source(char* path);
uniform* add_uniform(shader* s, uint8_t id, gpu_stage stage, char* name, gpu_data_type t);
void load_shader(shader* s);
void load_shader_from_file(shader* s, char* vertex_source, char* fragment_source);
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

uint8_t compile_shader_stage(uint32_t stage, char** segments, uint16_t segment_count)
{
    if(!glIsShader(stage))
	{
        PRINT_FN("%d is not a shader stage\n", stage);
        return;
    }

	glShaderSource(stage, segment_count, segments, NULL);
	glCompileShader(stage);

	//Check shader for errors
	GLint shader_stage_compiled = GL_FALSE;
	glGetShaderiv(stage, GL_COMPILE_STATUS, &shader_stage_compiled);
	if(shader_stage_compiled != GL_TRUE)
	{
		PRINT_FN("Unable to compile shader stage %u!\n", stage);
		print_stage_log(stage);
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
    to_return = calloc(1, file_size+1);
    to_return[file_size] = '\0';

    SDL_RWread(file, to_return, file_size, 1);

    SDL_RWclose(file);

    return to_return;
}

const float empty_f = 0.0;
const int32_t empty_i = 0.0;

uniform* add_uniform(shader* s, uint8_t index, gpu_stage stage, char* name, gpu_data_type type)
{
    if(index >= MAX_SHADER_UNIFORMS) return;
    if(type >= NO_OF_GPU_DATA_TYPES) return;
    if(stage >= NO_OF_SHADER_STAGES) stage = NO_OF_SHADER_STAGES - 1;
    
    uniform* u = &s->uniforms[index];
    u->stage = stage;
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

void sync_uniform(uniform u)
{
    PRINT_FN("Uniform type: %u\n", u.type);
    switch(u.type)
    {
        case G_FLOAT:
        PRINT_FN("%f\n",*u.x);
        glUniform1f(u.id, *u.x);
        break;

        case G_VEC2:
        PRINT_FN("%f, %f\n",*u.x, *u.y);
        glUniform2f(u.id, *u.x, *u.y);
        break;

        case G_VEC3:
        PRINT_FN("%f, %f, %f\n",*u.x, *u.y, *u.z);
        glUniform3f(u.id, *u.x, *u.y, *u.z);
        break;

        case G_VEC4:
        PRINT_FN("%f, %f, %f, %f\n",*u.x, *u.y, *u.z, *u.w);
        glUniform4f(u.id, *u.x, *u.y, *u.z, *u.w);
        break;

        case G_SAMPLER_2D:
        case G_INT:
        PRINT_FN("%u\n",*u.r);
        glUniform1i(u.id, *u.r);
        break;

        case G_IVEC2:
        PRINT_FN("%u, %u\n",*u.r, *u.g);
        glUniform2i(u.id, *u.r, *u.g);
        break;

        case G_IVEC3:
        PRINT_FN("%u, %u, %u\n",*u.r, *u.g, *u.b);
        glUniform3i(u.id, *u.r, *u.g, *u.b);
        break;
        
        case G_IVEC4:
        PRINT_FN("%u, %u, %u, %u\n",*u.r, *u.g, *u.b, *u.a);
        glUniform4i(u.id, *u.r, *u.g, *u.b, *u.a);
        break;
    }
}

void sync_all_uniforms(shader* s)
{
    if(s == NULL) return;
    GLint current_program;
    glGetIntegerv(GL_CURRENT_PROGRAM, &current_program);
    // PRINT_FN("Syncing uniforms for program:%u, current program:%u\n",s->id, current_program);
    if(s->id != current_program) return;

    uint8_t i = 0;
    for(; i < MAX_SHADER_UNIFORMS; i++)
        sync_uniform(s->uniforms[i]);
}

void load_shader(shader* s)
{
    if(s == NULL) return;

    uint8_t result;
    
    uint32_t vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    result = compile_shader_stage(vertex_shader, s->vert_strings, s->no_of_vert_strings);
    if(result != GL_TRUE) return;
    
    uint32_t fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    result = compile_shader_stage(fragment_shader, s->frag_strings, s->no_of_frag_strings);
    if(result != GL_TRUE) return;

	//Generate program
	s->id = glCreateProgram();

    glAttachShader(s->id, vertex_shader);
    glAttachShader(s->id, fragment_shader);

    //Link program
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

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    //cache uniforms
    uint8_t i = 0;
    for(; i < MAX_SHADER_UNIFORMS; i++)
    {
        if(s->uniforms[i].type)
            s->uniforms[i].id = glGetUniformLocation(s->id, s->uniforms[i].name);
    }

    PRINT_FN("Shader %u loaded!\n", s->id);
}

void load_shader_from_file(shader* s, char* vertex_source, char* fragment_source)
{
    if(s == NULL) return;

    s->vert_strings = (char**)calloc(1, sizeof(char*));
    s->vert_strings[0] = read_shader_source(vertex_source);
    if(s->vert_strings[0] == NULL) return;
    s->no_of_vert_strings = 1;

    s->frag_strings = (char**)calloc(1, sizeof(char*));
    s->frag_strings[0] = read_shader_source(fragment_source);
    if(s->frag_strings[0] == NULL) return;
    s->no_of_frag_strings = 1;

    load_shader(s);
}

void print_out_shader(shader* s)
{
    if(s == NULL) return;

    uint16_t i = 0;
    PRINT_FN("Vertex shader:\n");
    for(;i < s->no_of_vert_strings; i++)
        if(s->vert_strings[i])
            PRINT_FN("%s", s->vert_strings[i]);
        else
            PRINT_FN("Shader source no longer in memory.");

    PRINT_FN("Fragment shader:\n");
    for(i = 0; i < s->no_of_frag_strings; i++)
        if(s->frag_strings[i])
            PRINT_FN("%s", s->frag_strings[i]);
        else
            PRINT_FN("Shader source no longer in memory.");
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