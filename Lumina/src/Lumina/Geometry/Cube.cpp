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
    // ToDo: Add an instance component to the vertex array object.
    // m_Attributes.AddInstances("name", location = 3)
    m_Attributes.AddInstances("a_Transform", 3, 4, GL_FLOAT, sizeof(glm::mat4), 1, 0);
    m_Attributes.AddIndices(cubeData.indices.data(), cubeData.indices.size());

    m_VertexArray.ApplyAttributesInstanced(m_Attributes, m_InstanceBuffer.GetID());

    m_Texture.SetSlot(1);
    m_Texture.SetData("res/texture/dirt.jpg");
}

void Cube::Draw(GL::ShaderProgram& shader, const std::vector<glm::mat4>& instanceTransforms)
{
    // Update instance buffer with current transforms
    m_InstanceBuffer.UpdateBufferData(instanceTransforms);

    // Configure instance attribute
    m_VertexArray.Bind();
    m_InstanceBuffer.SetInstanceAttribute(
        /* location */ 3,
        /* size */ 4,
        /* type */ GL_FLOAT,
        /* stride */ sizeof(glm::mat4),
        /* divisor */ 1,
        /* offset */ 0
    );

    // Bind shader and set uniforms
    shader.Bind();
    // shader.SetUniformMatrix4fv("u_Model", m_Transform.GetMatrix());

    m_Texture.Bind();

    // Draw cubes using instanced rendering
    m_VertexArray.Bind();
    m_VertexArray.DrawInstanced(GL_TRIANGLES, instanceTransforms.size());
    m_VertexArray.Unbind();

    m_Texture.Unbind();
    shader.Unbind();
}