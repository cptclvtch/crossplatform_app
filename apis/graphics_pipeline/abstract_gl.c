#ifndef API_IMPLEMENTATION_ONLY
typedef struct shader_t
{
    uint32_t id;

    uint8_t uniforms[16];
}shader;

SDL_GLContext* context;
void start_graphics();
void close_graphics();
void load_shader(shader* s, char* vertex_source, char* fragment_source);
void use_shader(shader* s);
void unload_shader(shader* s);
#define COLOR_BIT GL_COLOR_BUFFER_BIT
#define DEPTH_BIT GL_DEPTH_BUFFER_BIT
#define clear_mask_type GLbitfield
#define clear_screen(bitfield) glClear(bitfield)
#endif

#ifdef API_IMPLEMENTATION_ONLY
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
void print_shader_log(shader* s)
{
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

uint8_t compile_shader_stage(uint32_t stage, char* code)
{
    if(!glIsShader(stage))
	{
        PRINT_FN("%d is not a shader stage\n", stage);
        return;
    }

	glShaderSource(stage, 1, &code, NULL);
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

void load_shader(shader* s, char* vertex_source, char* fragment_source)
{
    uint8_t result;
    
    char* vertex_code = read_shader_source(vertex_source);
    if(vertex_code == NULL) return;
    
    uint32_t vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    result = compile_shader_stage(vertex_shader, vertex_code);
    free(vertex_code);
    if(result != GL_TRUE) return;

    char* fragment_code = read_shader_source(fragment_source);
    if(fragment_code == NULL) return;
    
    uint32_t fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    result = compile_shader_stage(fragment_shader, fragment_code);
    free(fragment_code);
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
        printf("Error linking shader stages %d!\n", s->id);
        print_shader_log(s);
        return;
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    PRINT_FN("Shader %u loaded!\n", s->id);
}

void use_shader(shader* s)
{
    glUseProgram(s->id);
}

void unload_shader(shader* s)
{
    glDeleteProgram(s->id);
}
#endif