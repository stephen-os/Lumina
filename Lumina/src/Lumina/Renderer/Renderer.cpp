#include "Renderer.h"

#include <array>
#include <string>

#include <glm/gtc/matrix_transform.hpp>

#include "../Core/Aliases.h"

#include "ShaderProgram.h"
#include "VertexArray.h"
#include "Buffer.h"
#include "BufferLayout.h"
#include "FrameBuffer.h"

#include "../Utils/FileReader.h"

#include <iostream>
#include <fstream>
#include <filesystem>

namespace Lumina
{
    // Constants
    constexpr uint32_t MaxQuads = 10000;  // Increased from 1000
    constexpr uint32_t MaxVertices = MaxQuads * 4;
    constexpr uint32_t MaxIndices = MaxQuads * 6;
    constexpr uint32_t MaxTextureSlots = 16; // Depends on GPU

    // Vertex structure
    struct QuadVertex
    {
        glm::vec3 Position;
        glm::vec4 Color;
        glm::vec2 TexCoord;
        float TexIndex;
    };

    // Renderer data storage
    struct RendererData
    {
        // Core renderer resources
        Shared<FrameBuffer> RendererFB;
        Shared<VertexArray> QuadVA;
        Shared<VertexBuffer> QuadVB;
        Shared<IndexBuffer> QuadIB;
        Shared<ShaderProgram> QuadShader;

        // Batch rendering data
        uint32_t QuadIndexCount = 0;
        QuadVertex* QuadVertexBufferBase = nullptr;
        QuadVertex* QuadVertexBufferPtr = nullptr;

        // Texture management
        std::array<Shared<Texture>, MaxTextureSlots> TextureSlots;
        uint32_t TextureSlotIndex = 1; // 0 is white texture

        // Quad rendering data
        glm::vec4 QuadVertexPositions[4];
        glm::vec2 TexCoords[4];

        // View-projection matrix for camera support
        glm::mat4 ViewProjectionMatrix = glm::mat4(1.0f);

        // Resolution tracking
        uint32_t Width = 800;
        uint32_t Height = 600;

        // Performance statistics
        Renderer::Statistics Stats;
    };

    static RendererData s_Data;

    // Helper function to check if a file exists
    bool FileExists(const std::string& path)
    {
        std::ifstream file(path);
        return file.good();
    }

