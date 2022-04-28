#include "Game.h"


unsigned int SCR_WIDTH  = 1920;
unsigned int SCR_HEIGHT = 1080;

Camera camera = Camera(glm::vec3(0.0f, 1.5f, 48.0f));

int iCreateCount = 0;
int iTicks = 0;

float fDeltaTime = 0.0f;
float fLastFrame = 0.0f;
float fCurrentFrame = 0.0f;

float bFirstMouse = true;
float lastX = SCR_WIDTH / 2;
float lastY = SCR_HEIGHT / 2;

float fTorchLevel = 8.0f;
glm::vec3 vTorchColor = glm::vec3(fTorchLevel);

bool bShowLights = true;
bool bFlashlight = false;
bool bDebug = false;


Game::Game()
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

    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Welcome to Project 0", NULL, NULL);

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
    iCreateCount++;
    /*****************************************************************/
    /*****************************************************************/
    /*                       Build Environment                       */  
    /*****************************************************************/
    /*****************************************************************/
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

    /*****************************/
    /*        Load Models        */
    /*****************************/
    cCube = std::make_unique<Model>("res/models/cube_scifi/obj/Cube.obj");
    cEnv = std::make_unique<Model>("res/models/env_v01/game_env_v02.obj");
    cScene.Create(std::move(cEnv), glm::vec3(0.0f, 0.0f, 0.0f), 0.0f);


    /*********************************************************/
    /*********************************************************/
    /*                       VAO, VBO                        */  
    /*********************************************************/
    /*********************************************************/
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
    shader.Create("shaders/vLight.shader", "shaders/fLight.shader");
    simpleShader.Create("shaders/vSimple.shader", "shaders/fSimple.shader");
    simpleShader.Use();
    simpleShader.SetVec3("color", glm::vec3(1.0f, 1.0f, 1.0f));
    shader.Use();
    stbi_set_flip_vertically_on_load(true);



    /*******************************************************/
    /*******************************************************/
    /*                       Lights                        */  
    /*******************************************************/
    /*******************************************************/
    //Light dirLight = Light(DIR_LIGHT, glm::vec3(-0.2f, -1.0f, -0.3f));
    glm::vec3 lightPos =  glm::vec3(-0.2f, -1.0f, -0.3f);
    shader.SetVec3("dirLight.direction", lightPos);
    shader.SetVec3("dirLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
    shader.SetVec3("dirLight.diffuse", glm::vec3(0.1f, 0.1f, 0.1f));
    shader.SetVec3("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));

    float fLightSepDist = 4.0f;
    glm::vec3 amb  = glm::vec3(0.0f);
    glm::vec3 diff = glm::vec3(0.1f);
    glm::vec3 spec = glm::vec3(1.0f);
    for (int i = 0; i < 16; i++)
    {
        pointLights.push_back(PointLight("res/models/env_v01/game_env_light_tubes.obj", i, glm::vec3(-0.22, 3.3f, -36.5 + (i * fLightSepDist)), amb, diff, spec));
    }

    for (int i = 0; i < pointLights.size(); i++)
    {
        pointLights[i].SetAllUniforms(shader);
        pointLights[i].ToggleEffects();
    }

    spotLights.push_back(SpotLight(0, camera.Position, camera.Front, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
    spotLights[0].SetAllUniforms(shader);
    spotLights[0].SetDiffuse(shader, vTorchColor);
    spotLights[0].SetSpecular(shader, vTorchColor);

    /*******************************/
    /*        Set Draw Mode        */
    /*******************************/
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}


void Game::Start()
{
    Create();

    while (!glfwWindowShouldClose(window))
    {
        fDeltaTime = fCurrentFrame - fLastFrame;
        fLastFrame = fCurrentFrame;

        Update(fDeltaTime);
        iTicks++;
    }
    glfwTerminate();
}


void Game::Update(float fDeltaTime)
{
    fCurrentFrame = static_cast<float>(glfwGetTime());

    for (int i = 0; i < pointLights.size(); i++)
    {
        pointLights[i].Update(shader);
    }

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

    spotLights[0].SetPosition(shader, camera.Position);
    spotLights[0].SetDirection(shader, camera.Front);
    if (bFlashlight)
    {
        spotLights[0].SetDiffuse(shader, vTorchColor);
        spotLights[0].SetSpecular(shader, vTorchColor);
    }
    else
    {
        spotLights[0].SetDiffuse(shader, glm::vec3(0.0f, 0.0f, 0.0f));
        spotLights[0].SetSpecular(shader, glm::vec3(0.0f, 0.0f, 0.0f));
    }


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
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    if (bShowLights)
    {
        simpleShader.Use();
        simpleShader.SetMat4("projection", projection);
        simpleShader.SetMat4("view", view);
        glBindVertexArray(cubeVAO);
        for (int i = 0; i < pointLights.size(); i++)
        {
            pointLights[i].Draw(simpleShader);
        }
        glBindVertexArray(0);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Game::processInput(GLFWwindow* window)
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




/************************************************************************/
/************************************************************************/
/*                       GLFW Callback Functions                        */  
/************************************************************************/
/************************************************************************/
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

    if (key == GLFW_KEY_UP)
    {
        fTorchLevel += 0.2f;
        vTorchColor = fTorchLevel * glm::vec3(1.0f, 1.0f, 1.0f);
        if (fTorchLevel > 16.0f)
        {
            fTorchLevel = 16.0f;
            vTorchColor = fTorchLevel * glm::vec3(1.0f, 1.0f, 1.0f);
        }
    }
    if (key == GLFW_KEY_DOWN)
    {
        fTorchLevel -= 0.2f;
        vTorchColor = fTorchLevel * glm::vec3(1.0f, 1.0f, 1.0f);
        if (fTorchLevel < 0.0f)
        {
            fTorchLevel = 0.0f;
            vTorchColor = fTorchLevel * glm::vec3(1.0f, 1.0f, 1.0f);
        }
    }
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
