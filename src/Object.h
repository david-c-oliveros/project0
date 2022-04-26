#ifndef OBJECT_H
#define OBJECT_H

#include <memory>

#include "Model.h"


class Object
{
    private:
        Model* m_pModelMesh;

    public:
        Object(Model &modelMesh);
        Object(Model &modelMesh, glm::vec3 position);

        void Draw(Shader &shader);

        glm::vec3 vPos;
};

#endif
