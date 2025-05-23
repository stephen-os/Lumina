#pragma once

#include "Camera.h"

namespace Lumina
{
    class PerspectiveCamera : public Camera
    {
    public:
        PerspectiveCamera();
        PerspectiveCamera(float fov, float aspect, float near, float far);

        // Override the pure virtual function from the base class
        void SetProjectionMatrix(float fov, float aspect, float near, float far) override;
    };
}