#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include "ShaderProgram.h"
#include "VertexAttributes.h"

#include <glad/glad.h>

namespace GL
{
    class VertexArray
    {
    public:
        VertexArray();
        VertexArray(const VertexArray& other);
        VertexArray(VertexArray&& other) noexcept;
        VertexArray& operator=(const VertexArray& other);

        ~VertexArray() = default;

        void Destroy(); 
        void Bind();
        void Unbind();

        void ApplyAttributes(VertexAttributes& attributes);

        void DrawSequence(GLenum mode);
        void DrawIndexed(GLenum mode);
    private:
        bool m_IsBound;
        int m_VertexCount;
        int m_IndexCount;
        unsigned int m_VertexArrayID;
    };
}

#endif