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
#include "Texture.h"

#include "../Utils/FileReader.h"

#include <iostream>
#include <fstream>
#include <filesystem>

namespace Lumina
{
    constexpr uint32_t MaxQuads = 1000;
    constexpr uint32_t MaxVertices = MaxQuads * 4;
    constexpr uint32_t MaxIndices = MaxQuads * 6;
    constexpr uint32_t MaxTextureSlots = 16; // Depends on GPU

    struct QuadVertex
    {
        glm::vec3 Position;
        glm::vec4 Color;
        glm::vec2 TexCoord;
        float TexIndex;
    };

    struct RendererData
    {
        Shared<FrameBuffer> RendererFB; 

        Shared<VertexArray> QuadVA;
        Shared<VertexBuffer> QuadVB;
        Shared<IndexBuffer> QuadIB;
        Shared<ShaderProgram> QuadShader;

        uint32_t QuadIndexCount = 0;
        QuadVertex* QuadVertexBufferBase = nullptr;
        QuadVertex* QuadVertexBufferPtr = nullptr;

        std::array<Shared<Texture>, MaxTextureSlots> TextureSlots;
        uint32_t TextureSlotIndex = 1; // 0 is white texture

        glm::vec4 QuadVertexPositions[4];

        Renderer::Statistics Stats;
    };

    static RendererData s_Data;

    bool FileExists(const std::string& path)
    {
        std::ifstream file(path);
        return file.good();  // If the file can be opened successfully, it's valid
    }

    void Renderer::Init()
    {

        s_Data.RendererFB = FrameBuffer::Create(); 

        s_Data.QuadVA = VertexArray::Create();

        s_Data.QuadVB = VertexBuffer::Create(MaxVertices * sizeof(QuadVertex));

        s_Data.QuadVB->SetLayout({
            { BufferDataType::Float3, "a_Position"     },
            { BufferDataType::Float4, "a_Color"        },
            { BufferDataType::Float2, "a_TexCoord"     },
            { BufferDataType::Float,  "a_TexIndex"     }
        });

        s_Data.QuadVA->AddVertexBuffer(s_Data.QuadVB); 

        // Generate index buffer
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

        // Load or create a basic shader

        std::string vertexShader = ReadFile("res/shaders/Quad.vert");
        std::string fragmentShader = ReadFile("res/shaders/Quad.frag");

        s_Data.QuadShader = ShaderProgram::Create(vertexShader, fragmentShader);

        // Create a 1x1 white texture
        uint32_t whiteTextureData = 0xffffffff;
        s_Data.TextureSlots[0] = Texture::Create(1, 1);
        s_Data.TextureSlots[0]->SetData(&whiteTextureData, sizeof(uint32_t));

        // Default quad vertex positions (in object space)
        s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
        s_Data.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
        s_Data.QuadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
        s_Data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

        // Optional: set up default tex coords (if needed for untextured)
        // For DrawQuad you might want to provide these per-vertex:
        // {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}
    }

    void Renderer::Begin()
    {
        s_Data.QuadIndexCount = 0;
        s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
        s_Data.TextureSlotIndex = 1;
    }

    void Renderer::End()
    {
        uint32_t dataSize = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase;
        s_Data.QuadVB->SetData(s_Data.QuadVertexBufferBase, dataSize);

        Flush();
    }

    void Renderer::Flush()
    {
        for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
            s_Data.TextureSlots[i]->Bind(i);

        s_Data.RendererFB->Bind(); 
        s_Data.QuadShader->Bind();
        s_Data.QuadVA->Bind();
        s_Data.QuadVA->DrawIndexed();
        s_Data.RendererFB->Unbind();

        s_Data.Stats.DrawCalls++;
    }

    void Renderer::SetResolution(uint32_t width, uint32_t height)
    {
        s_Data.RendererFB->Resize(width, height);
    }

    uint32_t Renderer::GetImage()
    {
        return s_Data.RendererFB->GetColorAttachment(); 
    }

    void Renderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
    {
        if (s_Data.QuadIndexCount >= MaxIndices)
            End(), Begin();

        glm::vec3 pos = { position.x, position.y, 0.0f };
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

        float texIndex = 0.0f; // White texture

        for (size_t i = 0; i < 4; i++)
        {
            s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
            s_Data.QuadVertexBufferPtr->Color = color;
            s_Data.QuadVertexBufferPtr->TexCoord = { /* Set default quad coords */ };
            s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
            s_Data.QuadVertexBufferPtr++;
        }

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