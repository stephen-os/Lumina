#ifndef BUFFER_H
#define BUFFER_H

#include <string>
#include <glad/glad.h>

namespace GL
{
    enum BufferType : int
    {
        ARRAY = GL_ARRAY_BUFFER,
        ELEMENT = GL_ELEMENT_ARRAY_BUFFER
    };

    enum DrawMode : int
    {
        STATIC = GL_STATIC_DRAW,
        DYNAMIC = GL_DYNAMIC_DRAW,
        STREAM = GL_STREAM_DRAW
    };

    struct BufferInfo
    {
        void* data = nullptr;           // Default: null (expect user define)

        GLuint type = ARRAY;            // Default: ARRAY
        GLenum usage = STATIC;          // Default: STATIC
        GLsizei size = 4;               // Default: 4 bytes
        uint32_t count = 0;             // Default: 0 elements
        uint32_t stride = 3;            // Default: 3 (vec3)
        bool isInstance = false;        // Default: false (single draw per data) 

        // TODO:
        // Add name or remove name?
        // Buffer should handle location? 
    };

    class Buffer
    {
    public:
        // Constructor: Initializes the buffer object and sets default values for member variables.
        Buffer();

        // Destructor: Ensures proper cleanup by deleting the OpenGL buffer if it has been created.
        ~Buffer();

        // Copy constructor: Creates a new buffer object as a copy of another, potentially duplicating OpenGL state.
        Buffer(const Buffer& other);

        // Move constructor: Transfers ownership of an existing buffer's resources to a new instance.
        Buffer(Buffer&& other) noexcept;

        // Copy assignment operator: Copies the buffer's data and OpenGL state from another instance.
        Buffer& operator=(const Buffer& other);

        // Initializes the OpenGL buffer object, generating a new buffer ID.
        void Init();

        // Deletes the OpenGL buffer, freeing its associated GPU memory.
        void Delete();

        // Binds the buffer object to the appropriate OpenGL target (e.g., GL_ARRAY_BUFFER).
        void Bind() const;

        // Unbinds the currently bound buffer from the target.
        void Unbind() const;
        
        // Sets the name of the buffer, typically used for identification or debugging purposes.
        void SetName (const std::string& name) { m_Name = name; }

        // Retrieves the buffer's name, typically used for identification or debugging purposes.
        std::string GetName() const { return m_Name; }

        // Retrieves the stride of the data in the buffer (e.g., the size of each vertex attribute).
        uint32_t GetStride() const { return m_Stride; }

        // Retrieves the number of elements stored in the buffer.
        uint32_t GetCount() const { return m_Count; }

        // Retrieves the size of the buffer in bytes.
        GLsizei GetSize() const { return m_Size; }

        // Retrieves the OpenGL ID of the buffer, which uniquely identifies it within the GPU.
        unsigned int GetID() const { return m_ID; }

        bool IsInstance() const { return m_IsInstance; }

        // Uploads data to the GPU buffer. 
        // - `type`: Specifies the buffer type (e.g., GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER).
        // - `data`: Pointer to the buffer data to upload.
        // - `count`: Number of elements in the data array.
        // - `stride`: Size (in bytes) of a single data element.
        // - `usage`: OpenGL usage hint (e.g., GL_STATIC_DRAW, GL_DYNAMIC_DRAW).
        void SetData(GLuint type, const void* data, uint32_t count, uint32_t stride, bool isInstance, GLenum usage = GL_STATIC_DRAW);

        // void SetData(); 

        // Updates part of the buffer data.
        // - `offset`: Byte offset in the buffer where the update should begin.
        // - `data`: Pointer to the new data to upload.
        // - `size`: Size (in bytes) of the data to upload.
        void UpdateBuffer(const void* data, int count, GLenum usage = GL_STATIC_DRAW);
    private:
        std::string m_Name;    // Optional identifier for the buffer.
        GLuint m_Type;         // Type of buffer (e.g., GL_ARRAY_BUFFER).
        uint32_t m_Count;      // Number of elements stored in the buffer.
        uint32_t m_Stride;     // Size of a single element in bytes.
        GLsizei m_Size;        // Size of the buffer in bytes.
        bool m_IsInstance;     // Indicates if the buffer is used for instancing.
        GLuint m_ID;           // OpenGL identifier for the buffer.
    };
}

#endif