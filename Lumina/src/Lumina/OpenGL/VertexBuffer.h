#ifndef VERTEX_ATTRIBUTE_H
#define VERTEX_ATTRIBUTE_H

#include <string>
#include <vector>

#include <glad/glad.h>

namespace GL
{
    class VertexBuffer
    {
    public:
        VertexBuffer(const std::string& name, int count, int stride, const float* floats, GLenum usage = GL_STATIC_DRAW);
        ~VertexBuffer() = default;

        void Destroy();

        void Bind(); 
        void Unbind(); 

        std::string GetBufferName() const { return m_Name; };
        unsigned int GetStride() const { return m_Stride; };
        unsigned int GetBufferID() const { return m_VertexBufferID; };
        unsigned int GetCount() const { return m_Count; };
    private:
        std::string m_Name;
        unsigned int m_Count;
        unsigned int m_Stride;
        unsigned int m_VertexBufferID = 0;
    };
}

#endif 