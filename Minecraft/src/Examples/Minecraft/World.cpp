// World.cpp
#include "World.h"

World::World() {
    // Reserve space for chunks and offsets
    m_Chunks.reserve(WORLD_SIZE_X * WORLD_SIZE_Z);  // Assuming a 16x128 world grid
    m_ChunkOffsets.reserve(WORLD_SIZE_X * WORLD_SIZE_Z);

    // Initialize chunks and their world offsets
    for (int z = 0; z < WORLD_SIZE_Z; ++z) {
        for (int x = 0; x < WORLD_SIZE_X; ++x) {
            m_Chunks.emplace_back(CHUNK_WIDTH, CHUNK_HEIGHT, CHUNK_DEPTH);
            m_ChunkOffsets.emplace_back(x * CHUNK_WIDTH, 0.0, z * CHUNK_DEPTH);
        }
    }
}

void World::Generate() {
    // Generate chunks for the entire world
    for (auto& chunk : m_Chunks) {
        chunk.GenerateChunk();
    }
}

void World::Draw(GL::ShaderProgram& shader) {
    for (size_t i = 0; i < m_Chunks.size(); ++i) {
        shader.SetUniform3fv("u_ChunkOffset", m_ChunkOffsets[i]);
        m_Chunks[i].Draw(shader);
    }
}
