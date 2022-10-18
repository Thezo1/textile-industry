#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include <string.h>
#include <math.h>


#define GLFW_INCLUDE_GLEXT
#include<GLFW/glfw3.h>
#include "shader.c"
#include "stb.c"

#define DEFAULT_SCREEN_WIDTH 1600
#define DEFAULT_SCREEN_HEIGHT 900

void framebuffer_size_callblack(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
                     GLsizei length, const GLchar *message,
                     const void *userParam) {
    (void)source;
    (void)id;
    (void)length;
    (void)userParam;
    fprintf(stderr,
            "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
            (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type, severity,
            message);
}

int main(void)
{
    if (!glfwInit()) {

        fprintf(stderr, "Error: could not initialize GLFW\n");

        exit(1);
    }

    GLFWwindow *const window =
        glfwCreateWindow(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT,
                         "OpenGL Template", NULL, NULL);

    if (window == NULL) {
        fprintf(stderr, "ERROR: could not create a window.\n");
        glfwTerminate();
        exit(1);
    }

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callblack);

    load_gl_extensions();

    if (glDebugMessageCallback != NULL) {
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(MessageCallback, 0);
    }

    const char *vertex_file_path = "shaders/shader.vert";
    const char *fragment_file_path = "shaders/shader.frag";

    GLuint program;
    if (load_shader_program(vertex_file_path, fragment_file_path, &program) != 1) {
        exit(1);
    }


    //compile vertex shader
    float vertices[] = {
        //coordinates       //colors
        -0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 0.0f,  0.0f, 0.0f,
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f,    0.0f, 1.0f,
        0.5f, 0.5f, 0.0f,   1.0f, 0.0f, 1.0f,   1.0f, 1.0f,
        0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 1.0f,  1.0f, 0.0f,
    };

    unsigned int indices[]={
        0, 2, 1,
        0, 3, 2
    };

    //texture
    int width_image, height_image, num_color_channel;
    unsigned char* bytes = stbi_load("textures/ttk.png", &width_image, &height_image, &num_color_channel, 0);

    GLuint texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //float flat_color[] = {1.0f, 1.0f, 1.0f};
    //glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flat_color);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_image, height_image, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(bytes);
    glBindTexture(GL_TEXTURE_2D, 0);

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.5f, 0.25f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        GLuint tex0_uni = glGetUniformLocation(program, "tex0");

        glUseProgram(program);

        glUniform1i(tex0_uni, 0);
        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindTexture(GL_TEXTURE_2D, texture);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    printf("\nHello Arch\n");
    return 0;
}

