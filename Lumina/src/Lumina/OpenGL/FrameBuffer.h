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

        void Destroy(); 
        void Bind() const;
        void Unbind() const;

        void AttachTexture(unsigned int id);
        void AttachDepthBuffer(unsigned int id);
    private:
        unsigned int m_FrameBufferID;
    };
}

#endif
