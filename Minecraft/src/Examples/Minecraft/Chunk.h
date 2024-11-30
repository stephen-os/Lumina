#pragma once

#include "Lumina/Geometry/Cube.h"
#include <glm/glm.hpp>
#include <vector>

class Chunk
{
public:
    Chunk(int width = 16, int height = 128, int depth = 16);

    void GenerateChunk();
    void Draw(GL::ShaderProgram& shader);

private:
    int m_Width;
    int m_Height;
    int m_Depth;

    Cube m_Cube;
    std::vector<glm::mat4> m_InstanceTransforms;
};
