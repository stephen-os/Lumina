#pragma once
#include "VertexArray.h"
#include "Texture.h"
#include <glm/glm.hpp>

namespace Lumina
{
    class Renderer
    {
    public:
        // Core Renderer Functions
        static void Init();
        static void Shutdown();
        static void Begin(const glm::mat4& viewProjection = glm::mat4(1.0f));
        static void End();
        static void Flush();

        // Resolution Management
        static void SetResolution(uint32_t width, uint32_t height);
        static uint32_t GetImage();
        static glm::vec2 GetResolution();

        // Basic Drawing Functions
        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Shared<Texture>& texture, const glm::vec4& tintColor = glm::vec4(1.0f));

        // Extended Drawing Functions
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Shared<Texture>& texture, const glm::vec4& tintColor = glm::vec4(1.0f));
        static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);
        static void DrawQuad(const glm::mat4& transform, const Shared<Texture>& texture, const glm::vec4& tintColor = glm::vec4(1.0f));

        // Rotated Quad Drawing
        static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
        static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Shared<Texture>& texture, const glm::vec4& tintColor = glm::vec4(1.0f));
        static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);
        static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Shared<Texture>& texture, const glm::vec4& tintColor = glm::vec4(1.0f));

        // Textured Rect Drawing (for sprite sheets/atlases)
        static void DrawTexturedRect(const glm::vec2& position, const glm::vec2& size, const Shared<Texture>& texture,
            const glm::vec2& texCoordMin, const glm::vec2& texCoordMax,
            const glm::vec4& tintColor = glm::vec4(1.0f));

        // Statistics Tracking
        struct Statistics
        {
            uint32_t DrawCalls = 0;
            uint32_t QuadCount = 0;

            uint32_t GetTotalVertexCount() const { return QuadCount * 4; }
            uint32_t GetTotalIndexCount() const { return QuadCount * 6; }
        };

        static void ResetStats();
        static Statistics GetStats();

    private:
        // Internal helper for drawing quads with custom texture coordinates
        static void DrawQuadInternal(const glm::mat4& transform, const Shared<Texture>& texture,
            const glm::vec2 texCoords[4], const glm::vec4& tintColor);
    };
}