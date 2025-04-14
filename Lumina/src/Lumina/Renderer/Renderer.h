#pragma once
#include "VertexArray.h"
#include "Texture.h"
#include "ShaderProgram.h"

#include <glm/glm.hpp>

#include "Cameras/Camera.h"

namespace Lumina
{
    struct QuadAttributes
    {
        glm::vec3 Position = { 0, 0, 0 };
        glm::vec2 Size = { 1, 1 };
        float Rotation = 0.0f;
        Shared<Texture> Texture = nullptr;
		glm::vec4 TextureCoords = { 0, 0, 1, 1 };
        glm::vec4 TintColor = glm::vec4(1.0f);
		Shared<ShaderProgram> Shader = nullptr;
    };

    class Renderer
    {
    public:
        // Core Renderer Functions
        static void Init();
        static void Shutdown();

		// Begin and End Batch
        static void Begin(Camera& camera);
        static void End();
        
        static void StartBatch();
		static void EndBatch();

		// Draw Batch
        static void Flush();

        // Resolution Management
        static void SetResolution(uint32_t width, uint32_t height);
        static glm::vec2 GetResolution();
        
        // Get the image stored in the Framebuffer
        static uint32_t GetImage();

        // Basic Drawing Functions
        static void DrawQuad(const QuadAttributes& attributes);
        // static void DrawQuad(const QuadAttributes& attributes, const Shared<ShaderProgram>& shader);
       
        // Statistics Tracking
        struct Statistics
        {
            uint32_t DrawCalls = 0;
            uint32_t QuadCount = 0;
            uint32_t TexturesUsed = 0;
			uint32_t ShadersUsed = 0;

            uint32_t GetTotalVertexCount() const { return QuadCount * 4; }
            uint32_t GetTotalIndexCount() const { return QuadCount * 6; }
        };

        static Statistics GetStats();
        static void ResetStats();
    };
}