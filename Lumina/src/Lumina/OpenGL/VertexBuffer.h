#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include <string>
#include <glad/glad.h>

namespace GL
{
    class VertexBuffer
    {
    public:
        VertexBuffer();
        VertexBuffer(const std::string& name, int location, const float* data, int count, int stride, GLenum usage = GL_STATIC_DRAW);

        VertexBuffer(const VertexBuffer& other);
        VertexBuffer(VertexBuffer&& other) noexcept;
        VertexBuffer& operator=(const VertexBuffer& other);

        ~VertexBuffer() = default;
        
        void Destroy();
        void Bind() const;
        void Unbind() const;

        std::string GetBufferName() const { return m_Name; }
        unsigned int GetLocation() const { return m_Location; }
        unsigned int GetStride() const { return m_Stride; }
        unsigned int GetCount() const { return m_Count; }
        unsigned int GetID() const { return m_VertexBufferID; }

        void SetName(const std::string& name) { m_Name = name; }
        void SetData(int location, const float* data, int count, int stride, GLenum usage = GL_STATIC_DRAW);
    private:
        std::string m_Name;
        int m_Location; 
        int m_Count;
        int m_Stride;
        unsigned int m_VertexBufferID = 0;
    };
}

#endif