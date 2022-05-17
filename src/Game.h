#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <memory>
#include <algorithm>

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

bool AABBAndAABB(BoxCollider b1, BoxCollider b2);
bool OverlapOnAxis(BoxCollider b1, BoxCollider b2, glm::vec2 axis);
glm::vec2 GetInterval(BoxCollider rect, glm::vec2 axis);


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
        BoxCollider cCollider1;
        BoxCollider cCollider2;

        std::unique_ptr<Model> cEnvMesh;
        std::unique_ptr<Model> cContainerMesh;
        std::unique_ptr<Model> cCubeMesh1;
        std::unique_ptr<Model> cCubeMesh2;
        Object cScene;
        Object cContainer;
        Object cCube1;
        Object cCube2;

        glm::vec3 vPlayerDelta;

        std::unique_ptr<Text> pText;

    public:
        Game();
        ~Game();

        bool Construct();
        void Start();
        void Create();
        void Update(float fDeltaTime);
        bool AABBCollide(BoxCollider a, BoxCollider b);
        glm::vec3 CalcAABBDistanceTo(BoxCollider a, BoxCollider b);
        glm::vec3 ResolveCollisions(BoxCollider a, glm::vec3 aVel, BoxCollider b);
        void processInput(GLFWwindow* window);

    private:
};

#endif
