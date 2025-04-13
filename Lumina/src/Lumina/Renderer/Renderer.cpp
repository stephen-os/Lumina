#include "Renderer.h"

#include <glad/glad.h>

#include <array>
#include <string>

#include <spdlog/spdlog.h>

#include <glm/gtc/matrix_transform.hpp>

#include "../Core/Aliases.h"

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
	constexpr uint32_t MaxShaderSlots = 16;

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

        // Batch rendering data
        uint32_t QuadIndexCount = 0;
        QuadVertex* QuadVertexBufferBase = nullptr;
        QuadVertex* QuadVertexBufferPtr = nullptr;

        // Shader management
		std::array<Shared<ShaderProgram>, MaxShaderSlots> ShaderSlots;
		uint32_t ShaderSlotIndex = 0; // 0 is default shader
        uint32_t ShaderSlotSize = 1;

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

        // Create a default shader at slot 0
        std::string vertexShader = ReadFile("res/shaders/Quad.vert");
        std::string fragmentShader = ReadFile("res/shaders/Quad.frag");
		s_Data.ShaderSlots[0] = ShaderProgram::Create(vertexShader, fragmentShader);

        // Create a 1x1 white texture for basic colored quads
        uint32_t whiteTextureData = 0xffffffff;
        s_Data.TextureSlots[0] = Texture::Create(1, 1);
        s_Data.TextureSlots[0]->SetData(&whiteTextureData, sizeof(uint32_t));

        // Set up default quad vertex positions in object space
        s_Data.QuadVertexPositions[0] = { -1.0f, -1.0f, 0.0f, 1.0f };  // Bottom left
        s_Data.QuadVertexPositions[1] = {  1.0f, -1.0f, 0.0f, 1.0f };  // Bottom right
        s_Data.QuadVertexPositions[2] = {  1.0f,  1.0f, 0.0f, 1.0f };  // Top right
        s_Data.QuadVertexPositions[3] = { -1.0f,  1.0f, 0.0f, 1.0f };  // Top left

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

    void Renderer::Begin(Camera& camera)
    {
        s_Data.RendererFB->Bind();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Store view-projection matrix
        s_Data.ViewProjectionMatrix = camera.GetViewMatrix() * camera.GetProjectionMatrix();

        StartBatch(); 
    }

    void Renderer::End()
    {
		EndBatch();

        s_Data.RendererFB->Unbind();
    }

	void Renderer::StartBatch()
	{
		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
		s_Data.TextureSlotIndex = 1; // 0 is reserved for white texture
		s_Data.ShaderSlotIndex = 0;  // 0 is reserved for default shader
	}

	void Renderer::EndBatch()
	{
        // Calculate data size and upload to GPU
        uint32_t dataSize = static_cast<uint32_t>((uint8_t*)s_Data.QuadVertexBufferPtr -
            (uint8_t*)s_Data.QuadVertexBufferBase);

        if (dataSize > 0) 
        {
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

        // Bind the active shader
		s_Data.ShaderSlots[s_Data.ShaderSlotIndex]->Bind();
		s_Data.ShaderSlots[s_Data.ShaderSlotIndex]->SetUniformMat4("u_ViewProjection", s_Data.ViewProjectionMatrix);

        // Draw the batch
        s_Data.QuadVA->Bind();
        s_Data.QuadVA->DrawIndexed();
        s_Data.QuadVA->Unbind();

        // Update stats
        s_Data.Stats.DrawCalls++;
    }

    void Renderer::SetResolution(uint32_t width, uint32_t height)
    {
        if (width == 0 || height == 0) 
        {
            std::cerr << "Invalid resolution: " << width << "x" << height << std::endl;
            return;
        }

        glViewport(0, 0, width, height);

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

    void Renderer::DrawQuad(const QuadAttributes& attributes)
    {
        if (s_Data.QuadIndexCount >= MaxIndices)
        {
            EndBatch();
            StartBatch();
        }

        // We have a shader
		if (attributes.Shader)
		{
            uint32_t shaderIndex = 0;

			// Check if the shader is already in the slots
            for (uint32_t i = 1; i < s_Data.ShaderSlotSize; i++)
            {
                // We have found the shader we want to use
                if (s_Data.ShaderSlots[i] == attributes.Shader)
                {
					if (s_Data.ShaderSlotIndex != i)
					{
                        // Clear old batch
                        EndBatch();
                        StartBatch();

						// Set the new shader slot index
						s_Data.ShaderSlotIndex = i;
						shaderIndex = i;
                        break;
					}
                }
            }

            // Must be a new shader
            if (shaderIndex == 0)
            {
                // Clear old batch
                EndBatch();
                StartBatch();

                // Add new shader to the slots
                s_Data.ShaderSlotIndex = s_Data.ShaderSlotSize;
                s_Data.ShaderSlots[s_Data.ShaderSlotIndex] = attributes.Shader;
                s_Data.ShaderSlotSize++;
            }
		}
        // We dont have a new shader so use default
        else
        {
            // If not already on default shader,
            // clear batch and switch to it
			if (s_Data.ShaderSlotIndex != 0)
			{
				EndBatch();
				StartBatch();
                s_Data.ShaderSlotIndex = 0;
			}
        }

        // Compute transform
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), attributes.Position) *
            glm::rotate(glm::mat4(1.0f), attributes.Rotation, glm::vec3(0.0f, 0.0f, 1.0f)) *
            glm::scale(glm::mat4(1.0f), glm::vec3(attributes.Size, 1.0f));

        // Handle texture slot
        float texIndex = 0.0f;
        if (attributes.Texture)
        {
            for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
            {
                if (s_Data.TextureSlots[i] == attributes.Texture)
                {
                    texIndex = static_cast<float>(i);
                    break;
                }
            }

            if (texIndex == 0.0f)
            {
                if (s_Data.TextureSlotIndex >= MaxTextureSlots)
                {
                    EndBatch();
                    StartBatch();
                }

                texIndex = static_cast<float>(s_Data.TextureSlotIndex);
                s_Data.TextureSlots[s_Data.TextureSlotIndex] = attributes.Texture;
                s_Data.TextureSlotIndex++;
            }
        }

        // Extract custom UV bounds
        glm::vec2 uvMin = { attributes.TextureCoords.x, attributes.TextureCoords.y };
        glm::vec2 uvMax = { attributes.TextureCoords.z, attributes.TextureCoords.w };

        // Map UVs per corner (same order as QuadVertexPositions: bottom-left, bottom-right, top-right, top-left)
        glm::vec2 uvs[4] = {
            { uvMin.x, uvMin.y },
            { uvMax.x, uvMin.y },
            { uvMax.x, uvMax.y },
            { uvMin.x, uvMax.y }
        };

        for (size_t i = 0; i < 4; i++)
        {
            s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
            s_Data.QuadVertexBufferPtr->Color = attributes.TintColor;
            s_Data.QuadVertexBufferPtr->TexCoord = uvs[i];
            s_Data.QuadVertexBufferPtr->TexIndex = texIndex;
            s_Data.QuadVertexBufferPtr++;
        }

        s_Data.QuadIndexCount += 6;
        s_Data.Stats.QuadCount++;
    }

	Renderer::Statistics Renderer::GetStats()
	{
		return s_Data.Stats;
	}

	void Renderer::ResetStats()
	{
		memset(&s_Data.Stats, 0, sizeof(Statistics));
	}
}