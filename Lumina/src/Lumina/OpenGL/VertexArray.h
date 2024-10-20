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
        VertexArray(VertexAttributes attributes, ShaderProgram& shader);
        ~VertexArray() = default;

        void Destroy(); 

        void Bind();
        void Unbind();

        void DrawSequence(GLenum mode);
        void DrawIndexed(GLenum mode);
    private:
        GL::VertexAttributes m_Attributes; 

        bool m_IsBound;

        unsigned int m_VertexArrayID = 0;
    };
}

#endif