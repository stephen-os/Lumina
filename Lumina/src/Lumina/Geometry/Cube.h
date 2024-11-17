#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "../OpenGL/VertexBuffer.h"

class Cube
{
public:
    Cube();

    const std::vector<float>& GetVertices() const { return m_Vertices; }
    const std::vector<unsigned int>& GetIndices() const { return m_Indices; }

    void SetTransform(const glm::mat4& transform) { m_Transform = transform; }
    const glm::mat4& GetTransform() const { return m_Transform; }

private:
    std::vector<float> m_Vertices;
    std::vector<unsigned int> m_Indices;
    glm::mat4 m_Transform;

    void InitializeGeometry();
}