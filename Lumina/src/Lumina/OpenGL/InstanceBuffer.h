#ifndef INSTANCE_BUFFER_H
#define INSTANCE_BUFFER_H

#include <glad/glad.h>
#include <vector>

#include <glm/glm.hpp>

namespace GL
{
    class InstanceBuffer
    {
    public:
        InstanceBuffer();
        ~InstanceBuffer();

        // Binds the instance buffer
        void Bind() const;

        // Unbinds the instance buffer
        void Unbind() const;

        unsigned int GetID() { return m_BufferID; }

        // Updates the instance buffer data
        void UpdateBufferData(const std::vector<glm::mat4>& data, GLenum usage = GL_DYNAMIC_DRAW);

        // Configures an instance attribute for the buffer
        void SetInstanceAttribute(GLuint location, GLint size, GLenum type, GLsizei stride, GLuint divisor, GLsizei offset = 0);

        // Deletes the buffer
        void Destroy();

    private:
        GLuint m_BufferID;
        GLsizei m_InstanceCount;
    };
}

#endif // INSTANCE_BUFFER_H