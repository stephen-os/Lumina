#include "TileObject.h"

TileObject::TileObject()
{
    InitializeGeometry();
}

TileObject::~TileObject()
{
    m_VertexArray.Destroy();
    m_Attributes.Destroy();
    m_Texture.Destroy();
}

void TileObject::InitializeGeometry() {
    TileData tileData;

    // Per vertex
    m_Attributes.AddBuffer("a_Position", 0, tileData.positions.data(), sizeof(float), tileData.positions.size(), 3, false);
    m_Attributes.AddBuffer("a_Normal", 1, tileData.normals.data(), sizeof(float), tileData.normals.size(), 3, false);
    m_Attributes.AddBuffer("a_TexCoord", 2, tileData.texCoords.data(), sizeof(float), tileData.texCoords.size(), 2, false);

    m_Attributes.AddIndices(tileData.indices.data(), tileData.indices.size());

    m_VertexArray.ApplyAttributes(m_Attributes);

    m_Texture.SetData("res/texture/world_tileset.png");
}

void TileObject::Draw(GL::ShaderProgram& shader) 
{
    m_VertexArray.Bind();

    m_Texture.Bind();

    m_VertexArray.DrawIndexed(GL_TRIANGLES);

    m_Texture.Unbind();

    m_VertexArray.Unbind();
}