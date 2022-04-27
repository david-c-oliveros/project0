#include "SpotLight.h"


SpotLight::SpotLight(int uIndex, glm::vec3 position, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse,
                       glm::vec3 specular, float constant, float linear, float quadratic, float cutOff, float outerCutOff)
    : iUniformIndex(uIndex), m_vPos(position), m_vDir(direction), m_vAmbient(ambient), m_vDiffuse(diffuse),
      m_vSpecular(specular), m_fConstant(constant), m_fLinear(linear), m_fQuadratic(quadratic), m_fCutOff(cutOff), m_fOuterCutOff(outerCutOff)
{
}


SpotLight::~SpotLight()
{
}


void SpotLight::Draw(Shader &shader)
{
    //shader.Use();
    //glm::mat4 model = glm::mat4(1.0f);
    //model = glm::translate(model, vPos);
    //model = glm::scale(model, glm::vec3(0.2, 0.2, 0.2));
    //std::cout << "\nPosition: " << glm::to_string(vPos) << std::endl;
    //std::cout << "Model matrix:\n" << glm::to_string(model) << std::endl;
    //shader.SetMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}


void SpotLight::SetPosition(Shader &shader, glm::vec3 position)
{
    m_vPos = position;
    std::string i = std::to_string(iUniformIndex);
    shader.SetVec3("spotLights[" + i + "].position", m_vPos);
}


void SpotLight::SetDirection(Shader &shader, glm::vec3 direction)
{
    m_vDir = direction;
    std::string i = std::to_string(iUniformIndex);
    shader.SetVec3("spotLights[" + i + "].direction", m_vDir);
}


void SpotLight::SetAmbient(Shader &shader, glm::vec3 ambient)
{
    m_vAmbient = ambient;
    std::string i = std::to_string(iUniformIndex);
    shader.SetVec3("spotLights[" + i + "].ambient", m_vAmbient);
}


void SpotLight::SetDiffuse(Shader &shader, glm::vec3 diffuse)
{
    m_vDiffuse = diffuse;
    std::string i = std::to_string(iUniformIndex);
    shader.SetVec3("spotLights[" + i + "].diffuse", m_vDiffuse);
}

void SpotLight::SetSpecular(Shader &shader, glm::vec3 specular)
{
    m_vSpecular = specular;
    std::string i = std::to_string(iUniformIndex);
    shader.SetVec3("spotLights[" + i + "].specular", m_vSpecular);
}


void SpotLight::SetCutOff(Shader &shader, float cutOff)
{
    m_fCutOff = cutOff;
    std::string i = std::to_string(iUniformIndex);
    shader.SetFloat("spotLights[" + i + "].cutOff", glm::cos(glm::radians(m_fCutOff)));
}


void SpotLight::SetAllUniforms(Shader &shader)
{
    std::string i = std::to_string(iUniformIndex);
    shader.SetVec3("spotLights[" + i + "].position", m_vPos);
    shader.SetVec3("spotLights[" + i + "].direction", m_vDir);
    shader.SetVec3("spotLights[" + i + "].ambient", m_vAmbient);
    shader.SetVec3("spotLights[" + i + "].diffuse", m_vDiffuse);
    shader.SetVec3("spotLights[" + i + "].specular", m_vSpecular);
    shader.SetFloat("spotLights[" + i + "].constant", m_fConstant);
    shader.SetFloat("spotLights[" + i + "].linear", m_fLinear);
    shader.SetFloat("spotLights[" + i + "].quadratic", m_fQuadratic);
    shader.SetFloat("spotLights[" + i + "].cutOff", glm::cos(glm::radians(m_fCutOff)));
    shader.SetFloat("spotLights[" + i + "].outerCutOff", glm::cos(glm::radians(m_fOuterCutOff)));
}
