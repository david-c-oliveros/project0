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
#include "SpotLight.h"
#include "BoxCollider.h"
#include "Text.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


class Game
{
    private:
        GLFWwindow* window;
        Shader shader;
        Shader simpleShader;
        Shader textShader;

        std::vector<PointLight> pointLights;
        std::vector<SpotLight> spotLights;

        //std::unique_ptr<BoxCollider> cCollider;
        BoxCollider cCollider;

        std::unique_ptr<Model> cEnvMesh;
        std::unique_ptr<Model> cContainerMesh;
        std::unique_ptr<Model> cCubeMesh;
        Object cScene;
        Object cContainer;
        Object cCube;

        std::unique_ptr<Text> pText;

    public:
        Game();
        ~Game();

        bool Construct();
        void Start();
        void Create();
        void Update(float fDeltaTime);
        bool Collide(BoxCollider a, BoxCollider b);
        void processInput(GLFWwindow* window);

    private:
};

#endif
