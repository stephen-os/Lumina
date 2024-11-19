#include "Cube.h"

Cube::Cube() : m_Attributes(3)
{
    InitializeGeometry();
}

void Cube::InitializeGeometry()
{
    CubeData cubeData;

    m_Attributes.AddVertices("a_Position", 0, cubeData.positions.data(), cubeData.positions.size(), 3);
    m_Attributes.AddVertices("a_Normal", 1, cubeData.normals.data(), cubeData.normals.size(), 3);
    m_Attributes.AddVertices("a_TexCoord", 2, cubeData.texCoords.data(), cubeData.texCoords.size(), 2);
    m_Attributes.AddIndices(cubeData.indices.data(), cubeData.indices.size());

    m_VertexArray.ApplyAttributes(m_Attributes);

    m_Texture.SetSlot(1);
    m_Texture.SetData("res/texture/dirt.jpg");
}

void Cube::Draw(GL::ShaderProgram& shader)
{
    shader.Bind();
    shader.SetUniformMatrix4fv("u_Transform", glm::mat4(1));
    shader.SetUniformMatrix4fv("u_Model", m_Transform.GetMatrix());

    m_Texture.Bind(); 

    m_VertexArray.Bind();
    m_VertexArray.DrawIndexed(GL_TRIANGLES);
    m_VertexArray.Unbind();

    m_Texture.Unbind(); 

    shader.Unbind();
}
