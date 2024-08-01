#ifndef VERTEX_ATTRIBUTE_H
#define VERTEX_ATTRIBUTE_H

#include <string>
#include <vector>

#include <glad/glad.h>

namespace GL
{
    class VertexAttribute
    {
    public:
        VertexAttribute(const std::string& name, int vertices, int components, const std::vector<float>& floats, GLenum usage = GL_STATIC_DRAW);
        ~VertexAttribute() { Destroy(); }
        void Update(const std::vector<float>& floats);
        void Destroy();

        std::string GetName() { return m_Name; };
        int GetComponents() { return m_Components; };
        GLuint GetBuffer() { return m_Buffer; };
    private:
        std::string m_Name;
        int m_Vertices;
        int m_Components;
        GLuint m_Buffer = 0;
    };
}

#endif 