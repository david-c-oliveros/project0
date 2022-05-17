#include "BoxCollider.h"


BoxCollider::BoxCollider(glm::vec3 origin, glm::vec3 size)
    : vOrigin(origin)
{
    vDim.x = size.x / 2;
    vDim.y = size.y / 2;
    vDim.z = size.z / 2;

    vMin = vOrigin - vDim;
    vMax = vOrigin + vDim;
}


BoxCollider::~BoxCollider()
{
}


void BoxCollider::UpdatePos(glm::vec3 newPos)
{
    vOrigin = newPos;
    vMin.x = vOrigin.x - vDim.x;
    vMin.y = vOrigin.y - vDim.y;
    vMin.z = vOrigin.z - vDim.z;

    vMax.x = vOrigin.x + vDim.x;
    vMax.y = vOrigin.y + vDim.y;
    vMax.z = vOrigin.z + vDim.z;
}


glm::vec3 BoxCollider::GetPos()
{
    return vOrigin;
}
