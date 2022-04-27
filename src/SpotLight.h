#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Shader.h"


class SpotLight
{
    private:
        float m_fIntensity;
        
        int iUniformIndex;

        glm::vec3 m_vAmbient;
        glm::vec3 m_vDiffuse;
        glm::vec3 m_vSpecular;

        float m_fConstant;
        float m_fLinear;
        float m_fQuadratic;

        float m_fCutOff;
        float m_fOuterCutOff;

    public:
        glm::vec3 m_vPos;
        glm::vec3 m_vDir;

        SpotLight(int uIndex, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 direction = glm::vec3(1.0f, 0.0f, 0.0f),
                   glm::vec3 ambient = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 diffuse = glm::vec3(0.5f, 0.5f, 0.5f),
                   glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f), float constant = 1.0f, float linear = 1.09f,
                   float quadratic = 0.032f, float cutOff = 12.5f, float OuterCutOff = 15.0f);
        ~SpotLight();

        void Draw(Shader &shader);
        void SetAllUniforms(Shader &shader);
        void SetPosition(Shader &shader, glm::vec3 position);
        void SetDirection(Shader &shader, glm::vec3 direction);
        void SetAmbient(Shader &shader, glm::vec3 ambient);
        void SetDiffuse(Shader &shader, glm::vec3 diffuse);
        void SetSpecular(Shader &shader, glm::vec3 specular);
        void SetCutOff(Shader &shader, float cutOff);
};

#endif
