#ifndef VERTEX_ATTRIBUTES_H
#define VERTEX_ATTRIBUTES_H

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "InstanceBuffer.h"

#include <glad/glad.h>

#include <string>
#include <vector>
#include <optional>
#include <iostream>

namespace GL
{
    class VertexAttributes
    {
    public:
        VertexAttributes(const unsigned int& maxAttributeCount) : m_VertexCount(-1), m_MaxAttributeCount(maxAttributeCount)
        { 
            m_Attributes.reserve(m_MaxAttributeCount); 
        };

        VertexAttributes(const VertexAttributes& other);
        VertexAttributes(VertexAttributes&& other) noexcept; 
        VertexAttributes& operator=(const VertexAttributes& other);

        ~VertexAttributes() = default;

        void Destroy(); 

        // Possibly implement. 
        // void Reserve(int size);

        void AddVertices(const std::string& name, int location, const float* data, int count, int stride, GLenum usage = GL_STATIC_DRAW);
        void AddInstances(const std::string& name, int location, int size, unsigned int type, int stride, unsigned int divisor, int offset = 0);
        void AddIndices(const unsigned int* indices, int count, GLenum usage = GL_STATIC_DRAW);

        std::vector<VertexBuffer>::iterator begin() { return m_Attributes.begin(); }
        std::vector<VertexBuffer>::iterator end() { return m_Attributes.end(); }

        int GetVertexCount() const { return m_VertexCount; };
        int GetIndexCount() const { return m_IndexBuffer.GetIndexCount(); }
        unsigned int GetIndexBufferID() const { return m_IndexBuffer.GetID(); };
    private:
        int m_VertexCount;
        unsigned int m_MaxAttributeCount; 
        
        IndexBuffer m_IndexBuffer;
        InstanceBuffer m_InstanceBuffer; 
        std::vector<VertexBuffer> m_Attributes;
    };
}

#endif