#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Shader.h"
#include "Object.h"
#include "TickTimer.h"


class PointLight
{
    private:
        float m_fIntensity;
        
        int iUniformIndex;

        std::string cLightModelPath;
        std::unique_ptr<Model> cLightModel;
        std::unique_ptr<Object> cLightObject;

        glm::vec3 m_vLightColor;

        glm::vec3 m_vAmbient;
        glm::vec3 m_vDiffuse;
        glm::vec3 m_vSpecular;

        float m_fConstant;
        float m_fLinear;
        float m_fQuadratic;

        TickTimer cFlickerTimer = TickTimer(25);
        bool m_bEffects;

    public:
        glm::vec3 m_vPos;
        glm::vec3 vCol;
        bool bOn = true;
        PointLight(std::string modelPath, int uIndex, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
                   glm::vec3 ambient = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 diffuse = glm::vec3(0.5f, 0.5f, 0.5f),
                   glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f));

        void Draw(Shader &shader);
        void SetAllUniforms(Shader &shader);
        void SetPosition(Shader &shader, glm::vec3 position);
        void SetAmbient(Shader &shader, glm::vec3 ambient);
        void SetDiffuse(Shader &shader, glm::vec3 diffuse);
        void SetSpecular(Shader &shader, glm::vec3 specular);

        void ToggleEffects();
        void Update(Shader &shader);
        void Flicker(Shader &shader);
};

#endif
