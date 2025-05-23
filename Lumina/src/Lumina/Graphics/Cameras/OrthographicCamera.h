#pragma once
#include "Camera.h"

namespace Lumina
{
    class OrthographicCamera : public Camera
    {
    public:
        OrthographicCamera();
        OrthographicCamera(float left, float right, float bottom, float top, float near, float far);

        // Override the pure virtual function from the base class
        void SetProjectionMatrix(float fov, float aspect, float near, float far) override;

        // Additional function specific to orthographic cameras
        void SetOrthographicProjection(float left, float right, float bottom, float top, float near, float far);

        // Getters for orthographic boundaries
        float GetLeft() const { return m_Left; }
        float GetRight() const { return m_Right; }
        float GetBottom() const { return m_Bottom; }
        float GetTop() const { return m_Top; }
        float GetNear() const { return m_Near; }
        float GetFar() const { return m_Far; }

    private:
        // Orthographic projection boundaries
        float m_Left;
        float m_Right;
        float m_Bottom;
        float m_Top;
        float m_Near;
        float m_Far;
    };
}