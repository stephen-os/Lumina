#include "Camera.h"

#include <sstream>
#include <iomanip>

std::string MatrixToString(const std::string& name, const glm::mat4& matrix) {
    std::ostringstream oss;
    const float* ptr = glm::value_ptr(matrix);

    // Set the precision for floating-point numbers
    oss << std::fixed << std::setprecision(4);

    oss << name << ":\n";

    // Format the matrix as a 4x4 grid
    for (int i = 0; i < 16; ++i) {
        if (i % 4 == 0 && i != 0) {
            oss << "\n"; // Newline for each row
        }
        oss << ptr[i] << " ";
    }

    oss << "\n";

    return oss.str();
}

Camera::Camera(float fov, float aspect, float near, float far)
    : m_ProjectionMatrix(glm::perspective(glm::radians(fov), aspect, near, far)),
    m_ViewMatrix(1.0f),
    m_Position(0.0f, 0.0f, 3.0f),
    m_Front(0.0f, 0.0f, -1.0f),
    m_Up(0.0f, 1.0f, 0.0f)
{
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
    UpdateViewMatrix();
}

void Camera::Pitch(float degrees)
{
    float radians = glm::radians(degrees);
    glm::vec3 right = glm::normalize(glm::cross(m_Front, m_Up));
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), radians, right);
    m_Front = glm::normalize(glm::vec3(rotation * glm::vec4(m_Front, 0.0f)));
    UpdateViewMatrix();
}

void Camera::Yaw(float degrees)
{
    float radians = glm::radians(degrees);
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), radians, m_Up);
    m_Front = glm::normalize(glm::vec3(rotation * glm::vec4(m_Front, 0.0f)));
    UpdateViewMatrix();
}

const glm::mat4& Camera::GetProjectionMatrix() const { return m_ProjectionMatrix; }
const glm::mat4& Camera::GetViewMatrix() const { return m_ViewMatrix; }

std::string Camera::GetProjectionMatrixString() const {
    return MatrixToString("Projection Matrix", m_ProjectionMatrix);
}

std::string Camera::GetViewMatrixString() const {
    return MatrixToString("View Matrix", m_ViewMatrix);
}

void Camera::UpdateViewMatrix()
{
    m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}
