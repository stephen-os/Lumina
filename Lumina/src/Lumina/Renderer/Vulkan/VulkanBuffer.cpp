#include "VulkanBuffer.h"

#include "VulkanContext.h"

#include <cstring>

namespace Lumina
{
    VulkanVertexBuffer::VulkanVertexBuffer(uint32_t size)
    {
        VkDevice device = VulkanContext::GetDevice();

        // Create buffer
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        vkCreateBuffer(device, &bufferInfo, nullptr, &m_Buffer);

        // Allocate memory
        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(device, m_Buffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;

        VkPhysicalDeviceMemoryProperties memProps;
        vkGetPhysicalDeviceMemoryProperties(VulkanContext::GetPhysicalDevice(), &memProps);

        for (uint32_t i = 0; i < memProps.memoryTypeCount; i++)
        {
            if ((memRequirements.memoryTypeBits & (1 << i)) &&
                (memProps.memoryTypes[i].propertyFlags & (VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)))
            {
                allocInfo.memoryTypeIndex = i;
                break;
            }
        }

        vkAllocateMemory(device, &allocInfo, nullptr, &m_Memory);
        vkBindBufferMemory(device, m_Buffer, m_Memory, 0);
    }

    VulkanVertexBuffer::VulkanVertexBuffer(float* vertices, uint32_t size) : VulkanVertexBuffer(size)
    {
        SetData(vertices, size);
    }

    VulkanVertexBuffer::~VulkanVertexBuffer()
    {
        VkDevice device = VulkanContext::GetDevice();
        vkDestroyBuffer(device, m_Buffer, nullptr);
        vkFreeMemory(device, m_Memory, nullptr);
    }

    void VulkanVertexBuffer::SetData(const void* data, uint32_t size)
    {
        void* dst;
        vkMapMemory(VulkanContext::GetDevice(), m_Memory, 0, size, 0, &dst);
        std::memcpy(dst, data, size);
        vkUnmapMemory(VulkanContext::GetDevice(), m_Memory);
    }

    VulkanIndexBuffer::VulkanIndexBuffer(uint32_t* indices, uint32_t count) : m_Count(count)
    {
        VkDevice device = VulkanContext::GetDevice();
        VkDeviceSize size = count * sizeof(uint32_t);

        // Create buffer
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        vkCreateBuffer(device, &bufferInfo, nullptr, &m_Buffer);

        // Allocate memory
        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(device, m_Buffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;

        VkPhysicalDeviceMemoryProperties memProps;
        vkGetPhysicalDeviceMemoryProperties(VulkanContext::GetPhysicalDevice(), &memProps);

        for (uint32_t i = 0; i < memProps.memoryTypeCount; i++)
        {
            if ((memRequirements.memoryTypeBits & (1 << i)) &&
                (memProps.memoryTypes[i].propertyFlags & (VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)))
            {
                allocInfo.memoryTypeIndex = i;
                break;
            }
        }

        vkAllocateMemory(device, &allocInfo, nullptr, &m_Memory);
        vkBindBufferMemory(device, m_Buffer, m_Memory, 0);

        // Set initial data
        void* dst;
        vkMapMemory(device, m_Memory, 0, size, 0, &dst);
        std::memcpy(dst, indices, size);
        vkUnmapMemory(device, m_Memory);
    }

    VulkanIndexBuffer::~VulkanIndexBuffer()
    {
        VkDevice device = VulkanContext::GetDevice();
        vkDestroyBuffer(device, m_Buffer, nullptr);
        vkFreeMemory(device, m_Memory, nullptr);
    }
}
