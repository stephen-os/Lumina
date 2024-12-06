#include "TileObject.h"

TileObject::TileObject()
{
    InitializeGeometry();
}

void TileObject::InitializeGeometry() {
    TileData tileData;

    // Per vertex
    m_Attributes.AddBuffer("a_Position", 0, tileData.positions.data(), sizeof(float), tileData.positions.size(), 3, false);
    m_Attributes.AddBuffer("a_Normal", 1, tileData.normals.data(), sizeof(float), tileData.normals.size(), 3, false);
    m_Attributes.AddBuffer("a_TexCoord", 2, tileData.texCoords.data(), sizeof(float), tileData.texCoords.size(), 2, false);

    m_Attributes.AddIndices(tileData.indices.data(), tileData.indices.size());

    m_VertexArray.ApplyAttributes(m_Attributes);

    m_TextureAtlas.SetData("res/texture/atlas.jpg", 4, 4);
}

void TileObject::Draw(GL::ShaderProgram& shader, int textureIndex) 
{
    m_VertexArray.Bind();

    m_TextureAtlas.Bind();

    m_VertexArray.DrawIndexed(GL_TRIANGLES);

    m_TextureAtlas.Unbind();

    m_VertexArray.Unbind();
}