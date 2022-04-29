#include "Camera.h"


Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(WALK_SPEED), SprintSpeed(SPRINT_SPEED),
      MouseSensitivity(SENSITIVITY), Zoom(ZOOM), bSprint(false), bConstrainToFloor(true),
      cCollider(BoxCollider(position, glm::vec3(1.0f)))
{
    cCollider = BoxCollider(position, glm::vec3(1.0f));
    Position = position;
    Floor = position.y;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    //pCollider = std::make_unique<BoxCollider>(Position, glm::vec3(1.0f));
    updateCameraVectors();
}


Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(WALK_SPEED), SprintSpeed(SPRINT_SPEED),
      MouseSensitivity(SENSITIVITY), Zoom(ZOOM), bSprint(false), bConstrainToFloor(true),
      cCollider(BoxCollider(glm::vec3(posX, posY, posZ), glm::vec3(1.0f)))
{
    Position = glm::vec3(posX, posY, posZ);
    Floor = posY;
    WorldUp = glm::vec3(upX, upY, upZ);
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}


glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(Position, Position + Front, Up);
}


void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime, bool bDebug)
{
    float velocity = bSprint ? SprintSpeed * deltaTime : MovementSpeed * deltaTime;
    if (bDebug)
        velocity *= 4;

    if (direction == FORWARD)
        UpdatePos(Front * velocity);
    if (direction == BACKWARD)
        UpdatePos(-(Front * velocity));
    if (direction == LEFT)
        UpdatePos(-(Right * velocity));
    if (direction == RIGHT)
        UpdatePos(Right * velocity);

    if (direction == UP)
        Position += WorldUp * velocity;
    if (direction == DOWN)
        Position -= WorldUp * velocity;

    if (bConstrainToFloor && !bDebug)
        Position.y = Floor;
}


void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean bConstrainPitch)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw   += xoffset;
    Pitch += yoffset;

    if (bConstrainPitch)
    {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    updateCameraVectors();
}


void Camera::ProcessMouseScroll(float yoffset)
{
    Zoom -= (float)yoffset;

    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 45.0f)
        Zoom = 45.0f;
}


void Camera::updateCameraVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);

    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up    = glm::normalize(glm::cross(Right, Front));
}


void Camera::UpdatePos(glm::vec3 newPos)
{
    Position += newPos;
    cCollider.UpdatePos(Position);
}


void Camera::PrintPosition()
{
    std::cout << "(" << Position.x << ", " << Position.y << ", " << Position.z << ")" << std::endl;
}
