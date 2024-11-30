// World.h
#pragma once

#include "Chunk.h"
#include <vector>
#include "Lumina/OpenGL/ShaderProgram.h"
#include <glm/glm.hpp>

class World {
public:
    World(); // No parameters needed

    void Generate(); // Generates all chunks in the world
    void Draw(GL::ShaderProgram& shader);

private:
    std::vector<Chunk> m_Chunks; // A grid of chunks
    std::vector<glm::vec3> m_ChunkOffsets; // Offsets for each chunk in the world
    static constexpr int WORLD_SIZE_X = 5; // 16 chunks along the X-axis
    static constexpr int WORLD_SIZE_Y = 1; // 16 chunks along the Y-axis
    static constexpr int WORLD_SIZE_Z = 5; // 128 chunks along the Z-axis
    static constexpr int CHUNK_WIDTH = 16;  // Each chunk has 16 units of width
    static constexpr int CHUNK_HEIGHT = 128; // Each chunk has 128 units of height
    static constexpr int CHUNK_DEPTH = 16;  // Each chunk has 16 units of depth
};
