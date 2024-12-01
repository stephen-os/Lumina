#include "Chunk.h"
#include <glm/gtc/matrix_transform.hpp>

Chunk::Chunk(int width, int height, int depth) : m_Width(width), m_Height(height), m_Depth(depth)
{
}

void Chunk::GenerateChunk()
{
    m_InstanceTransforms.clear();
    for (int x = 0; x < m_Width; ++x)
    {
        for (int z = 0; z < m_Depth; ++z)
        {
            for (int y = 0; y < m_Height; ++y)
            {
                glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
                m_InstanceTransforms.push_back(transform);
            }
        }
    }
}

void Chunk::Draw(GL::ShaderProgram& shader)
{
    m_Cube.Draw(shader, m_InstanceTransforms);
}