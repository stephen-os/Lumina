#include <glm/glm.hpp>

#include "../OpenGL/ShaderProgram.h"

class Light
{
public: 
	~Light() = default;
protected:
	glm::vec3 m_Color; 
	float m_Intensity; 
};

class DirectionalLight : public Light
{
public:
    DirectionalLight(const glm::vec3& direction, const glm::vec3& color, float intensity);

    void SetUniforms(GL::ShaderProgram& shader) const;
private:
    glm::vec3 m_Direction;
};

class PointLight : public Light
{
public:
    PointLight(const glm::vec3& position, const glm::vec3& color, float intensity, float range);

    void SetUniforms(GL::ShaderProgram& shader) const;
private:
    glm::vec3 m_Position;
    float m_Range;
};