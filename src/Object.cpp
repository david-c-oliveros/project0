#include "Object.h"


Object::Object(Model &modelMesh, float rotAngle)
    : m_pModelMesh(&modelMesh), vPos(glm::vec3(0.0f, 0.0f, 0.0f)), fRotAngle(rotAngle)
{
}


Object::Object(Model &modelMesh, glm::vec3 pos, float rotAngle)
    : m_pModelMesh(&modelMesh), vPos(pos), fRotAngle(rotAngle)
{
}


void Object::Draw(Shader &shader)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(fRotAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::translate(model, vPos);
    shader.SetMat4("model", model);
    m_pModelMesh->Draw(shader);
}
