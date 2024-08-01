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
        VertexArray(ShaderProgram& program, VertexAttributes& attributes);
        ~VertexArray() { Destroy(); };

        void Bind();
        void Unbind();
        void Destroy();

        void DrawSequence(GLenum mode);
        void DrawIndexed(GLenum mode);

    private:
        VertexAttributes& m_Attributes;
        GLuint m_VertexArray;
        bool m_IsBound;
    };
}

#endif