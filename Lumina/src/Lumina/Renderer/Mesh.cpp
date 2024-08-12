#include "Mesh.h"

Mesh::Mesh(float* vertices, float* normals, unsigned int* indices, size_t vertexCount, size_t indicesCount) :
	m_Vertices(vertices),
	m_Normals(normals),
	m_Indices(indices),
	m_VertexCount(vertexCount),
	m_IndexCount(indicesCount) {}

Mesh::~Mesh()
{
	delete[] m_Vertices;
	delete[] m_Normals; 
	delete[] m_Indices;

	delete m_Attributes;
	delete m_VertexArray;
}

void Mesh::Draw() const
{
	m_VertexArray->Bind();
	m_VertexArray->DrawIndexed(GL_TRIANGLES);
	m_VertexArray->Unbind();
}

void Mesh::AttachShader(GL::ShaderProgram& shader)
{
	m_Shader = &shader;

	// Question: Delete old pointers for 
	// Attributes and VertexArray? 
	delete m_Attributes;
	delete m_VertexArray;

	m_Attributes = new GL::VertexAttributes(); 
	m_Attributes->AddVertices("a_Position", m_VertexCount * 3, 3, m_Vertices);
	m_Attributes->AddVertices("a_Normal", m_VertexCount * 3, 3, m_Normals);
	m_Attributes->AddIndices(m_Indices, m_IndexCount);

	m_VertexArray = new GL::VertexArray(*m_Shader, *m_Attributes);
}