#ifndef VERTEX_ATTRIBUTES_H
#define VERTEX_ATTRIBUTES_H

#include "Buffer.h"
#include "InstanceBuffer.h"

#include <glad/glad.h>
#include <string>
#include <vector>
#include <optional>
#include <iostream>

namespace GL
{
    class VertexAttributes
    {
    public:
        // Default constructor
        VertexAttributes() = default;

        // Copy constructor: Creates a deep copy of another VertexAttributes object
        VertexAttributes(const VertexAttributes& other);

        // Move constructor: Transfers ownership of resources from another VertexAttributes object
        VertexAttributes(VertexAttributes&& other) noexcept;

        // Copy assignment operator: Handles deep copying from another VertexAttributes object
        VertexAttributes& operator=(const VertexAttributes& other);

        // Destructor: Ensures cleanup of allocated resources
        ~VertexAttributes() { Destroy(); }

        // Destroys all buffers associated with the vertex attributes
        void Destroy();

        // Adds a new vertex buffer with given parameters and uploads its data to the GPU
        void AddVertices(const std::string& name, int location, const float* data, int count, int stride, GLenum usage = GL_STATIC_DRAW);

        // Configures an instance buffer with specified attributes
        void AddInstances(const std::string& name, int location, int size, unsigned int type, int stride, unsigned int divisor, int offset = 0);

        // Adds an index buffer to the vertex attributes and uploads its data to the GPU
        void AddIndices(const unsigned int* indices, int count, GLenum usage = GL_STATIC_DRAW);

        // Provides iterators for accessing all vertex attribute buffers
        std::vector<std::pair<GLuint, Buffer>>::iterator begin() { return m_Attributes.begin(); }
        std::vector<std::pair<GLuint, Buffer>>::iterator end() { return m_Attributes.end(); }

        // Updates instance buffer data with a vector of transformation matrices
        void UpdateInstanceBufferData(const std::vector<glm::mat4>& data);

        // Retrieves the number of indices in the index buffer
        int GetIndexCount() const { return m_IndexBuffer.GetCount(); }

        // Retrieves the OpenGL ID of the index buffer
        unsigned int GetIndexBufferID() const { return m_IndexBuffer.GetID(); }

        // Retrieves the OpenGL ID of the instance buffer
        unsigned int GetInstanceBufferID() const { return m_InstanceBuffer.GetID(); }

        // Provides access to the instance buffer for further operations
        InstanceBuffer& GetInstanceBuffer() { return m_InstanceBuffer; }

    private:
        Buffer m_IndexBuffer; // Buffer for storing index data
        InstanceBuffer m_InstanceBuffer; // Instance buffer for per-instance attributes
        std::vector<std::pair<GLuint, Buffer>> m_Attributes; // List of vertex attribute buffers with location IDs
    };
}

#endif