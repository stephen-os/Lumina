#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <glad/glad.h>

namespace GL
{
    class FrameBuffer
    {
    public:
        FrameBuffer();
        ~FrameBuffer();

        void Bind() const;
        void Unbind() const;
        void AttachTexture(unsigned int textureID);
        void AttachDepthBuffer(float width, float height);

    private:
        unsigned int m_FrameBufferID;
        unsigned int m_DepthBufferID;
    };
}

#endif
