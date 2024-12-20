#include "Mesh.h"

#include "tiny_gltf.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

Mesh::Mesh(MeshData& data)
{ 
    m_VertexAttributes.AddBuffer("a_Position", 0, data.vertices, sizeof(float), uint32_t(data.vertexCount * 3), 3, false);
    m_VertexAttributes.AddBuffer("a_Normal", 1, data.normals, sizeof(float), uint32_t(data.vertexCount * 3), 3, false);
    m_VertexAttributes.AddIndices(data.indices, data.indicesCount);

    m_Transform = data.transform; 

    m_VertexArrays.ApplyAttributes(m_VertexAttributes);
}

Mesh::Mesh(const Mesh& other)
    : m_Transform(other.m_Transform), m_VertexAttributes(other.m_VertexAttributes),
    m_VertexArrays(other.m_VertexArrays) {}

Mesh::Mesh(Mesh&& other) noexcept
    : m_Transform(std::move(other.m_Transform)), m_VertexAttributes(std::move(other.m_VertexAttributes)),
    m_VertexArrays(std::move(other.m_VertexArrays)) {}

Mesh& Mesh::operator=(const Mesh& other)
{
    if (this != &other) 
    {
        m_Transform = other.m_Transform;
        m_VertexAttributes = other.m_VertexAttributes;
        m_VertexArrays = other.m_VertexArrays;
    }
    return *this;
}

void Mesh::Destroy()
{
    m_VertexAttributes.Destroy(); 
    m_VertexArrays.Destroy(); 
}

void Mesh::Draw(GL::ShaderProgram& shader)
{
    shader.SetUniformMatrix4fv("u_Model", m_Transform.GetMatrix());

    m_VertexArrays.Bind();
    m_VertexArrays.DrawIndexed(GL_TRIANGLES);
    m_VertexArrays.Unbind();
}