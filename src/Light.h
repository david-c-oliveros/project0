#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Shader.h"


enum LightType
{
    DIR_LIGHT,
    POINT_LIGHT,
    SPOT_LIGHT
};


class Light
{
    private:
        LightType m_eType;
        float m_fIntensity;
        float m_LightCube[180];

        glm::vec3 m_vVertexPos;
        glm::vec3 m_vVertexNormal;

    public:
        glm::vec3 vPos;
        Light(LightType type);
        Light(LightType type, glm::vec3 position);

        void Draw(Shader &shader);
};

#endif
