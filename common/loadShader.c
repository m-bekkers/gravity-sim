/**
 * Boilerplate general purpose shader loading function
 */

#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "loadShader.h"

// note: this function was originally in cpp
GLuint LoadShaders(const char *vertex_file_path, const char *fragment_file_path) {
    // create shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // read the vertex shader code from the file
    char *VertexShaderCode = NULL; // std::string

    FILE *VertexShaderStream = fopen(vertex_file_path, "r"); // open the provided file in read mode
    if(VertexShaderStream) {
        // move pointer to end of file w/ offset = 0
        fseek(VertexShaderStream, 0, SEEK_END);
        long file_size = ftell(VertexShaderStream); // ftell returns the pointer location (so number of bytes in the file)
        rewind(VertexShaderStream); // go back to start of file

        VertexShaderCode = malloc(file_size + 1); // create string buffer big enough for the file
        if(VertexShaderCode) {
            fread(VertexShaderCode, 1, file_size, VertexShaderStream); // read file into buffer
            VertexShaderCode[file_size] = '\0'; // ensure file can stop being read
        }

        fclose(VertexShaderStream); // cleanup
    }
    else {
        printf("ERROR: Unable to open %s. Please ensure you are in the correct directory.\n", vertex_file_path);
        printf("Press any key to continue.\n");
        getchar();
        return 0;
    }

    // read fragment shader code from file
    char *FragmentShaderCode = NULL; // buffer for the file contents

    FILE *FragmentShaderStream = fopen(fragment_file_path, "r");
    if(FragmentShaderStream) {
        fseek(FragmentShaderStream, 0, SEEK_END);
        long file_size = ftell(FragmentShaderStream);
        rewind(FragmentShaderStream);
        FragmentShaderCode = malloc(file_size + 1);
        if(FragmentShaderCode) {
            fread(FragmentShaderCode, 1, file_size, FragmentShaderStream);
            FragmentShaderCode[file_size] = '\0';
        }
        fclose(FragmentShaderStream);
    }
    else {
        printf("ERROR: Unable to open %s. Please ensure you are in the correct directory.\n", fragment_file_path);
        printf("Press any key to continue.\n");
        getchar();
        return 0;
    }

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // compile vertex shader
    printf("Compiling shader: %s\n", vertex_file_path);
    char const *VertexSourcePointer = VertexShaderCode;
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
    glCompileShader(VertexShaderID);

    // check vertex shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if(InfoLogLength > 0){
        char *VertexShaderErrorMessage = (char *)malloc(InfoLogLength + 1);

        if(VertexShaderErrorMessage) {
            glGetShaderInfoLog(
                VertexShaderID,
                InfoLogLength,
                NULL,
                VertexShaderErrorMessage
            );
        }

        VertexShaderErrorMessage[InfoLogLength] = '\0';
        printf("%s\n", VertexShaderErrorMessage);

        free(VertexShaderErrorMessage);
    }

    // compile fragment shader
    printf("Compiling shader: %s\n", fragment_file_path);
    char const *FragmentSourcePointer = FragmentShaderCode;
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
    glCompileShader(FragmentShaderID);

    // check fragment shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);

    if(InfoLogLength > 0) {
        char *FragmentShaderErrorMessage = (char *)malloc(InfoLogLength + 1);

        if(FragmentShaderErrorMessage) {
            glGetShaderInfoLog(
                FragmentShaderID,
                InfoLogLength,
                NULL,
                FragmentShaderErrorMessage
            );
        }

        FragmentShaderErrorMessage[InfoLogLength] = '\0';
        printf("%s\n", FragmentShaderErrorMessage);

        free(FragmentShaderErrorMessage);
    }

    // link the program
    printf("Linking program...\n");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);

    // check program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);

    if(InfoLogLength > 0) {
        char *ProgramErrorMessage = (char *)malloc(InfoLogLength + 1);

        if(ProgramErrorMessage) {
            glGetProgramInfoLog(
                ProgramID,
                InfoLogLength,
                NULL,
                ProgramErrorMessage
            );
        }

        ProgramErrorMessage[InfoLogLength] = '\0';
        printf("%s\n", ProgramErrorMessage);

        free(ProgramErrorMessage);
    }

    // cleanup
    free(VertexShaderCode);
    free(FragmentShaderCode);

    glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);
	
	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}