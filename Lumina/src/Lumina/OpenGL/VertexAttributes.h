#ifndef VERTEX_ATTRIBUTES_H
#define VERTEX_ATTRIBUTES_H

#include <string>
#include <stdexcept>
#include <vector>

#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include <glad/glad.h>

#include <iostream>
#include <memory>

#include <optional>

namespace GL
{
    class VertexAttributes
    {
    public:
        VertexAttributes(const unsigned int& maxAttributeCount) : m_VertexCount(-1), m_MaxAttributeCount(maxAttributeCount), m_AttributeCount(0)
        { 
            m_Attributes.reserve(m_MaxAttributeCount); 
        };
        ~VertexAttributes() {};
        void AddVertices(const std::string& name, int count, int components, const float* floats, GLenum usage = GL_STATIC_DRAW);
        void AddIndices(const unsigned int* indices, unsigned int count, GLenum usage = GL_STATIC_DRAW);

        std::vector<VertexBuffer>::iterator begin() { return m_Attributes.begin(); }
        std::vector<VertexBuffer>::iterator end() { return m_Attributes.end(); }

        int GetVertexCount() const { return m_VertexCount; };
        unsigned int GetIndexCount() const { return m_IndexBuffer->GetIndexCount(); }
        GLuint GetIndexBufferID() const { return m_IndexBuffer->GetIndexBufferID(); };
    private:
        int m_VertexCount;
        unsigned int m_MaxAttributeCount; 
        unsigned int m_AttributeCount;
        
        std::optional<IndexBuffer> m_IndexBuffer;
        std::vector<VertexBuffer> m_Attributes;
    };
}

#endif