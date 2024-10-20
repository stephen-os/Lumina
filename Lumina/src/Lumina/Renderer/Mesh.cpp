#include "Mesh.h"

#include "tiny_gltf.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

Mesh::Mesh(MeshData& data, GL::ShaderProgram& shader)
{ 
    GL::VertexAttributes attributes(2);
    attributes.AddVertices("a_Position", data.vertexCount * 3, 3, data.vertices);
    attributes.AddVertices("a_Normal", data.vertexCount * 3, 3, data.normals);
    attributes.AddIndices(data.indices, data.indicesCount);

    m_Transform = data.transform; 

    m_VertexArrays.emplace(attributes, shader); 
}

void Mesh::Destroy()
{
    if (m_VertexArrays)
    {
        m_VertexArrays->Destroy(); 
    }
}

void Mesh::Draw(GL::ShaderProgram& shader)
{
    shader.SetUniformMatrix4fv("u_Model", m_Transform.GetMatrix());

    m_VertexArrays->Bind();
    m_VertexArrays->DrawIndexed(GL_TRIANGLES);
    m_VertexArrays->Unbind();
}