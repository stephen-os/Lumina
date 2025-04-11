#pragma once

#include <vulkan/vulkan.h>
#include "../../Core/Aliases.h"
#include "../Buffer.h"

namespace Lumina
{
    class VulkanVertexBuffer : public VertexBuffer
    {
    public:
        VulkanVertexBuffer(uint32_t size);
        VulkanVertexBuffer(float* vertices, uint32_t size);
        ~VulkanVertexBuffer();

        void Bind() const override {};
        void Unbind() const override {};

        void SetData(const void* data, uint32_t size) override;

        const BufferLayout& GetLayout() const override { return m_Layout; }
        void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }

    private:
        VkBuffer m_Buffer = VK_NULL_HANDLE;
        VkDeviceMemory m_Memory = VK_NULL_HANDLE;
        BufferLayout m_Layout;
    };

    class VulkanIndexBuffer : public IndexBuffer
    {
    public:
        VulkanIndexBuffer(uint32_t* indices, uint32_t count);
        ~VulkanIndexBuffer();

        void Bind() const override {};
        void Unbind() const override {};

        uint32_t GetCount() const override { return m_Count; }

    private:
        VkBuffer m_Buffer = VK_NULL_HANDLE;
        VkDeviceMemory m_Memory = VK_NULL_HANDLE;
        uint32_t m_Count = 0;
    };
}
