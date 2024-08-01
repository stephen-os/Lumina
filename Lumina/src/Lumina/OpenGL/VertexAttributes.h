#ifndef VERTEX_ATTRIBUTES_H
#define VERTEX_ATTRIBUTES_H

#include <string>
#include <vector>
#include <stdexcept>

#include "VertexAttribute.h"

#include <glad/glad.h>

namespace GL
{
    class VertexAttributes
    {
    public:
        VertexAttributes() : m_Vertices(-1), m_IndexBuffer(0), m_IndexCount(0) {}
        ~VertexAttributes() { Destroy(); };
        VertexAttribute& AddAttribute(const std::string& name, int vertices, int components, const std::vector<float>& floats, GLenum usage = GL_STATIC_DRAW);
        void AddIndices(const std::vector<unsigned int>& ints, GLenum usage = GL_STATIC_DRAW);
        void Destroy();

        std::vector<VertexAttribute>::iterator begin() { return m_Attributes.begin(); }
        std::vector<VertexAttribute>::iterator end() { return m_Attributes.end(); }

        int VertexCount() const { return m_Vertices; }
        size_t IndexCount() const { return m_IndexCount; }

        GLuint GetIndexBuffer() { return m_IndexBuffer; };
    private:
        int m_Vertices;
        GLuint m_IndexBuffer;
        std::vector<VertexAttribute> m_Attributes;
        size_t m_IndexCount;
    };
}

#endif