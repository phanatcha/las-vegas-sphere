#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Globals.h>
#include "Input.h"
#include <camera.h>

#include <iostream>

// ~~~~~~~~~>constants<~~~~~~~~~~~
float lastX = Config::SCR_WIDTH / 2.0f;
float lastY = Config::SCR_HEIGHT / 2.0f;
bool firstMouse = true;


void processInput(GLFWwindow *window) 
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // ~~~~~>camera controls<~~~~~~
    float cameraSpeed = static_cast<float>(2.5 * deltaTime);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);
    
    // ~~~~>texture sequences controls<~~~~~
    // >>>numbers<<<
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) currentPattern = 1;
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) currentPattern = 2;
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) currentPattern = 3;
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) currentPattern = 4;
    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) currentPattern = 5;
    // >>>arrows<<<
    // right arrow = next pattern
    static bool rightKeyPressed = false;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        if (!rightKeyPressed) {
            currentPattern++;
            if (currentPattern > 5) 
                currentPattern = 1;
            rightKeyPressed = true;
        }
    } else {
        rightKeyPressed = false;
    }

    // left arrow = previous pattern
    static bool leftKeyPressed = false;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        if (!leftKeyPressed) {
            currentPattern--;
            if (currentPattern < 1)
                currentPattern = 5;
            leftKeyPressed = true;
        }
    } else {
        leftKeyPressed = false;
    }
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; 

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) 
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}