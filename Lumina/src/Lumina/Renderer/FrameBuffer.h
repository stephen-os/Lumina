#pragma once

#include <glad/glad.h>

#include <cstdint>

namespace Lumina
{
    class FrameBuffer
    {
    public:
        FrameBuffer();
        ~FrameBuffer();

        void Bind() const;
        void Unbind() const;

        void Resize(uint32_t width, uint32_t height);
        uint32_t GetID() { return m_BufferID; }
        uint32_t GetColorAttachment() { return m_ColorAttachment; }

        uint32_t GetWidth() { return m_Width; }
        uint32_t GetHeight() { return m_Height; }
    private:
        uint32_t m_BufferID = 0;
        uint32_t m_ColorAttachment = 0;
        uint32_t m_DepthAttachment = 0;

        uint32_t m_Width;
        uint32_t m_Height;
    };
}