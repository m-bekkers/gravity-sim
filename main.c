#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#include "common/loadShader.c"


int main() {
    
    glewExperimental = true; // needed for core profile
    if(!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }
    
    // Window creation
    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // opengl 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // macos stuff
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // no old opengl

    GLFWwindow *window; // pointer to window object
    window = glfwCreateWindow(1024, 768, "Tutorial 1", NULL, NULL);
    if(window == NULL) {
        fprintf(stderr, "Failed to open GLFW window.\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window); // init glew
    glewExperimental = true;
    if(glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to init GLEW\n");
        return -1;
    }
    
    // create a VAO (vertex array object)
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID); // set as current
    
    // ensure we can capture esc keypress
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    
    // array of 3 vectors to represent 3 vertices
    static const GLfloat g_vertex_buffer_data[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
    };

    // identify vertex buffer
    GLuint vertexbuffer;
    // generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &vertexbuffer);
    // the following commands will talk about vertexbuffer buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    // give vertices to opengl
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    
    // create and compile glsl program from shaders
    GLuint programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

    do {
        // clear the screen - may cause flickering
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(programID);
        
        // drawing here
        
        // 1st attribute buffer: vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
            0,          // attribute 0
            3,          // size
            GL_FLOAT,   // type
            GL_FALSE,   // normalized
            0,          // stride
            (void *)0   // array buffer offset
        );

        // draw triangle
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDisableVertexAttribArray(0);

        // swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    } // check if esc pressed or if window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

    return 0;
}