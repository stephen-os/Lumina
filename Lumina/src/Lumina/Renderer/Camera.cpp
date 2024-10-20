#include "Camera.h"

#include <sstream>
#include <iomanip>
#include <iostream>

#include <imgui.h>
#include <GLFW/glfw3.h>

#define KEY_W GLFW_KEY_W
#define KEY_S GLFW_KEY_S

#define KEY_D GLFW_KEY_D
#define KEY_A GLFW_KEY_A

#define KEY_ESC GLFW_KEY_ESCAPE

Camera::Camera(float fov, float aspect, float near, float far)
    : m_ProjectionMatrix(glm::perspective(glm::radians(fov), aspect, near, far)),
    m_ViewMatrix(1.0f),
    m_Position(0.0f, 0.0f, 0.0f),
    m_Front(0.0f, 0.0f, -1.0f),
    m_Up(0.0f, 1.0f, 0.0f)
{
    m_Right = glm::normalize(glm::cross(m_Front, m_Up));
    UpdateViewMatrix();
}

void Camera::SetProjectionMatrix(float fov, float aspect, float near, float far)
{
    m_ProjectionMatrix = glm::mat4(glm::perspective(glm::radians(fov), aspect, near, far));
    UpdateViewMatrix();
}

void Camera::SetPosition(const glm::vec3& position)
{
    m_Position = position;
    UpdateViewMatrix();
}

void Camera::SetRotation(float pitch, float yaw)
{
    m_Front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    m_Front.y = sin(glm::radians(pitch));
    m_Front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    m_Front = glm::normalize(m_Front);
    m_Right = glm::normalize(glm::cross(m_Front, m_Up));  // Update m_Right
    UpdateViewMatrix();
}

void Camera::Pitch(float degrees)
{
    float radians = glm::radians(degrees);
    glm::vec3 right = glm::normalize(glm::cross(m_Front, m_Up));
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), radians, right);
    m_Front = glm::normalize(glm::vec3(rotation * glm::vec4(m_Front, 0.0f)));
    m_Right = glm::normalize(glm::cross(m_Front, m_Up));  // Update m_Right
    UpdateViewMatrix();
}

void Camera::Yaw(float degrees)
{
    float radians = glm::radians(degrees);
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), radians, m_Up);
    m_Front = glm::normalize(glm::vec3(rotation * glm::vec4(m_Front, 0.0f)));
    m_Right = glm::normalize(glm::cross(m_Front, m_Up));  // Update m_Right
    UpdateViewMatrix();
}

void Camera::Strafe(float distance)
{
    m_Position += m_Right * distance;
    UpdateViewMatrix();
}

void Camera::Advance(float distance)
{
    m_Position += m_Front * distance;
    UpdateViewMatrix();
}

const glm::mat4& Camera::GetProjectionMatrix() const { return m_ProjectionMatrix; }
const glm::mat4& Camera::GetViewMatrix() const { return m_ViewMatrix; }

std::string Camera::GetProjMatrixToString() const {
    return MatrixToString("Projection Matrix", m_ProjectionMatrix);
}

std::string Camera::GetViewMatrixToString() const {
    return MatrixToString("View Matrix", m_ViewMatrix);
}

std::string Camera::MatrixToString(const std::string& name, const glm::mat4& matrix) const
{
    std::stringstream ss;
    ss << std::fixed << std::setprecision(3);

    ss << name << "\n"; 
    ss << "|-----------------------------|\n";

    for (int i = 0; i < 4; ++i) {
        ss << "| ";
        for (int j = 0; j < 4; ++j) {
            float value = matrix[j][i];
            if (value < 0) {
                ss << "-" << std::abs(value) << " ";
            }
            else {
                ss << " " << value << " ";
            }
        }
        ss << "|\n";
    }

    ss << "|-----------------------------|\n";

    return ss.str();
}

void Camera::UpdateViewMatrix()
{
    m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}

void Camera::HandleKeyInput(const float& distance)
{
    if (ImGui::IsKeyPressed((ImGuiKey)KEY_W))
        m_MoveForward = true;
    if (ImGui::IsKeyPressed((ImGuiKey)KEY_S))
        m_MoveBackward = true;
    if (ImGui::IsKeyPressed((ImGuiKey)KEY_D))
        m_MoveRight = true;
    if (ImGui::IsKeyPressed((ImGuiKey)KEY_A))
        m_MoveLeft = true;

    if (ImGui::IsKeyPressed((ImGuiKey)KEY_ESC))
        glfwSetWindowShouldClose(glfwGetCurrentContext(), GLFW_TRUE);

    if (ImGui::IsKeyReleased((ImGuiKey)KEY_W))
        m_MoveForward = false;
    if (ImGui::IsKeyReleased((ImGuiKey)KEY_S))
        m_MoveBackward = false;
    if (ImGui::IsKeyReleased((ImGuiKey)KEY_D))
        m_MoveRight = false;
    if (ImGui::IsKeyReleased((ImGuiKey)KEY_A))
        m_MoveLeft = false;

    if (m_MoveForward)
        Advance(distance);
    if (m_MoveBackward)
        Advance(-distance);
    if (m_MoveRight)
        Strafe(distance);
    if (m_MoveLeft)
        Strafe(-distance);
}

void Camera::HandleMouseInput(const float& sensitivity)
{
    double x, y;

    if (ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows))
    {
        if (ImGui::IsMouseDown(ImGuiMouseButton_Right))
        {
            ImGui::SetMouseCursor(ImGuiMouseCursor_None);

            if (!m_IsMouseDown)
            {
                glfwGetCursorPos(glfwGetCurrentContext(), &x, &y);

                m_OriginalMousePos = glm::vec2(x, y);
                m_CurrentMousePos = glm::vec2(x, y);
                m_IsMouseDown = true;
            }

            glfwGetCursorPos(glfwGetCurrentContext(), &x, &y);
            float xOffset = x - m_CurrentMousePos.x;
            float yOffset = m_CurrentMousePos.y - y;

            xOffset *= sensitivity;
            yOffset *= sensitivity;

            Yaw(-xOffset);
            Pitch(-yOffset);

            glfwGetCursorPos(glfwGetCurrentContext(), &x, &y);
            m_CurrentMousePos = glm::vec2(x, y);
        }
        else if (m_IsMouseDown)
        {
            glfwSetCursorPos(glfwGetCurrentContext(), m_OriginalMousePos.x, m_OriginalMousePos.y);
            m_IsMouseDown = false;
            m_IsFirstMouse = true;
            ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);
        }
    }
}