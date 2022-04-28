#include "PointLight.h"


PointLight::PointLight(std::string modelPath, int uIndex, glm::vec3 position,
                       glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
    : iUniformIndex(uIndex), m_vPos(position), m_vAmbient(ambient), m_vDiffuse(diffuse),
      m_vSpecular(specular), m_fConstant(1.0f), m_fLinear(1.0f), m_fQuadratic(0.032f),
      m_vLightColor(diffuse), m_bEffects(false)
{
    vCol = m_vLightColor;
    cLightModel = std::make_unique<Model>(modelPath);
    cLightObject = std::make_unique<Object>(std::move(cLightModel), m_vPos, 0.0f);
}


void PointLight::Draw(Shader &shader)
{
    shader.SetVec3("vColor", 10.0f * vCol);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, m_vPos);
    model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
    shader.SetMat4("model", model);
    cLightObject->Draw(shader);
}


void PointLight::SetPosition(Shader &shader, glm::vec3 position)
{
    m_vPos = position;
    std::string i = std::to_string(iUniformIndex);
    shader.SetVec3("pointLights[" + i + "].position", m_vPos);
}


void PointLight::SetAmbient(Shader &shader, glm::vec3 ambient)
{
    m_vAmbient = ambient;
    std::string i = std::to_string(iUniformIndex);
    shader.SetVec3("pointLights[" + i + "].ambient", m_vAmbient);
}


void PointLight::SetDiffuse(Shader &shader, glm::vec3 diffuse)
{
    m_vDiffuse = diffuse;
    std::string i = std::to_string(iUniformIndex);
    shader.SetVec3("pointLights[" + i + "].diffuse", m_vDiffuse);
}

void PointLight::SetSpecular(Shader &shader, glm::vec3 specular)
{
    m_vSpecular = specular;
    std::string i = std::to_string(iUniformIndex);
    shader.SetVec3("pointLights[" + i + "].specular", m_vSpecular);
}


void PointLight::SetAllUniforms(Shader &shader)
{
    std::string i = std::to_string(iUniformIndex);
    shader.SetVec3("pointLights[" + i + "].position", m_vPos);
    shader.SetVec3("pointLights[" + i + "].ambient", m_vAmbient);
    shader.SetVec3("pointLights[" + i + "].diffuse", m_vDiffuse);
    shader.SetVec3("pointLights[" + i + "].specular", m_vSpecular);
    shader.SetFloat("pointLights[" + i + "].constant", m_fConstant);
    shader.SetFloat("pointLights[" + i + "].linear", m_fLinear);
    shader.SetFloat("pointLights[" + i + "].quadratic", m_fQuadratic);
}


void PointLight::ToggleEffects()
{
    m_bEffects = !m_bEffects;
    if (m_bEffects)
        cFlickerTimer.Start();
    else
        cFlickerTimer.Reset();
}


void PointLight::Update(Shader &shader)
{
    if (m_bEffects)
        Flicker(shader);
}


void PointLight::Flicker(Shader &shader)
{
    shader.Use();
    if (cFlickerTimer.Check())
    {
        bOn = !bOn;
        float fNextVal = glm::linearRand(0.5f, 1.0f);
        vCol = fNextVal * m_vLightColor;
        glm::vec3 vSpec = 10.0f * vCol;

        SetDiffuse(shader, vCol);
        SetSpecular(shader, vSpec);

        int fNextInterval = glm::linearRand(3.0f, 8.0f);

        cFlickerTimer.Reset();
        cFlickerTimer.ChangeMax(fNextInterval);
        cFlickerTimer.Start();
    }
    else
    {
        cFlickerTimer.Update();
    }
}
