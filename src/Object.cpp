#include "Object.h"


Object::Object(Model &modelMesh)
    : m_pModelMesh(&modelMesh), vPos(glm::vec3(0.0f, 0.0f, 0.0f))
{
}


Object::Object(Model &modelMesh, glm::vec3 pos)
    : m_pModelMesh(&modelMesh), vPos(pos)
{
}


void Object::Draw(Shader &shader)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, vPos);
    shader.SetMat4("model", model);
    m_pModelMesh->Draw(shader);
}
