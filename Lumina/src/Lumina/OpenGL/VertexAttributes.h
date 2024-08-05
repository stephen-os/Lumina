#ifndef VERTEX_ATTRIBUTES_H
#define VERTEX_ATTRIBUTES_H

#include <string>
#include <vector>
#include <stdexcept>

#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include <glad/glad.h>

#include <iostream>
#include <memory>

namespace GL
{
    class VertexAttributes
    {
    public:
        VertexAttributes() : m_VertexCount(-1) {};
        ~VertexAttributes() {};
        void AddVertices(const std::string& name, int count, int components, const float* floats, GLenum usage = GL_STATIC_DRAW);
        void AddIndices(const unsigned int* indices, unsigned int count, GLenum usage = GL_STATIC_DRAW);

        std::vector<std::unique_ptr<VertexBuffer>>::iterator begin() { return m_Attributes.begin(); }
        std::vector<std::unique_ptr<VertexBuffer>>::iterator end() { return m_Attributes.end(); }

        void BindVertices()
        {
            for (auto& attribute : m_Attributes)
            {
                attribute->Bind();
                std::cout << attribute->GetBufferID() << std::endl;
                glEnableVertexAttribArray(attribute->GetBufferID() - 1);
                glVertexAttribPointer(attribute->GetBufferID() - 1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
            }
        }

        void BindIndices()
        {
            m_IndexBuffer->Bind();
        }

        int GetVertexCount() const { return m_VertexCount; };
        unsigned int GetIndexCount() const { return m_IndexBuffer->GetIndexCount(); }
        GLuint GetIndexBufferID() const { return m_IndexBuffer->GetIndexBufferID(); };
    private:
        int m_VertexCount; 
        IndexBuffer* m_IndexBuffer = nullptr;
        std::vector<std::unique_ptr<VertexBuffer>> m_Attributes;
    };
}

#endif