#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>

#include "glfwCallbacks.h"

#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Object.h"
#include "PointLight.h"


class Game
{
    private:
        unsigned int iScreenWidth  = 1920;
        unsigned int iScreenHeight = 1080;

        Camera camera;

        float fDeltaTime = 0.0f;
        float fLastFrame = 0.0f;

        float bFirstMouse = true;
        float lastX = iScreenWidth / 2;
        float lastY = iScreenHeight / 2;

        bool bShowLights = true;
        bool bFlashlight = false;
        bool bDebug = false;

    public:
        Game(int width, int height);
        ~Game();

        bool Construct();
        void Start();
        void Create();
        void Update();
        void Render();
};

#endif
