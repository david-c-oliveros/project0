#include "PointLight.h"


PointLight::PointLight(int uIndex, glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse,
                       glm::vec3 specular, float constant, float linear, float quadratic)
    : iUniformIndex(uIndex), m_vPos(position), m_vAmbient(ambient), m_vDiffuse(diffuse),
      m_vSpecular(specular), m_fConstant(constant), m_fLinear(linear), m_fQuadratic(quadratic)
{
}


void PointLight::Draw(Shader &shader)
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
