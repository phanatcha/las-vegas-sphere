#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader_m.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Globals.h"
#include "Input.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Starfield.h"
#include "Moonlight.h"

#include "stb_image.h"
#include "filesystem.h"
#include <iostream>
#include <vector>

// ~~~~~~~~>vegasSphere<~~~~~~~~~~
Sphere vegasSphere(1.0f, 36, 18, true,  2);
Sphere moonSphere(0.2f, 36, 18, true, 2);
Cylinder baseCylinder(1.2f, 1.2f, 0.07f, 36, 1, true, 2);

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

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    
    Shader lightingSphereShader("light_sphere.vs", "light_sphere.fs");
    Shader lightingCylinderShader("light_base.vs", "light_base.fs");
    Shader lightingStarShader("star.vs", "star.fs");
    Shader lightMoonShader("moon.vs", "moon.fs");

    // ~~~~~~~~~~~~~~>vegasSphere buffer setup<~~~~~~~~~~~~~~
    unsigned int vegasSphereVAO, vegasSphereVBO, vegasSphereEBO;
    glGenVertexArrays(1, &vegasSphereVAO);
    glGenBuffers(1, &vegasSphereVBO);
    glGenBuffers(1, &vegasSphereEBO);

    glBindVertexArray(vegasSphereVAO);

    glBindBuffer(GL_ARRAY_BUFFER, vegasSphereVBO);
    glBufferData(GL_ARRAY_BUFFER, vegasSphere.getInterleavedVertexSize(), vegasSphere.getInterleavedVertices(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vegasSphereEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, vegasSphere.getIndexSize(), vegasSphere.getIndices(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vegasSphere.getInterleavedStride(), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vegasSphere.getInterleavedStride(), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // ~~~~~~~~~~~~~~>baseCylinder buffer setup<~~~~~~~~~~~~~~
    unsigned int baseCylinderVAO, baseCylinderVBO, baseCylinderEBO;
    glGenVertexArrays(1, &baseCylinderVAO);
    glGenBuffers(1, &baseCylinderVBO);
    glGenBuffers(1, &baseCylinderEBO);

    glBindVertexArray(baseCylinderVAO);

    glBindBuffer(GL_ARRAY_BUFFER, baseCylinderVBO);
    glBufferData(GL_ARRAY_BUFFER, baseCylinder.getInterleavedVertexSize(), baseCylinder.getInterleavedVertices(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, baseCylinderEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, baseCylinder.getIndexSize(), baseCylinder.getIndices(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, baseCylinder.getInterleavedStride(), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, baseCylinder.getInterleavedStride(), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

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

    // ~~~~~~~~~~~~~~~> moonSphere buffer setup <~~~~~~~~~~~~~~~~~
    unsigned int moonSphereVAO, moonSphereVBO, moonSphereEBO;
    glGenVertexArrays(1, &moonSphereVAO);
    glGenBuffers(1, &moonSphereVBO);
    glGenBuffers(1, &moonSphereEBO);

    glBindVertexArray(moonSphereVAO);

    glBindBuffer(GL_ARRAY_BUFFER, moonSphereVBO);
    glBufferData(GL_ARRAY_BUFFER, moonSphere.getInterleavedVertexSize(), moonSphere.getInterleavedVertices(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, moonSphereEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, moonSphere.getIndexSize(), moonSphere.getIndices(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, moonSphere.getInterleavedStride(), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, moonSphere.getInterleavedStride(), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    unsigned int moonTexture;
    //~~~~~~~~~>moonTexture<~~~~~~~~~~~~~
    glGenTextures(1, &moonTexture);
    glBindTexture(GL_TEXTURE_2D, moonTexture); 
    // set the texture wrapping/filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(FileSystem::getPath("resources/textures/moon1024.bmp").c_str(), &width, &height, &nrChannels, 0);

    if (data) 
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);
        
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // activate and bind texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, moonTexture);

        // ~~~~~~~~~~>>>>SPHERE<<<<~~~~~~~~~~~~~~~~~~~~~~~~
        // ~~~~~~~~~~>view/projection transformations<~~~~~~~~~~
        lightingSphereShader.use();
        sendMoonlightToShader(lightingSphereShader);
        // >>>black plastic material<<<
        lightingSphereShader.setVec3("material.ambient", 0.0f, 0.0f, 0.0f);
        lightingSphereShader.setVec3("material.diffuse", 0.02f, 0.02f, 0.02f);
        lightingSphereShader.setVec3("material.specular", 0.6f, 0.6f, 0.6f);
        lightingSphereShader.setFloat("material.shininess", 32.0f);

        lightingSphereShader.setVec3("viewPos", camera.Position);

        float currentLightLevel = isScreenOn ? globalBrightness : 0.0f;

        // beam time and the active pattern to the gpu
        lightingSphereShader.setFloat("time", currentFrame);
        lightingSphereShader.setInt("patternType", currentPattern);
        lightingSphereShader.setFloat("brightness", currentLightLevel);

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)Config::SCR_WIDTH / (float)Config::SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 vegasSphereModel = glm::mat4(1.0f);
        
        lightingSphereShader.setMat4("projection", projection);
        lightingSphereShader.setMat4("view", view);
        lightingSphereShader.setMat4("model", vegasSphereModel);

        glBindVertexArray(vegasSphereVAO);
        glDrawElements(GL_TRIANGLES, vegasSphere.getIndexCount(), GL_UNSIGNED_INT, (void*)0);

        // ~~~~~~~~~~>>>>Cylinder<<<<~~~~~~~~~~~~~~~~~~~~~~~~
        lightingCylinderShader.use();
        sendMoonlightToShader(lightingCylinderShader);

        // >>>reinforced concrete material<<<
        lightingCylinderShader.setVec3("material.ambient", 0.1f, 0.1f, 0.1f);
        lightingCylinderShader.setVec3("material.diffuse", 0.5f, 0.5f, 0.5f);
        lightingCylinderShader.setVec3("material.specular", 0.05f, 0.05f, 0.05f);
        lightingCylinderShader.setFloat("material.shininess", 2.0f);        

        lightingCylinderShader.setVec3("viewPos", camera.Position);
        
        lightingCylinderShader.setMat4("projection", projection);
        lightingCylinderShader.setMat4("view", view);

        glm::mat4 baseCylinderModel = glm::mat4(1.0f);
        baseCylinderModel = glm::translate(baseCylinderModel, glm::vec3(0.0f, -0.3f, 0.0f));
        lightingCylinderShader.setMat4("model", baseCylinderModel);

        glBindVertexArray(baseCylinderVAO);
        glDrawElements(GL_TRIANGLES, baseCylinder.getIndexCount(), GL_UNSIGNED_INT, (void*)0);

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

        // ~~~~~~~~~~>>>>Moon<<<<~~~~~~~~~~~~~~~~~~~~~~~~
        lightMoonShader.use();
        
        lightMoonShader.setMat4("projection", projection);
        lightMoonShader.setMat4("view", view);

        glm::mat4 moonModel = glm::mat4(1.0f);
        moonModel = glm::translate(moonModel, glm::vec3(-2.0f, 3.0f, -3.0f));
        lightMoonShader.setMat4("model", moonModel);

        glBindVertexArray(moonSphereVAO);
        glDrawElements(GL_TRIANGLES, moonSphere.getIndexCount(), GL_UNSIGNED_INT, (void*)0);

        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }
    glDeleteVertexArrays(1, &baseCylinderVAO);
    glDeleteBuffers(1, &baseCylinderVBO);
    glfwTerminate();
    return 0;
}


