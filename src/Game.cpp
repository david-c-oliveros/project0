#include "Game.h"


unsigned int SCR_WIDTH  = 1920;
unsigned int SCR_HEIGHT = 1080;

Camera camera = Camera(glm::vec3(0.0f, 1.5f, 16.0f));

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
bool bCollision = false;


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

    pText = std::make_unique<Text>(SCR_WIDTH, SCR_HEIGHT);



    /**********************************************************/
    /**********************************************************/
    /*                       GL Configs                       */  
    /**********************************************************/
    /**********************************************************/
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    return true;
}


void Game::Create()
{
    /*****************************************************************/
    /*****************************************************************/
    /*                       Build Environment                       */  
    /*****************************************************************/
    /*****************************************************************/
    /*****************************/
    /*        Load Models        */
    /*****************************/
    cContainerMesh = std::make_unique<Model>("res/models/cube_scifi/obj/Cube.obj");
    //cEnvMesh = std::make_unique<Model>("res/models/env_v01/game_env_v02.obj");
    cCubeMesh1 = std::make_unique<Model>("res/models/cube.obj");
    cCubeMesh2 = std::make_unique<Model>("res/models/cube.obj");

    //cScene.Create(std::move(cEnvMesh), glm::vec3(0.0f), 0.0f);
    cContainer.Create(std::move(cContainerMesh), glm::vec3(0.0f), 0.0f);
    cCube1.Create(std::move(cCubeMesh1), glm::vec3(0.0f), 0.0f);
    cCube2.Create(std::move(cCubeMesh2), glm::vec3(3.0f, 0.0f, 0.0f), 0.0f);

    cCollider1.UpdatePos(cCube1.vPos);
    cCollider2.UpdatePos(cCube2.vPos);

    //cCollider = std::make_unique<BoxCollider>(glm::vec3(0.0f), glm::vec3(1.0f));



    /*******************************************************/
    /*******************************************************/
    /*                       Shaders                       */  
    /*******************************************************/
    /*******************************************************/
    shader.Create("shaders/vLight.shader", "shaders/fLight.shader");

    simpleShader.Create("shaders/vSimple.shader", "shaders/fSimple.shader");
    simpleShader.Use();
    simpleShader.SetVec3("color", glm::vec3(1.0f, 1.0f, 1.0f));

    textShader.Create("shaders/vText.shader", "shaders/fText.shader");
    textShader.SetMat4("projection", pText->projection);

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
    shader.SetVec3("dirLight.ambient", glm::vec3(0.0f, 1.0f, 0.0f));
    shader.SetVec3("dirLight.diffuse", glm::vec3(0.0f, 0.1f, 0.0f));
    shader.SetVec3("dirLight.specular", glm::vec3(0.0f, 1.0f, 0.0f));

    float fLightSepDist = 4.0f;
    glm::vec3 amb  = glm::vec3(0.0f);
    glm::vec3 diff = glm::vec3(0.1f);
    glm::vec3 spec = glm::vec3(1.0f);
    for (int i = 0; i < 16; i++)
    {
        pointLights.push_back(PointLight("res/models/env_v01/game_env_light_tubes.obj", i,
                              glm::vec3(-0.22, 3.3f, -36.5 + (i * fLightSepDist)), amb, diff, spec));
    }

    for (int i = 0; i < pointLights.size(); i++)
    {
        pointLights[i].SetAllUniforms(shader);
        /************************************************/
        /*        Randomize Which Lights Flicker        */
        /************************************************/
        if ((int)(glm::linearRand(0, 100)) % 15 == 0)
            pointLights[i].ToggleEffects();
    }

    spotLights.push_back(SpotLight(0, camera.vPos, camera.Front, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
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
        bCollision = false;
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
    shader.SetVec3("viewPos", camera.vPos);
    shader.SetFloat("material.shininess", 32.0f);

    if (bDebug)
        shader.SetVec3("dirLight.ambient", glm::vec3(0.5f, 0.5f, 0.5f));
    else
        shader.SetVec3("dirLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));

    spotLights[0].SetPosition(shader, camera.vPos);
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
    //cScene.Draw(shader);
    //cContainer.Draw(shader);
    //colliders[0].pCube->Draw(shader);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    cCube1.Draw(shader);
    cCube2.Draw(shader);

    if (bShowLights)
    {
        simpleShader.Use();
        simpleShader.SetMat4("projection", projection);
        simpleShader.SetMat4("view", view);
        for (int i = 0; i < pointLights.size(); i++)
        {
            pointLights[i].Draw(simpleShader);
        }
        glBindVertexArray(0);
    }

    textShader.Use();
    //pText->RenderText(textShader, "Cube:  " + glm::to_string(cCube2.vPos), 25.0f, 50.0f, 0.7f, glm::vec3(0.5f, 0.8f, 0.2f));
    //pText->RenderText(textShader, "Cube Collider: " + glm::to_string(cCollider2.vOrigin), 25.0f, 20.0f, 0.7f, glm::vec3(0.5f, 0.8f, 0.2f));
    //pText->RenderText(textShader, "Col: " + glm::to_string(camera.cCollider.vOrigin), 25.0f, 110.0f, 0.7f, glm::vec3(0.5f, 0.8f, 0.2f));
    //pText->RenderText(textShader, "Del: " + glm::to_string(vPlayerDelta), 25.0f, 80.0f, 0.7f, glm::vec3(0.5f, 0.8f, 0.2f));
    if (bCollision)
        pText->RenderText(textShader, "Collision!", 25.0f, 80.0f, 0.7f, glm::vec3(0.5f, 0.8f, 0.2f));
    pText->RenderText(textShader, "Pos: " + glm::to_string(camera.vPos), 25.0f, 50.0f, 0.7f, glm::vec3(0.5f, 0.8f, 0.2f));
    //pText->RenderText(textShader, "Vel: " + glm::to_string(camera.Velocity), 25.0f, 20.0f, 0.7f, glm::vec3(0.5f, 0.8f, 0.2f));

    glfwSwapBuffers(window);
    glfwPollEvents();
}


bool Game::AABBCollide(BoxCollider a, BoxCollider b)
{
    std::cout << "a min: " << glm::to_string(a.vMin) << std::endl;
    std::cout << "a max: " << glm::to_string(a.vMax) << std::endl;
    std::cout << "b min: " << glm::to_string(b.vMin) << std::endl;
    std::cout << "b max: " << glm::to_string(b.vMax) << std::endl;
    return (a.vMin.x < b.vMax.x && a.vMax.x > b.vMin.x) &&
           (a.vMin.y < b.vMax.y && a.vMax.y > b.vMin.y) &&
           (a.vMin.z < b.vMax.z && a.vMax.z > b.vMin.z);
}


glm::vec3 Game::CalcAABBDistanceTo(BoxCollider a, BoxCollider b)
{
    glm::vec3 vDelta = glm::vec3(0.0f);

    /************************/
    /*        X Axis        */
    /************************/
    if (a.vMin.x < b.vMin.x)
    {
        vDelta.x = b.vMin.x - a.vMax.x;
    }
    else if (a.vMin.x > b.vMin.x)
    {
        vDelta.x = a.vMin.x - b.vMax.x;
    }

    /************************/
    /*        Y Axis        */
    /************************/
    //if (a.vOrigin.y < b.vOrigin.y)
    //{
    //    vDelta.y = b.vOrigin.y - (a.vOrigin.y + a.vDim.y);
    //}
    //else if (a.vOrigin.y > b.vOrigin.y)
    //{
    //    vDelta.y = a.vOrigin.y - (b.vOrigin.y + b.vDim.y);
    //}

    /************************/
    /*        Y Axis        */
    /************************/
    if (a.vMin.y < b.vMin.y)
    {
        vDelta.y = b.vMin.y - a.vMax.y;
    }
    else if (a.vMin.y > b.vMin.y)
    {
        vDelta.y = a.vMin.y - b.vMax.y;
    }

    /************************/
    /*        Z Axis        */
    /************************/
    if (a.vMin.z < b.vMin.z)
    {
        vDelta.z = b.vMin.z - a.vMax.z;
    }
    else if (a.vMin.z > b.vMin.z)
    {
        vDelta.z = a.vMin.z - b.vMax.z;
    }

    return vDelta;
}


glm::vec3 Game::ResolveCollisions(BoxCollider a, glm::vec3 aVel, BoxCollider b)
{
    glm::vec3 vDistance = CalcAABBDistanceTo(a, b);
    glm::vec3 vMoveDelta = glm::vec3(0.0f);
    glm::vec3 vTimeToCollide = glm::vec3(0.0f);
    vTimeToCollide.x = aVel.x != 0.0f ? abs(vDistance.x / aVel.x) : 0.0f;
    vTimeToCollide.z = aVel.z != 0.0f ? abs(vDistance.z / aVel.z) : 0.0f;

    float fShortestTime = 0.0f;
    if (aVel.x != 0.0f && aVel.z == 0.0f)
    {
        fShortestTime = vTimeToCollide.x;
        vMoveDelta.x = fShortestTime * aVel.x;
    }
    else if (aVel.x == 0.0f && aVel.z != 0.0f)
    {
        fShortestTime = vTimeToCollide.z;
        vMoveDelta.z = fShortestTime * aVel.z;
    }
    else
    {
        fShortestTime = std::min(abs(vTimeToCollide.x), abs(vTimeToCollide.z));
        vMoveDelta.x = fShortestTime * aVel.x;
        vMoveDelta.z = fShortestTime * aVel.z;
    }

    return vMoveDelta;
}


void Game::processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    bool bCollide = false;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(FORWARD, fDeltaTime, bDebug);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(BACKWARD, fDeltaTime, bDebug);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(LEFT, fDeltaTime, bDebug);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(RIGHT, fDeltaTime, bDebug);
    }


    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        cCube2.vNextPos += glm::vec3(-0.1f, 0.0f, 0.0f);
        cCollider2.UpdatePos(cCube2.vNextPos);
        cCube2.vVel = glm::vec3(-0.1f, 0.0f, 0.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        cCube2.vNextPos += glm::vec3(0.1f, 0.0f, 0.0f);
        cCollider2.UpdatePos(cCube2.vNextPos);
        cCube2.vVel = glm::vec3(0.1f, 0.0f, 0.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        cCube2.vNextPos += glm::vec3(0.0f, 0.0f, 0.1f);
        cCollider2.UpdatePos(cCube2.vNextPos);
        cCube2.vVel = glm::vec3(0.0f, 0.0f, 0.1f);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        cCube2.vNextPos += glm::vec3(0.0f, 0.0f, -0.1f);
        cCollider2.UpdatePos(cCube2.vNextPos);
        cCube2.vVel = glm::vec3(0.0f, 0.0f, -0.1f);
    }


    /************************************/
    /*        Resolve Collisions        */
    /************************************/
    if (AABBCollide(cCollider1, camera.cCollider))
    {
        bCollision = true;
        glm::vec3 vVelocityVec = ResolveCollisions(camera.cCollider, camera.vVel, cCollider1);
        shader.SetVec3("dirLight.ambient", glm::vec3(0.1f, 0.0f, 0.0f));
        shader.SetVec3("dirLight.diffuse", glm::vec3(0.1f, 0.0f, 0.0f));
        shader.SetVec3("dirLight.specular", glm::vec3(1.0f, 0.0f, 0.0f));
        //camera.vPos += vVelocityVec;
        camera.vNextPos = camera.vPos;
        camera.cCollider.UpdatePos(camera.vPos);
    }
    else
    {
        camera.vPos = camera.vNextPos;
        camera.cCollider.UpdatePos(camera.vPos);
    }

    if (AABBCollide(cCollider1, cCollider2))
    {
        glm::vec3 vVelocityVec = ResolveCollisions(cCollider2, cCube2.vVel, cCollider1);
        cCube2.vPos += vVelocityVec;
        cCube2.vNextPos = cCube2.vPos;
        cCollider2.UpdatePos(cCube2.vPos);
    }
    else
    {
        cCube2.vPos = cCube2.vNextPos;
        cCollider2.UpdatePos(cCube2.vPos);
    }

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


bool AABBAndAABB(BoxCollider b1, BoxCollider b2)
{
    std::vector<glm::vec2> axesToTest = {
        glm::vec2(0.0f, 1.0f),
        glm::vec2(1.0f, 0.0f)
    };

    for (int i = 0; i < axesToTest.size(); i++)
    {
        if (!OverlapOnAxis(b1, b2, axesToTest[i]))
        {
            return false;
        }
        return true;
    }
    return true;
}


bool OverlapOnAxis(BoxCollider b1, BoxCollider b2, glm::vec2 axis)
{
    glm::vec2 interval1 = GetInterval(b1, axis);
    glm::vec2 interval2 = GetInterval(b2, axis);

    return ((interval2.x <= interval1.y) && (interval1.x <= interval2.y));
}


glm::vec2 GetInterval(BoxCollider rect, glm::vec2 axis)
{
    glm::vec2 result = glm::vec2(0.0f);

    glm::vec2 min = glm::vec2(rect.vMin.x, rect.vMin.z);
    glm::vec2 max = glm::vec2(rect.vMax.x, rect.vMax.z);

    glm::vec2 vertices[] = {
        glm::vec2(min.x, min.y), glm::vec2(min.x, max.y),
        glm::vec2(max.x, min.y), glm::vec2(max.x, max.y)
    };

    result.x = glm::dot(axis, vertices[0]);
    result.y = result.x;
    for (int i = 0; i < 4; i++)
    {
        float projection = glm::dot(axis, vertices[i]);
        if (projection < result.x)
        {
            result.x = projection;
        }
        if (projection > result.y)
        {
            result.y = projection;
        }
    }

    return result;
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
