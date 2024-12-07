#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../script_helper.c"

#define DEFINITION_MODE
#define IMPLEMENTATION_MODE
#include "../../apis/graphics_pipeline/vec.c"
#include "../../apis/graphics_pipeline/mesh.c"

// typedef struct s_position
// {
//     float x,y,z;
// } position;

// typedef struct s_texture
// {
//     float x,y;
// } texture;

uint8_t process_file(char* path, char* output_folder)
{
    sterilize_path(path);
    sterilize_path(output_folder);

    //validate .obj path, no folders allowed
    const char* extension = ".obj";
    char* validation = strstr(path, extension);
    if(validation)
    {
        size_t extension_size = strlen(path) - (size_t)validation + (size_t)path;
        if(extension_size != strlen(extension)) return 0;
    }

    FILE* input_file = fopen(path, "r");
    if(input_file == NULL)
    {
        //attempt relative address
        char cwd[256];
        get_cwd(cwd, 256);
        strcat(cwd,"\\");
        strcat(cwd,path);
        input_file = fopen(cwd,"r");
    }

    if(input_file == NULL)
    {
        printf("Can't open input file: %s\n", path);
        return 0;
    }

    //construct output path
    char* file_name;
    char* file_extension;
    
    file_name = strrchr(path, '\\');
    if(file_name == NULL) file_name = path;

    file_extension = strrchr(path, '.');
    char destination[256];
    strcpy(destination, output_folder);
    strcat(destination, "\\");
    file_extension[0] = '\0';
    strcat(destination, file_name);
    file_extension[0] = '.';
    strcat(destination, ".mesh");

    FILE* output_file = fopen(destination, "wb");
    if(output_file == NULL)
    {
        printf("Couldn't open output file: %s\n", destination);    
        return 0;
    }

    //
    fseek(input_file, 0, SEEK_END);
    fpos_t file_size = 0;
    fgetpos(input_file, &file_size);
    fseek(input_file, 0, SEEK_SET);
    // printf("File size: %u\n", file_size);

    char* buffer = (char*)malloc(file_size);
    fread(buffer, 1, file_size, input_file);
    
    //count the number of vertices
    mesh_index_t vertex_count = 0;
    char* vertex = buffer;
    while((vertex = strstr(vertex, "v ")) != NULL)
    {
        vertex_count++;
        vertex++;
    }

    // printf("Vertex count: %u\n", vertex_count);

    //read vertex data
    mesh_vertex* vertices = (mesh_vertex*)malloc(vertex_count*sizeof(mesh_vertex));
    uint32_t i = 0;
    vertex = buffer;
    while((vertex = strstr(vertex, "v ")) != NULL)
    {
        vertex++;
        vertices[i].pos.x = atof(vertex = strstr(vertex, " ") + 1);
        vertices[i].pos.y = atof(vertex = strstr(vertex, " ") + 1);
        vertices[i].pos.z = atof(vertex = strstr(vertex, " ") + 1);

        // printf("pos: %f %f %f\n", vertices[i].pos.x, vertices[i].pos.y, vertices[i].pos.z);

        i++;
        vertex++;
    }

    i = 0;
    vertex = buffer;
    while((vertex = strstr(vertex, "vt ")) != NULL)
    {
        vertex++;
        vertices[i].tex.x = atof(vertex = strstr(vertex, " ") + 1);
        vertices[i].tex.y = atof(vertex = strstr(vertex, " ") + 1);

        // printf("tex: %f %f\n", vertices[i].tex.x, vertices[i].tex.y);

        i++;
        vertex++;
    }

    uint32_t face_count = 0;
    char* face = buffer;
    while((face = strstr(face, "f ")) != NULL)
    {
        face_count++;
        face++;
    }
    
    uint32_t* faces = (uint32_t*)malloc(face_count*3*sizeof(uint32_t));

    face = buffer;
    while((face = strstr(face, "f ")) != NULL)
    {
        vertices[i].tex.x = atof(vertex = strstr(vertex, " ") + 1);
        face++;
    }

    //output to file
    uint32_t count_to_be_written = (uint32_t)vertex_count;
    fwrite(&count_to_be_written, sizeof(uint32_t), 1, output_file);
    fwrite((void*)vertices, sizeof(mesh_vertex), vertex_count, output_file);

    //
    fclose(input_file);
    fclose(output_file);
    free(vertices);
    free(buffer);

    printf("Converted \t%s\n", destination);
    return 1;
}

int main(int argc, char** argv)
{
    uint8_t i = 1;
    for(;i < argc - 1; i++) process_file(argv[i], argv[argc-1]);
}