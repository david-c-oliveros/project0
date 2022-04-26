#ifndef OBJECT_H
#define OBJECT_H

#include "Model.h"


class Object
{
    private:
        Model* m_pModelMesh;

    public:
        glm::vec3 vPos;
        float fRotAngle;

    public:
        Object(Model &modelMesh, float rotAngle);
        Object(Model &modelMesh, glm::vec3 position, float rotAngle);

        void Draw(Shader &shader);
};

#endif
