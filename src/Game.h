#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <memory>
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
#include "PointLight.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


class Game
{
    private:
        GLFWwindow* window;
        //std::shared_ptr<Shader> shader;
        //std::shared_ptr<Shader> simpleShader;
        Shader shader;
        Shader simpleShader;

        std::vector<PointLight> pointLights;

        std::unique_ptr<Model> cEnv;
        //std::shared_ptr<Object> cScene;
        Object cScene;

        unsigned int cubeVBO;
        unsigned int cubeVAO;

    public:
        Game();
        ~Game();

        bool Construct();
        void Start();
        void Create();
        void Update(float fDeltaTime);
        void processInput(GLFWwindow* window);

    private:
};

#endif
