// Camera.h
#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

namespace Lumina
{
    class Camera
    {
    public:
        Camera();
        virtual ~Camera() = default;

        virtual void SetPosition(const glm::vec3& position);
        virtual void SetRotation(float pitch, float yaw);
        virtual void Strafe(float distance);
        virtual void Advance(float distance);
        virtual void Pitch(float degrees);
        virtual void Yaw(float degrees);

        // This becomes pure virtual to force derived classes to implement their own projection logic
        virtual void SetProjectionMatrix(float fov, float aspect, float near, float far) = 0;

        const glm::mat4& GetProjectionMatrix() const;
        const glm::mat4& GetViewMatrix() const;
        std::string GetProjMatrixToString() const;
        std::string GetViewMatrixToString() const;
        glm::vec3 GetPosition() const { return m_Position; }

        void HandleKeyInput(const float& distance);
        void HandleMouseInput(const float& sensitivity);
        void HandleScrollInput(const float& sensitivity);

    protected:
        std::string MatrixToString(const std::string& name, const glm::mat4& matrix) const;
        virtual void UpdateViewMatrix();

        glm::mat4 m_ProjectionMatrix;
        glm::mat4 m_ViewMatrix;
        glm::vec3 m_Position;
        glm::vec3 m_Front;
        glm::vec3 m_Up;
        glm::vec3 m_Right;

        // Keyboard
        bool m_MoveForward = false;
        bool m_MoveBackward = false;
        bool m_MoveLeft = false;
        bool m_MoveRight = false;

        // Mouse
        glm::vec2 m_CurrentMousePos = glm::vec2(0.0f, 0.0f);
        glm::vec2 m_OriginalMousePos = glm::vec2(0.0f, 0.0f);
        bool m_IsMouseDown = false;
        bool m_IsFirstMouse = true;
    };
}