SDL_GLContext context;

GLuint gProgramID = 0;
GLint gVertexPos2DLocation = -1;
GLuint gVBO = 0;
GLuint gIBO = 0;


void printProgramLog(GLuint program)
{
	if(!glIsProgram(program))
    {
        PRINT_FN("%d is not a program\n", program);
        return;
    }
    //Program log length
    int infoLogLength = 0;
    int maxLength = infoLogLength;
    
    //Get info string length
    glGetProgramiv( program, GL_INFO_LOG_LENGTH, &maxLength );
    
    //Allocate string
    char* infoLog = malloc(maxLength);
    
    //Get info log
    glGetProgramInfoLog( program, maxLength, &infoLogLength, infoLog );
    if(infoLogLength)
        PRINT_FN( "%s\n", infoLog );
    
    //Deallocate string
    dealloc(infoLog);
    return;
}

void printShaderLog(GLuint shader)
{
	if(!glIsShader(shader))
	{
        PRINT_FN("%d is not a shader\n", shader);
        return;
    }

    //Shader log length
    int infoLogLength = 0;
    int maxLength = infoLogLength;
    
    //Get info string length
    glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &maxLength );
    
    //Allocate string
    char* infoLog = malloc(maxLength);
    
    //Get info log
    glGetShaderInfoLog( shader, maxLength, &infoLogLength, infoLog );
    if(infoLogLength)
        PRINT_FN( "%s\n", infoLog );

    //Deallocate string
    dealloc(infoLog);
    return;
}

bool initGL()
{
	//Create vertex shader
	GLuint vertexShader = glCreateShader( GL_VERTEX_SHADER );

	//Get vertex source
	const GLchar* vertexShaderSource[] =
	{
		"#version 140\n\
        in vec2 LVertexPos2D; \
        void main() { gl_Position = vec4( LVertexPos2D.x, LVertexPos2D.y, 0, 1 ); }"
	};

	//Set vertex source
	glShaderSource( vertexShader, 1, vertexShaderSource, NULL );

	//Compile vertex source
	glCompileShader( vertexShader );

	//Check vertex shader for errors
	GLint vShaderCompiled = GL_FALSE;
	glGetShaderiv( vertexShader, GL_COMPILE_STATUS, &vShaderCompiled );
	if( vShaderCompiled != GL_TRUE )
	{
		printf( "Unable to compile vertex shader %d!\n", vertexShader );
		printShaderLog( vertexShader );
        return false;
	}

    //Attach vertex shader to program
    glAttachShader( gProgramID, vertexShader );


    //Create fragment shader
    GLuint fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );

    //Get fragment source
    const GLchar* fragmentShaderSource[] =
    {
        "#version 140\nout vec4 LFragment; void main() { LFragment = vec4( 1.0, 1.0, 1.0, 1.0 ); }"
    };

    //Set fragment source
    glShaderSource( fragmentShader, 1, fragmentShaderSource, NULL );

    //Compile fragment source
    glCompileShader( fragmentShader );

    //Check fragment shader for errors
    GLint fShaderCompiled = GL_FALSE;
    glGetShaderiv( fragmentShader, GL_COMPILE_STATUS, &fShaderCompiled );
    if( fShaderCompiled != GL_TRUE )
    {
        printf( "Unable to compile fragment shader %d!\n", fragmentShader );
        printShaderLog( fragmentShader );
        return false;
    }

	//Generate program
	gProgramID = glCreateProgram();

    //Attach fragment shader to program
    glAttachShader( gProgramID, fragmentShader );

    //Link program
    glLinkProgram( gProgramID );

    //Check for errors
    GLint programSuccess = GL_TRUE;
    glGetProgramiv( gProgramID, GL_LINK_STATUS, &programSuccess );
    if( programSuccess != GL_TRUE )
    {
        printf( "Error linking program %d!\n", gProgramID );
        printProgramLog( gProgramID );
        return false;
    }

    //Get vertex attribute location
    gVertexPos2DLocation = glGetAttribLocation( gProgramID, "LVertexPos2D" );
    if( gVertexPos2DLocation == -1 )
    {
        printf( "LVertexPos2D is not a valid glsl program variable!\n" );
        return false;
    }

    //Initialize clear color
    glClearColor( 0.f, 0.f, 0.f, 1.f );

    //VBO data
    GLfloat vertexData[] =
    {
        -0.5f, -0.5f,
        0.5f, -0.5f,
        0.5f,  0.5f,
        -0.5f,  0.5f
    };

    //IBO data
    GLuint indexData[] = { 0, 1, 2, 3 };

    //Create VBO
    glGenBuffers( 1, &gVBO );
    glBindBuffer( GL_ARRAY_BUFFER, gVBO );
    glBufferData( GL_ARRAY_BUFFER, 2 * 4 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW );

    //Create IBO
    glGenBuffers( 1, &gIBO );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, gIBO );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), indexData, GL_STATIC_DRAW );


	return true;
}
