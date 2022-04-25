#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Object.h"
#include "Light.h"


void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

const unsigned int SCR_WIDTH  = 1600;
const unsigned int SCR_HEIGHT = 1200;

Camera camera(glm::vec3(0.0f, 1.5f, 3.0f));

float fDeltaTime = 0.0f;
float fLastFrame = 0.0f;

float bFirstMouse = true;
float lastX = 800;
float lastY = 600;

bool bShowLights = true;


int main()
{
    /**********************************/
    /*        Configure Window        */
    /**********************************/
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Welcome to Project 0", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
        printf("\nERROR: Failed to initialize GLEW");



    /**********************************************************/
    /**********************************************************/
    /*                       GL Configs                       */  
    /**********************************************************/
    /**********************************************************/
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glEnable(GL_DEPTH_TEST);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);



    /****************************************************/
    /****************************************************/
    /*                       Cube                       */  
    /****************************************************/
    /****************************************************/
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    Model cube = Model("res/models/cube_scifi/obj/Cube.obj");
    Model backpack = Model("res/models/backpack/backpack.obj");
    

    std::vector<Object> objects;
    for (int i = 0; i < 8; i++)
    {
        objects.push_back(Object(cube, glm::vec3(2.0 * i, 0.0f, 0.0f)));
    }


    /*********************************************************/
    /*********************************************************/
    /*                       VAO, VBO                        */  
    /*********************************************************/
    /*********************************************************/
    unsigned int cubeVBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);

    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);




    /*********************************************************/
    /*********************************************************/
    /*                       Textures                        */  
    /*********************************************************/
    /*********************************************************/
