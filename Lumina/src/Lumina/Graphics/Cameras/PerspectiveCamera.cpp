#include "PerspectiveCamera.h"

namespace Lumina
{
    PerspectiveCamera::PerspectiveCamera()
        : Camera()
    {
        // Default perspective projection
        SetProjectionMatrix(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
    }

    PerspectiveCamera::PerspectiveCamera(float fov, float aspect, float near, float far)
        : Camera()
    {
        SetProjectionMatrix(fov, aspect, near, far);
    }

    void PerspectiveCamera::SetProjectionMatrix(float fov, float aspect, float near, float far)
    {
        m_ProjectionMatrix = glm::perspective(glm::radians(fov), aspect, near, far);
    }
}