#include "Game.h"


Game::Game(int width, int height)
    : fDeltaTime(0.0f), fLastFrame(0.0f), bFirstMouse(true)
{
}


Game::~Game()
{
}


bool Game::Construct()
{
    /************************************************************************/
    /************************************************************************/
    /*                       Initialize GLFW and GLEW                       */  
    /************************************************************************/
    /************************************************************************/
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Welcome to Project 0", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        printf("\nERROR: Failed to initialize GLEW");
        return false;
    }



    /**********************************************************/
    /**********************************************************/
    /*                       GL Configs                       */  
    /**********************************************************/
    /**********************************************************/
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glEnable(GL_DEPTH_TEST);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    return true;
}


void Game::Create()
{
    camera = Camera(glm::vec3(0.0f, 1.5f, 48.0f));
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
    Model cModelHallway_Cross = Model("extra/models/hallway/Hallway_Cross.obj");
    Model cModelHallway_Door = Model("extra/models/hallway/Hallway_Door.obj");
    Model cModelHallway_End = Model("extra/models/hallway/Hallway_End.obj");
    Model cModelHallway_L = Model("extra/models/hallway/Hallway_L.obj");
    Model cModelHallway_Ramp = Model("extra/models/hallway/Hallway_Ramp.obj");
    Model cModelHallway_Straight = Model("extra/models/hallway/Hallway_Straight.obj");

    Model cEnv = Model("res/models/env_v01/game_env_v01.obj");
    

    /******************************************************************/
    /******************************************************************/
    /*                       Build Environment                        */  
    /******************************************************************/
    /******************************************************************/
    Object cScene = Object(cEnv, glm::vec3(0.0f, 0.0f, 0.0f), 0.0f);

    std::vector<Object> cubes;
    for (int i = 0; i < 8; i++)
    {
        cubes.push_back(Object(cube, glm::vec3(2.0f * i, 0.0f, 0.0f), 0.0f));
    }

    std::vector<Object> vHallways_Straight;
    for (int i = 0; i < 8; i++)
    {
        vHallways_Straight.push_back(Object(cModelHallway_Straight, glm::vec3(0.0f, 0.0f, i * 10.0f), 0.0f));
    }

    std::vector<Object> vHallways_L;
    float fStart = vHallways_Straight[vHallways_Straight.size() - 1].vPos.z;
    for (int i = 0; i < 1; i++)
    {
        vHallways_L.push_back(Object(cModelHallway_L, glm::vec3(-9.0f, 0.0f, fStart + 9.0f), 0.0f));
    }

    for (int i = 0; i < 4; i++)
    {
        vHallways_Straight.push_back(Object(cModelHallway_Straight, glm::vec3(fStart + 9.0f, 0.0f, 19.0f + i * 10.0f), -90.0f));
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
    //Light dirLight = Light(DIR_LIGHT, glm::vec3(-0.2f, -1.0f, -0.3f));
    glm::vec3 lightPos =  glm::vec3(-0.2f, -1.0f, -0.3f);
    shader.SetVec3("dirLight.direction", lightPos);
    shader.SetVec3("dirLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
    shader.SetVec3("dirLight.diffuse", glm::vec3(0.1f, 0.1f, 0.1f));
    shader.SetVec3("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));

    float fLightSepDist = 4.0f;
    std::vector<PointLight> pointLights;
    glm::vec3 colors[] = {
        glm::vec3(1.0, 0.0f, 0.0f),
        glm::vec3(0.0, 1.0f, 0.0f),
        glm::vec3(0.0, 0.0f, 1.0f),
    };

    pointLights.push_back(PointLight(0, glm::vec3(-0.22f,  3.5f, -36.5 + (0 * fLightSepDist))));
    pointLights.push_back(PointLight(1, glm::vec3(-0.22f,  3.5f, -36.5 + (1 * fLightSepDist))));
    pointLights.push_back(PointLight(2, glm::vec3(-0.22f,  3.5f, -34.8 + (2 * fLightSepDist))));
    pointLights.push_back(PointLight(3, glm::vec3(-0.22f,  3.5f, -36.5 + (3 * fLightSepDist))));
    pointLights.push_back(PointLight(4, glm::vec3(-0.22f,  3.5f, -36.5 + (4 * fLightSepDist))));
    pointLights.push_back(PointLight(5, glm::vec3(-0.22f,  3.5f, -34.8 + (5 * fLightSepDist))));
    pointLights.push_back(PointLight(6, glm::vec3(-0.22f,  3.5f, -34.8 + (6 * fLightSepDist))));
    pointLights.push_back(PointLight(7, glm::vec3(-0.22f,  3.5f, -34.8 + (7 * fLightSepDist))));

    for (int i = 0; i < pointLights.size(); i++)
    {
        pointLights[i].SetAllUniforms(shader);
        pointLights[i].SetDiffuse(shader, glm::vec3(0.1f, 0.1f, 0.1f));
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
}


void Game::Start()
{
    OnCreate();

    while (!glfwWindowShouldClose(window))
    {
        fDeltaTime = fCurrentFrame - fLastFrame;
        fLastFrame = fCurrentFrame;

        Update(fDeltaTime);
        Render();

        glfwTerminate();
    }
}


void Game::Update(float fDeltaTime)
{
    float fCurrentFrame = static_cast<float>(glfwGetTime());

    processInput(window);

    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.Use();
    shader.SetVec3("viewPos", camera.Position);
    shader.SetFloat("material.shininess", 32.0f);

    if (bDebug)
        shader.SetVec3("dirLight.ambient", glm::vec3(0.5f, 0.5f, 0.5f));
    else
        shader.SetVec3("dirLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));

    shader.SetVec3("spotLight.position", camera.Position);
    shader.SetVec3("spotLight.direction", camera.Front);
    if (bFlashlight)
    {
        shader.SetVec3("spotLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
        shader.SetVec3("spotLight.diffuse", glm::vec3(1.0f, 1.1f, 1.0f));
        shader.SetVec3("spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
    }
    else
    {
        shader.SetVec3("spotLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
        shader.SetVec3("spotLight.diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
        shader.SetVec3("spotLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
    }
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

    /*******************************************************/
    /*******************************************************/
    /*                       Render                        */  
    /*******************************************************/
    /*******************************************************/
    /******************************/
    /*        Draw Objects        */
    /******************************/
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    cScene.Draw(shader);

    if (bShowLights)
    {
        simpleShader.Use();
        simpleShader.SetMat4("projection", projection);
        simpleShader.SetMat4("view", view);
        glBindVertexArray(cubeVAO);
        //glm::mat4 model = glm::mat4(1.0f);
        //model = glm::translate(model, lightPos);
        //simpleShader.SetMat4("model", model);
        //glDrawArrays(GL_TRIANGLES, 0, 36);
        for (int i = 0; i < pointLights.size(); i++)
        {
            simpleShader.SetVec3("vColor", glm::vec3(1.0f, 1.0f, 1.0f));
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, pointLights[i].m_vPos);
            model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
            simpleShader.SetMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glBindVertexArray(0);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
}


void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, fDeltaTime, bDebug);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, fDeltaTime, bDebug);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, fDeltaTime, bDebug);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, fDeltaTime, bDebug);

    if (bDebug)
    {
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            camera.ProcessKeyboard(UP, fDeltaTime, bDebug);
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            camera.ProcessKeyboard(DOWN, fDeltaTime, bDebug);
    }

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
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        bFlashlight = !bFlashlight;
    if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
        bDebug = !bDebug;
    if (key == GLFW_KEY_L && action == GLFW_PRESS)
        bShowLights = !bShowLights;
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