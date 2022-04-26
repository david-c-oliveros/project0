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

const unsigned int SCR_WIDTH  = 1920;
const unsigned int SCR_HEIGHT = 1080;

Camera camera(glm::vec3(0.0f, 1.5f, 3.0f));

float fDeltaTime = 0.0f;
float fLastFrame = 0.0f;

float bFirstMouse = true;
float lastX = SCR_WIDTH / 2;
float lastY = SCR_HEIGHT / 2;

bool bShowLights = false;


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
    Model hallway_Cross = Model("extra/models/hallway/Hallway_Cross.obj");
    Model hallway_Door = Model("extra/models/hallway/Hallway_Door.obj");
    Model hallway_End = Model("extra/models/hallway/Hallway_End.obj");
    Model hallway_L = Model("extra/models/hallway/Hallway_L.obj");
    Model hallway_Ramp = Model("extra/models/hallway/Hallway_Ramp.obj");
    Model hallway_Straight = Model("extra/models/hallway/Hallway_Straight.obj");
    

    std::vector<Object> cubes;
    for (int i = 0; i < 8; i++)
    {
        cubes.push_back(Object(cube, glm::vec3(2.0f * i, 0.0f, 0.0f)));
    }

    std::vector<Object> hallways;
    for (int i = 0; i < 8; i++)
    {
        hallways.push_back(Object(hallway_Straight, glm::vec3(0.0f, 0.0f, 10.0f * i)));
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
    Light dirLight = Light(DIR_LIGHT, glm::vec3(-0.2f, -1.0f, -0.3f));
    shader.SetVec3("dirLight.direction", dirLight.vPos);
    shader.SetVec3("dirLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
    shader.SetVec3("dirLight.diffuse", glm::vec3(0.1f, 0.1f, 0.1f));
    shader.SetVec3("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));

    std::vector<Light> pointLights = {
        Light(POINT_LIGHT, glm::vec3( 1.0f,  0.0f,  2.0f)),
        Light(POINT_LIGHT, glm::vec3( 2.0f,  3.0f,  4.0f)),
        Light(POINT_LIGHT, glm::vec3( 4.0f,  2.0f,  12.0f)),
        Light(POINT_LIGHT, glm::vec3( 7.0f,  0.0f,  3.0f))
    };

    for (int i = 0; i < pointLights.size(); i++)
    {
        std::string num = std::to_string(i);
        shader.SetVec3("pointLights[" + num + "].position", pointLights[i].vPos);
        shader.SetVec3("pointLights[" + num + "].ambient", glm::vec3(0.05, 0.05, 0.05));
        shader.SetVec3("pointLights[" + num + "].diffuse", glm::vec3(0.8, 0.8, 0.8));
        shader.SetVec3("pointLights[" + num + "].specular", glm::vec3(1.0, 1.0, 1.0));
        shader.SetFloat("pointLights[" + num + "].constant", 1.0f);
        shader.SetFloat("pointLights[" + num + "].linear", 1.09f);
        shader.SetFloat("pointLights[" + num + "].quadratic", 0.032f);
    }
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

        shader.SetVec3("spotLight.position", camera.Position);
        shader.SetVec3("spotLight.direction", camera.Front);
        shader.SetVec3("spotLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
        shader.SetVec3("spotLight.diffuse", glm::vec3(1.0f, 1.1f, 1.0f));
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

        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        //hallway_Cross.Draw(shader);
        //hallway_Door.Draw(shader);
        //hallway_End.Draw(shader);
        //hallway_L.Draw(shader);
        //hallway_Ramp.Draw(shader);
        //hallway_Straight.Draw(shader);

        for (int i = 0; i < hallways.size(); i++)
        {
            hallways[i].Draw(shader);
        }

        //for (int i = 0; i < cubes.size(); i++)
        //{
        //    cubes[i].Draw(shader);
        //}
        //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        if (bShowLights)
        {
            simpleShader.Use();
            simpleShader.SetMat4("projection", projection);
            simpleShader.SetMat4("view", view);
            glBindVertexArray(cubeVAO);
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, dirLight.vPos);
            simpleShader.SetMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            for (int i = 0; i < pointLights.size(); i++)
            {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, pointLights[i].vPos);
                model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
                simpleShader.SetMat4("model", model);
                glDrawArrays(GL_TRIANGLES, 0, 36);
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
