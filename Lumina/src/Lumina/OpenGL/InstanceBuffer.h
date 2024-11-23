#ifndef INSTANCE_BUFFER_H
#define INSTANCE_BUFFER_H

#include <glad/glad.h>
#include <vector>
#include <string>

#include <glm/glm.hpp>

namespace GL
{
    class InstanceBuffer
    {
    public:
        InstanceBuffer();
        ~InstanceBuffer();

        void Destroy();

        void Bind() const;
        void Unbind() const;

        void UpdateBufferData(const std::vector<glm::mat4>& data, GLenum usage = GL_DYNAMIC_DRAW);

        unsigned int GetID() const { return m_InstanceBufferID; }
        int GetLocation() const { return m_Location; }

        void SetName(const std::string& name) { m_Name = name; };
        void SetInstanceAttribute(GLuint location, GLint size, GLenum type, GLsizei stride, GLuint divisor, GLsizei offset = 0);
    private:
        std::string m_Name; 
        int m_Location; 
        unsigned int  m_InstanceBufferID;
        GLsizei m_InstanceCount;
    };
}

#endif // INSTANCE_BUFFER_H