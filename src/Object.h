#ifndef OBJECT_H
#define OBJECT_H

#include <memory>

#include "Model.h"


class Object
{
    private:
        glm::vec3 m_vPos;
        Model* m_pModelMesh;

    public:
        Object(Model &modelMesh);
        Object(Model &modelMesh, glm::vec3 position);

        void Draw(Shader &shader);
};

#endif
