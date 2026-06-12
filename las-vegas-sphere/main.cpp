#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <shader_m.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Globals.h>
#include <Input.h>
#include <Sphere.h>
#include <Cylinder.h>
#include <Starfield.h>

#include <iostream>
#include <vector>

// ~~~~~~~~>sphere<~~~~~~~~~~
Sphere sphere(1.0f, 36, 18, true,  2);
Cylinder cylinder(1.2f, 1.2f, 0.07f, 36, 1, true, 2);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *window = glfwCreateWindow(Config::SCR_WIDTH, Config::SCR_HEIGHT, "SPHERE!", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    
    Shader lightingSphereShader("light_sphere.vs", "light_sphere.fs");
    Shader lightingCylinderShader("light_base.vs", "light_base.fs");
    Shader lightingStarShader("star.vs", "star.fs");

    // ~~~~~~~~~~~~~~>sphere buffer setup<~~~~~~~~~~~~~~
    unsigned int sphereVAO, sphereVBO, sphereEBO;
    glGenVertexArrays(1, &sphereVAO);
    glGenBuffers(1, &sphereVBO);
    glGenBuffers(1, &sphereEBO);

    glBindVertexArray(sphereVAO);

    glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
    glBufferData(GL_ARRAY_BUFFER, sphere.getInterleavedVertexSize(), sphere.getInterleavedVertices(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphere.getIndexSize(), sphere.getIndices(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sphere.getInterleavedStride(), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    // ~~~~~~~~~~~~~~>cylinder buffer setup<~~~~~~~~~~~~~~
    unsigned int cylinderVAO, cylinderVBO, cylinderEBO;
    glGenVertexArrays(1, &cylinderVAO);
    glGenBuffers(1, &cylinderVBO);
    glGenBuffers(1, &cylinderEBO);

    glBindVertexArray(cylinderVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cylinderVBO);
    glBufferData(GL_ARRAY_BUFFER, cylinder.getInterleavedVertexSize(), cylinder.getInterleavedVertices(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cylinderEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, cylinder.getIndexSize(), cylinder.getIndices(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, cylinder.getInterleavedStride(), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    // ~~~~~~~~~~~~~~>star buffer setup<~~~~~~~~~~~~~~
    initStars();

    unsigned int starVAO, starVBO;
    glGenVertexArrays(1, &starVAO);
    glGenBuffers(1, &starVBO);

    glBindVertexArray(starVAO);
    glBindBuffer(GL_ARRAY_BUFFER, starVBO);
    glBufferData(GL_ARRAY_BUFFER, stars.size() * sizeof(Star), stars.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Star), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(Star), (void*)offsetof(Star, size));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Star), (void*)offsetof(Star, brightness));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);
        
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // ~~~~~~~~~~>>>>SPHERE<<<<~~~~~~~~~~~~~~~~~~~~~~~~
        // ~~~~~~~~~~>view/projection transformations<~~~~~~~~~~
        lightingSphereShader.use();

        // beam time and the active pattern to the gpu
        lightingSphereShader.setFloat("time", currentFrame);
        lightingSphereShader.setInt("patternType", currentPattern);

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)Config::SCR_WIDTH / (float)Config::SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 sphereModel = glm::mat4(1.0f);
        
        lightingSphereShader.setMat4("projection", projection);
        lightingSphereShader.setMat4("view", view);
        lightingSphereShader.setMat4("model", sphereModel);

        glBindVertexArray(sphereVAO);
        glDrawElements(GL_TRIANGLES, sphere.getIndexCount(), GL_UNSIGNED_INT, (void*)0);

        // ~~~~~~~~~~>>>>Cylinder<<<<~~~~~~~~~~~~~~~~~~~~~~~~
        lightingCylinderShader.use();
        
        lightingCylinderShader.setMat4("projection", projection);
        lightingCylinderShader.setMat4("view", view);

        glm::mat4 cylinderModel = glm::mat4(1.0f);
        cylinderModel = glm::translate(cylinderModel, glm::vec3(0.0f, -0.3f, 0.0f));
        lightingCylinderShader.setMat4("model", cylinderModel);

        glBindVertexArray(cylinderVAO);
        glDrawElements(GL_TRIANGLES, cylinder.getIndexCount(), GL_UNSIGNED_INT, (void*)0);

        // ~~~~~~~~~~>>>>Star<<<<~~~~~~~~~~~~~~~~~~~~~~~~
        glEnable(GL_PROGRAM_POINT_SIZE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        lightingStarShader.use();
        lightingStarShader.setMat4("projection", projection);
        lightingStarShader.setMat4("view", view);

        glBindVertexArray(starVAO);
        glDrawArrays(GL_POINTS, 0, stars.size());

        glDisable(GL_BLEND);

        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }
    glDeleteVertexArrays(1, &cylinderVAO);
    glDeleteBuffers(1, &cylinderVBO);
    glfwTerminate();
    return 0;
}


