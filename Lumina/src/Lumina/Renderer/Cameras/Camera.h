#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

class Camera
{
public:
    Camera();

    void SetPosition(const glm::vec3& position);
    void SetRotation(float pitch, float yaw);

    void Strafe(float distance);
    void Advance(float distance);
    void Pitch(float degrees);
    void Yaw(float degrees);

    void SetProjectionMatrix(float fov, float aspect, float near, float far);

    const glm::mat4& GetProjectionMatrix() const;
    const glm::mat4& GetViewMatrix() const;

    std::string GetProjMatrixToString() const;
    std::string GetViewMatrixToString() const;

    glm::vec3 GetPosition() { return m_Position; }

    void HandleKeyInput(const float& distance); 
    void HandleMouseInput(const float& sensitivity);
 
private:
    std::string MatrixToString(const std::string& name, const glm::mat4& matrix) const;
    void UpdateViewMatrix();

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