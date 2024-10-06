#include "Mesh.h"

Mesh::Mesh(float* vertices, float* normals, unsigned int* indices, size_t vertexCount, size_t indicesCount) :
	m_VertexCount(vertexCount),
	m_IndexCount(indicesCount)
{
	m_Attributes.AddVertices("a_Position", m_VertexCount * 3, 3, vertices);
	m_Attributes.AddVertices("a_Normal", m_VertexCount * 3, 3, normals);
	m_Attributes.AddIndices(indices, m_IndexCount);
}

void Mesh::Draw()
{
	m_VertexArray->Bind();
	m_VertexArray->DrawIndexed(GL_TRIANGLES);
	m_VertexArray->Unbind();
}

void Mesh::AttachShader(GL::ShaderProgram& shader)
{
	m_VertexArray.emplace(shader, m_Attributes);
}