//    std::string filepath = "res/textures/container2.png";
//
//    unsigned int textureID;
//    glGenTextures(1, &textureID);
//
//    int width, height, nrComponents;
//    unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &nrComponents, 0);
//
//    if (data)
//    {
//        GLenum format;
//        if (nrComponents == 1)
//            format = GL_RED;
//        else if (nrComponents == 3)
//            format = GL_RGB;
//        else if (nrComponents == 4)
//            format = GL_RGBA;
//
//        glBindTexture(GL_TEXTURE_2D, textureID);
//        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
//        glGenerateMipmap(GL_TEXTURE_2D);
//
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//        stbi_image_free(data);
//    }
//    else
//    {
//        std::cout << "Texture failed to load at path: " << filepath << std::endl;
//        stbi_image_free(data);
//    }



    /*******************************************************/
    /*******************************************************/
    /*                       Shaders                       */  
    /*******************************************************/
    /*******************************************************/
    Shader simpleShader("shaders/vSimple.shader", "shaders/fSimple.shader");
    Shader shader("shaders/vLight.shader", "shaders/fLight.shader");
    shader.Use();
    //shader.SetInt("texture_diffuse1", 0);
    stbi_set_flip_vertically_on_load(true);



    /*********************************************************/
    /*********************************************************/
    /*                       Lighting                        */  
    /*********************************************************/
    /*********************************************************/
    glm::vec3 lightPos(5.2f, 1.0f, 2.0f);
    std::vector<Light> lights = {
        Light(DIR_LIGHT, lightPos),
        Light(POINT_LIGHT, glm::vec3( 0.7f,  0.2f,  2.0f)),
        Light(POINT_LIGHT, glm::vec3( 2.3f, -3.3f, -4.0f)),
        Light(POINT_LIGHT, glm::vec3(-4.0f,  2.0f, -12.0f)),
        Light(POINT_LIGHT, glm::vec3( 0.0f,  0.0f, -3.0f))
    };

    glm::vec3 pointLightPositions[] = {
        glm::vec3( 0.7f,  0.2f,  2.0f),
        glm::vec3( 2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3( 0.0f,  0.0f, -3.0f)
    };



    /*******************************/
    /*        Set Draw Mode        */
    /*******************************/
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);



    while(!glfwWindowShouldClose(window))
    {
        float fCurrentFrame = static_cast<float>(glfwGetTime());
        fDeltaTime = fCurrentFrame - fLastFrame;
        fLastFrame = fCurrentFrame;

        processInput(window);

        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, textureID);

        shader.Use();
        shader.SetVec3("viewPos", camera.Position);

        shader.SetFloat("material.shininess", 32.0f);

        shader.SetVec3("dirLight.position", lightPos);
        shader.SetVec3("dirLight.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
        shader.SetVec3("dirLight.diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
        shader.SetVec3("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));

        shader.SetVec3("pointLights[0].position", pointLightPositions[0]);
        shader.SetVec3("pointLights[0].ambient", glm::vec3(0.05, 0.05, 0.05));
        shader.SetVec3("pointLights[0].diffuse", glm::vec3(0.8, 0.8, 0.8));
        shader.SetVec3("pointLights[0].specular", glm::vec3(1.0, 1.0, 1.0));
        shader.SetFloat("pointLights[0].constant", 1.0f);
        shader.SetFloat("pointLights[0].linear", 1.09f);
        shader.SetFloat("pointLights[0].quadratic", 0.032f);

        shader.SetVec3("pointLights[1].position", pointLightPositions[1]);
        shader.SetVec3("pointLights[1].ambient", glm::vec3(0.05, 0.05, 0.05));
        shader.SetVec3("pointLights[1].diffuse", glm::vec3(0.8, 0.8, 0.8));
        shader.SetVec3("pointLights[1].specular", glm::vec3(1.0, 1.0, 1.0));
        shader.SetFloat("pointLights[1].constant", 1.0f);
        shader.SetFloat("pointLights[1].linear", 1.09f);
        shader.SetFloat("pointLights[1].quadratic", 0.032f);

        shader.SetVec3("pointLights[2].position", pointLightPositions[2]);
        shader.SetVec3("pointLights[2].ambient", glm::vec3(0.05, 0.05, 0.05));
        shader.SetVec3("pointLights[2].diffuse", glm::vec3(0.8, 0.8, 0.8));
        shader.SetVec3("pointLights[2].specular", glm::vec3(1.0, 1.0, 1.0));
        shader.SetFloat("pointLights[2].constant", 1.0f);
        shader.SetFloat("pointLights[2].linear", 1.09f);
        shader.SetFloat("pointLights[2].quadratic", 0.032f);

        shader.SetVec3("pointLights[3].position", pointLightPositions[3]);
        shader.SetVec3("pointLights[3].ambient", glm::vec3(0.05, 0.05, 0.05));
        shader.SetVec3("pointLights[3].diffuse", glm::vec3(0.8, 0.8, 0.8));
        shader.SetVec3("pointLights[3].specular", glm::vec3(1.0, 1.0, 1.0));
        shader.SetFloat("pointLights[3].constant", 1.0f);
        shader.SetFloat("pointLights[3].linear", 1.09f);
        shader.SetFloat("pointLights[3].quadratic", 0.032f);

        shader.SetVec3("spotLight.position", camera.Position);
        shader.SetVec3("spotLight.direction", camera.Front);
        shader.SetVec3("spotLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
        shader.SetVec3("spotLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
        shader.SetVec3("spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
        shader.SetFloat("spotLight.constant", 1.0f);
        shader.SetFloat("spotLight.linear", 0.09f);
        shader.SetFloat("spotLight.linear", 0.032f);
        shader.SetFloat("spotLight.cutOff", glm::cos(glm::radians(12.5)));
        shader.SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0)));


        /***********************************************/
        /*        Set Model / View / Projection        */
        /***********************************************/
        glm::mat4 projection = glm::perspective(glm::radians<float>(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, (float)0.1, 200.0f);
        glm::mat4 view = camera.GetViewMatrix();
        shader.SetMat4("projection", projection);
        shader.SetMat4("view", view);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        for (int i = 0; i < objects.size(); i++)
        {
            objects[i].Draw(shader);
        }
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        if (bShowLights)
        {
            simpleShader.Use();
            simpleShader.SetMat4("projection", projection);
            simpleShader.SetMat4("view", view);
            glBindVertexArray(cubeVAO);
            for (int i = 0; i < lights.size(); i++)
            {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, lights[i].m_vPos);
                model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
                simpleShader.SetMat4("model", model);
                glDrawArrays(GL_TRIANGLES, 0, 36);
                //lights[i].Draw(simpleShader);
            }
            glBindVertexArray(0);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}


void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, fDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, fDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, fDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, fDeltaTime);

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.bSprint = true;
    else
        camera.bSprint = false;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
}


void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (bFirstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        bFirstMouse = false;
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
