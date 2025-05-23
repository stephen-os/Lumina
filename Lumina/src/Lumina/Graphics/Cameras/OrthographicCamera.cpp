#include "OrthographicCamera.h"

namespace Lumina
{
    OrthographicCamera::OrthographicCamera()
        : Camera(),
        m_Left(-10.0f), m_Right(10.0f),
        m_Bottom(-10.0f), m_Top(10.0f),
        m_Near(0.1f), m_Far(100.0f)
    {
        SetOrthographicProjection(m_Left, m_Right, m_Bottom, m_Top, m_Near, m_Far);
    }

    OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, float near, float far)
        : Camera(),
        m_Left(left), m_Right(right),
        m_Bottom(bottom), m_Top(top),
        m_Near(near), m_Far(far)
    {
        SetOrthographicProjection(left, right, bottom, top, near, far);
    }

    void OrthographicCamera::SetProjectionMatrix(float fov, float aspect, float near, float far)
    {
        // Convert a perspective-style input into orthographic projection
        // This is a convenience method that allows using the same interface as PerspectiveCamera
        float size = fov * 0.05f; // Convert FOV to orthographic size (arbitrary scale)
        float width = size * aspect;
        float height = size;

        m_Left = -width;
        m_Right = width;
        m_Bottom = -height;
        m_Top = height;
        m_Near = near;
        m_Far = far;

        SetOrthographicProjection(m_Left, m_Right, m_Bottom, m_Top, m_Near, m_Far);
    }

    void OrthographicCamera::SetOrthographicProjection(float left, float right, float bottom, float top, float near, float far)
    {
        m_Left = left;
        m_Right = right;
        m_Bottom = bottom;
        m_Top = top;
        m_Near = near;
        m_Far = far;

        m_ProjectionMatrix = glm::ortho(left, right, bottom, top, near, far);
    }
}