#include "Light.h"

DirectionalLight::DirectionalLight(const glm::vec3& direction, const glm::vec3& color, float intensity)
    : m_Direction(direction)
{
    m_Color = color;
    m_Intensity = intensity;
}

void DirectionalLight::SetUniforms(GL::ShaderProgram& shader) const
{
    shader.SetUniform3fv("u_Color", m_Color);
    shader.SetUniform1f("u_Intensity", m_Intensity);
    shader.SetUniform3fv("u_Direction", m_Direction); 
}

PointLight::PointLight(const glm::vec3& position, const glm::vec3& color, float intensity, float range)
    : m_Position(position), m_Range(range)
{
    m_Color = color;
    m_Intensity = intensity;
}

void PointLight::SetUniforms(GL::ShaderProgram & shader) const
{
    shader.SetUniform3fv("u_Color", m_Color);
    shader.SetUniform1f("u_Intensity", m_Intensity);
    shader.SetUniform3fv("u_Position", m_Position);
    shader.SetUniform1f("u_Range", m_Range); 
}