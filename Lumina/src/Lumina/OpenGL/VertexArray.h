#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include "ShaderProgram.h"
#include "VertexAttributes.h"
#include <glad/glad.h>
#include <iostream>
#include <algorithm>

namespace GL
{
    class VertexArray
    {
    public:
        VertexArray();
        VertexArray(const VertexArray& other);
        VertexArray(VertexArray&& other) noexcept;
        VertexArray& operator=(const VertexArray& other);
        ~VertexArray();

        void Destroy();
        void Bind();
        void Unbind();

        void ApplyAttributes(VertexAttributes& attributes);

        void DrawIndexed(GLenum mode);
        void DrawInstanced(GLenum mode, int instanceCount);

    private:
        bool m_IsBound;
        int m_IndexCount; // Updated to remove vertex count
        unsigned int m_VertexArrayID;
    };
}

#endif