    void Renderer::Init()
    {
        // Create framebuffer
        s_Data.RendererFB = FrameBuffer::Create();

        // Create vertex arrays and buffers
        s_Data.QuadVA = VertexArray::Create();
        s_Data.QuadVB = VertexBuffer::Create(MaxVertices * sizeof(QuadVertex));

        // Set up buffer layout
        s_Data.QuadVB->SetLayout({
            { BufferDataType::Float3, "a_Position" },
            { BufferDataType::Float4, "a_Color" },
            { BufferDataType::Float2, "a_TexCoord" },
            { BufferDataType::Float,  "a_TexIndex" }
            });

        s_Data.QuadVA->AddVertexBuffer(s_Data.QuadVB);

        // Generate index buffer for quads
        std::vector<uint32_t> quadIndices(MaxIndices);
        uint32_t offset = 0;
        for (uint32_t i = 0; i < MaxIndices; i += 6)
        {
            quadIndices[i + 0] = offset + 0;
            quadIndices[i + 1] = offset + 1;
            quadIndices[i + 2] = offset + 2;

            quadIndices[i + 3] = offset + 2;
            quadIndices[i + 4] = offset + 3;
            quadIndices[i + 5] = offset + 0;

            offset += 4;
        }

        s_Data.QuadIB = IndexBuffer::Create(quadIndices.data(), MaxIndices);
        s_Data.QuadVA->SetIndexBuffer(s_Data.QuadIB);

        // Allocate vertex buffer memory
        s_Data.QuadVertexBufferBase = new QuadVertex[MaxVertices];

        // Load shaders
        std::string vertexShader = ReadFile("res/shaders/Quad.vert");
        std::string fragmentShader = ReadFile("res/shaders/Quad.frag");
        s_Data.QuadShader = ShaderProgram::Create(vertexShader, fragmentShader);

        // Create a 1x1 white texture for basic colored quads
        uint32_t whiteTextureData = 0xffffffff;
        s_Data.TextureSlots[0] = Texture::Create(1, 1);
        s_Data.TextureSlots[0]->SetData(&whiteTextureData, sizeof(uint32_t));

        // Set up default quad vertex positions in object space
        s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };  // Bottom left
        s_Data.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };   // Bottom right
        s_Data.QuadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };   // Top right
        s_Data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };  // Top left

        // Set up default texture coordinates
        s_Data.TexCoords[0] = { 0.0f, 0.0f };  // Bottom left
        s_Data.TexCoords[1] = { 1.0f, 0.0f };  // Bottom right
        s_Data.TexCoords[2] = { 1.0f, 1.0f };  // Top right
        s_Data.TexCoords[3] = { 0.0f, 1.0f };  // Top left
    }

    void Renderer::Shutdown()
    {
        // Free allocated memory
        delete[] s_Data.QuadVertexBufferBase;
    }

    void Renderer::Begin(const glm::mat4& viewProjection)
    {
        // Store view-projection matrix
        s_Data.ViewProjectionMatrix = viewProjection;

        // Reset batch data
        s_Data.QuadIndexCount = 0;
        s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
        s_Data.TextureSlotIndex = 1; // 0 is reserved for white texture
    }

    void Renderer::End()
    {
        // Calculate data size and upload to GPU
        uint32_t dataSize = static_cast<uint32_t>((uint8_t*)s_Data.QuadVertexBufferPtr -
            (uint8_t*)s_Data.QuadVertexBufferBase);

        if (dataSize > 0) {
            s_Data.QuadVB->SetData(s_Data.QuadVertexBufferBase, dataSize);
            Flush();
        }
    }

    void Renderer::Flush()
    {
        // Skip if no data to render
        if (s_Data.QuadIndexCount == 0)
            return;

        // Bind all used textures
        for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
            s_Data.TextureSlots[i]->Bind(i);

        // Render to framebuffer
        s_Data.RendererFB->Bind();

        // Set view-projection matrix
        s_Data.QuadShader->Bind();

        // Draw the batch
        s_Data.QuadVA->Bind();
        s_Data.QuadVA->DrawIndexed();

        s_Data.RendererFB->Unbind();

        // Update stats
        s_Data.Stats.DrawCalls++;
    }

    void Renderer::SetResolution(uint32_t width, uint32_t height)
    {
        if (width == 0 || height == 0) {
            std::cerr << "Invalid resolution: " << width << "x" << height << std::endl;
            return;
        }

        s_Data.Width = width;
        s_Data.Height = height;
        s_Data.RendererFB->Resize(width, height);
    }

    uint32_t Renderer::GetImage()
    {
        return s_Data.RendererFB->GetColorAttachment();
    }

    glm::vec2 Renderer::GetResolution()
    {
        return { s_Data.Width, s_Data.Height };
    }

    void Renderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
    {
        DrawQuad(glm::vec3(position.x, position.y, 0.0f), size, color);
    }

    void Renderer::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
    {
        // Create transform matrix
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
            glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));

        DrawQuad(transform, color);
    }

    void Renderer::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
    {
        // Check if we need to flush the batch
        if (s_Data.QuadIndexCount >= MaxIndices)
            End(), Begin(s_Data.ViewProjectionMatrix);

        // Use white texture
        float texIndex = 0.0f;

        // Add vertices to batch
        for (size_t i = 0; i < 4; i++)
        {
            s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
            s_Data.QuadVertexBufferPtr->Color = color;
            s_Data.QuadVertexBufferPtr->TexCoord = s_Data.TexCoords[i];
            s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
            s_Data.QuadVertexBufferPtr++;
        }

        // Update indices and stats
        s_Data.QuadIndexCount += 6;
        s_Data.Stats.QuadCount++;
    }

    void Renderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Shared<Texture>& texture, const glm::vec4& tintColor)
    {
        DrawQuad(glm::vec3(position.x, position.y, 0.0f), size, texture, tintColor);
    }

    void Renderer::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Shared<Texture>& texture, const glm::vec4& tintColor)
    {
        // Create transform matrix
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
            glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));

        DrawQuad(transform, texture, tintColor);
    }

    void Renderer::DrawQuad(const glm::mat4& transform, const Shared<Texture>& texture, const glm::vec4& tintColor)
    {
        // Use default texture coordinates
        DrawQuadInternal(transform, texture, s_Data.TexCoords, tintColor);
    }

    void Renderer::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
    {
        DrawRotatedQuad(glm::vec3(position.x, position.y, 0.0f), size, rotation, color);
    }

    void Renderer::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
    {
        // Create transform with rotation
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
            glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f)) *
            glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));

        DrawQuad(transform, color);
    }

    void Renderer::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Shared<Texture>& texture, const glm::vec4& tintColor)
    {
        DrawRotatedQuad(glm::vec3(position.x, position.y, 0.0f), size, rotation, texture, tintColor);
    }

    void Renderer::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Shared<Texture>& texture, const glm::vec4& tintColor)
    {
        // Create transform with rotation
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
            glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f)) *
            glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));

        DrawQuad(transform, texture, tintColor);
    }

    void Renderer::DrawTexturedRect(const glm::vec2& position, const glm::vec2& size,
        const Shared<Texture>& texture,
        const glm::vec2& texCoordMin, const glm::vec2& texCoordMax,
        const glm::vec4& tintColor)
    {
        // Create custom texture coordinates for sprite sheet/atlas support
        glm::vec2 customTexCoords[4] = {
            { texCoordMin.x, texCoordMin.y },  // Bottom left
            { texCoordMax.x, texCoordMin.y },  // Bottom right
            { texCoordMax.x, texCoordMax.y },  // Top right
            { texCoordMin.x, texCoordMax.y }   // Top left
        };

        // Create transform
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, 0.0f)) *
            glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));

        DrawQuadInternal(transform, texture, customTexCoords, tintColor);
    }

    void Renderer::DrawQuadInternal(const glm::mat4& transform, const Shared<Texture>& texture,
        const glm::vec2 texCoords[4], const glm::vec4& tintColor)
    {
        // Check if we need to flush the batch
        if (s_Data.QuadIndexCount >= MaxIndices)
            End(), Begin(s_Data.ViewProjectionMatrix);

        float texIndex = 0.0f;

        // Check if the texture already exists in the batch's texture slots
        for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
        {
            if (s_Data.TextureSlots[i]->GetID() == texture->GetID())
            {
                texIndex = static_cast<float>(i);
                break;
            }
        }

        // If not found, bind a new one
        if (texIndex == 0.0f)
        {
            if (s_Data.TextureSlotIndex >= MaxTextureSlots)
            {
                End(), Begin(s_Data.ViewProjectionMatrix); // Flush batch
            }

            texIndex = static_cast<float>(s_Data.TextureSlotIndex);
            s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
            s_Data.TextureSlotIndex++;
        }

        // Add vertices to batch
        for (size_t i = 0; i < 4; i++)
        {
            s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
            s_Data.QuadVertexBufferPtr->Color = tintColor;
            s_Data.QuadVertexBufferPtr->TexCoord = texCoords[i];
            s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
            s_Data.QuadVertexBufferPtr++;
        }

        // Update indices and stats
        s_Data.QuadIndexCount += 6;
        s_Data.Stats.QuadCount++;
    }

    void Renderer::ResetStats()
    {
        memset(&s_Data.Stats, 0, sizeof(Statistics));
    }

    Renderer::Statistics Renderer::GetStats()
    {
        return s_Data.Stats;
    }
}