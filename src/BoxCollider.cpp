#include "BoxCollider.h"


BoxCollider::BoxCollider(glm::vec3 origin, glm::vec3 size)
    : m_vOrigin(origin)
{
    m_vDim.x = size.x / 2;
    m_vDim.y = size.y / 2;
    m_vDim.z = size.z / 2;
}


BoxCollider::~BoxCollider()
{
}


void BoxCollider::UpdatePos(glm::vec3 newPos)
{
    m_vOrigin = newPos;
    vMin.x = m_vOrigin.x - m_vDim.x;
    vMin.y = m_vOrigin.y - m_vDim.y;
    vMin.z = m_vOrigin.z - m_vDim.z;
}


glm::vec3 BoxCollider::GetPos()
{
    return m_vOrigin;
}
