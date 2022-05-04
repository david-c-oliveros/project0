#include "Object.h"


Object::Object()
{
}


Object::Object(std::unique_ptr<Model> modelMesh, float rotAngle)
    : m_pModelMesh(std::move(modelMesh)), vPos(glm::vec3(0.0f, 0.0f, 0.0f)), fRotAngle(rotAngle)
{
    vNextPos = vPos;
}


Object::Object(std::unique_ptr<Model> modelMesh, glm::vec3 pos, float rotAngle)
    : m_pModelMesh(std::move(modelMesh)), vPos(pos), fRotAngle(rotAngle)
{
    vNextPos = vPos;
}


void Object::Create(std::unique_ptr<Model> modelMesh, glm::vec3 pos, float rotAngle)
{
    m_pModelMesh = std::move(modelMesh);
    vPos = pos;
    vVel = glm::vec3(0.0f);
    fRotAngle = rotAngle;
}


void Object::Draw(Shader &shader)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(fRotAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::translate(model, vPos);
    shader.SetMat4("model", model);
    m_pModelMesh->Draw(shader);
}
