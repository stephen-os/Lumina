#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <glad/glad.h>

namespace GL
{
    class FrameBuffer 
    {
    public:
        FrameBuffer();
        ~FrameBuffer() { Destroy(); };

        void Destroy();

        void Bind() const;
        void Unbind() const;
        void AttachTexture(unsigned int textureID);

    private:
        unsigned int m_FrameBufferID;
    };
}

#endif