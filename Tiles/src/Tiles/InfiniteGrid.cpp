#include "InfiniteGrid.h"

InfiniteGrid::InfiniteGrid()
{
    InitializeGeometry();
}

InfiniteGrid::~InfiniteGrid()
{
    m_VertexArray.Destroy();
    m_Attributes.Destroy();
}

void InfiniteGrid::InitializeGeometry() {
    GridData data;

    m_Attributes.AddBuffer("a_Position", 0, data.positions.data(), (uint32_t)data.positions.size(), 3, false);    
    m_Attributes.AddIndices(data.indices.data(), (uint32_t)data.indices.size());
    m_VertexArray.ApplyAttributes(m_Attributes); 
}

void InfiniteGrid::Draw(GL::ShaderProgram& shader)
{
    m_VertexArray.Bind();
    m_VertexArray.DrawIndexed(GL_TRIANGLES);
    m_VertexArray.Unbind();
}