#ifndef DEPTH_BUFFER_H
#define DEPTH_BUFFER_H

#include <glad/glad.h>

namespace GL
{
    class DepthBuffer
    {
    public:
        DepthBuffer();
        ~DepthBuffer();

        void Destroy();
        void Bind() const;
        void Unbind() const;

        bool SetData(int width, int height);

        unsigned int GetID() const { return m_DepthBufferID; }
    private:
        int m_Width;
        int m_Height;
        unsigned int m_DepthBufferID;
    };
}

#endif