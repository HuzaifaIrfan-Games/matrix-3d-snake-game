
#include "matrix_3d_snake_game/game.h"


#include <iostream>

#include <glad/glad.h>


#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
 
#include "linmath.h"
 
#include <stdlib.h>
#include <stdio.h>


#include <cmath>

#include <glm/glm.hpp>                   // core GLM types (vec3, mat4, etc.)
#include <glm/gtc/matrix_transform.hpp>  // lookAt, perspective
#include <glm/gtc/type_ptr.hpp>          // value_ptr


float yaw = -90.0f, pitch = 0.0f;
float lastX = 400, lastY = 300;
bool firstMouse = true;

// Camera vectors
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float sensitivity = 0.1f;
    float xoffset = (xpos - lastX) * sensitivity;
    float yoffset = (lastY - ypos) * sensitivity;

    lastX = xpos;
    lastY = ypos;

    yaw   += xoffset;
    pitch += yoffset;

    // Clamp pitch
    if (pitch > 89.0f)  pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    // Update cameraFront vector
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}


static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}
 
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        std::cout << "Escape key pressed, closing the window." << std::endl;
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }


    if (action == GLFW_PRESS) {
        std::cout << "Key Pressed: " << key << std::endl;
    } else if (action == GLFW_RELEASE) {
        std::cout << "Key Released: " << key << std::endl;
    }

}



// Cube vertices (single face for demo)
float cubeVertices[] = {
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f
};

const char* vertexShaderSrc = R"(
#version 330 core
layout(location = 0) in vec3 aPos;
uniform mat4 view;
uniform mat4 projection;
void main() {
    gl_Position = projection * view * vec4(aPos, 1.0);
}
)";

const char* fragmentShaderSrc = R"(
#version 330 core
out vec4 FragColor;
void main() {
    FragColor = vec4(0.2, 0.6, 1.0, 1.0);
}
)";


GLuint compileShader(GLenum type, const char* src) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);
    return shader;
}

 
int main(void)
{
    std::cout << "Welcome to the 3D Snake Game!" << std::endl;
    start_game();
    // Game loop

    GLFWwindow* window;
    GLuint vertex_buffer, vertex_shader, fragment_shader, program;
    GLint mvp_location, vpos_location, vcol_location;
 
    glfwSetErrorCallback(error_callback);
 
if (!glfwInit()) {
    std::cerr << "Failed to init GLFW\n";
    return -1;
}


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
 
    window = glfwCreateWindow(640, 480, "Matrix 3D Snake Game", NULL, NULL);
    if (!window)
    {
        std::cerr << "Failed to create window\n";
        glfwTerminate();
        exit(EXIT_FAILURE);
        return -1;
    }

 
 
    glfwSetKeyCallback(window, key_callback);

        // Register mouse move callback
    glfwSetCursorPosCallback(window, mouse_callback);

    // Capture and hide the mouse
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

 
    glfwMakeContextCurrent(window);
        
    // Load GLAD *after* context is current

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "GLAD load failed\n";
        return -1;
    }

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << "\n";


    vertex_shader = compileShader(GL_VERTEX_SHADER, vertexShaderSrc);
    fragment_shader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSrc);
    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO); glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glEnable(GL_DEPTH_TEST);



    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClearColor(0.1, 0.1, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(program);

        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.f / 600.f, 0.1f, 100.0f);

        GLuint viewLoc = glGetUniformLocation(program, "view");
        GLuint projLoc = glGetUniformLocation(program, "projection");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}