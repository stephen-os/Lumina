#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <sstream>
#include <iomanip>
#include <string>

class Transform {
public:
    Transform() : m_position(0.0f), m_rotation(glm::quat()), m_scale(1.0f) {}
    ~Transform() {} 

    void SetPosition(const glm::vec3& position) { m_position = position; }
    void SetRotation(const glm::quat& rotation) { m_rotation = rotation; }
    void SetScale(const glm::vec3& scale) { m_scale = scale; }

    glm::vec3 GetPosition() const { return m_position; }
    glm::quat GetRotation() const { return m_rotation; }
    glm::vec3 GetScale() const { return m_scale; }

    void Translate(const glm::vec3& translation) { m_position += translation; }
    void Rotate(float angle, const glm::vec3& axis) { m_rotation = glm::rotate(m_rotation, angle, axis); }
    void scale(const glm::vec3& scale) { m_scale *= scale; }

    glm::mat4 GetTransformMatrix() const 
    {
        glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), m_scale);
        glm::mat4 rotationMatrix = glm::mat4_cast(m_rotation);
        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), m_position);

        return translationMatrix * rotationMatrix * scaleMatrix;
    }

    std::string ToString() 
    {
        glm::mat4 matrix = GetTransformMatrix();
        std::stringstream ss;
        ss << std::fixed << std::setprecision(3);

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

private:
    glm::vec3 m_position;
    glm::quat m_rotation;
    glm::vec3 m_scale;
};