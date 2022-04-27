#ifndef OBJECT_H
#define OBJECT_H

#include "Model.h"


class Object
{
    private:
        std::unique_ptr<Model> m_pModelMesh;

    public:
        glm::vec3 vPos;
        float fRotAngle;

    public:
        Object();
        Object(std::unique_ptr<Model> modelMesh, float rotAngle);
        Object(std::unique_ptr<Model> modelMesh, glm::vec3 pos, float rotAngle);

        void Create(std::unique_ptr<Model> modelMesh, glm::vec3 pos, float rotAngle);
        void Draw(Shader &shader);
};

#endif
