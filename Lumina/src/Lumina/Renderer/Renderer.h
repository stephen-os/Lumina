#pragma once

#include "VertexArray.h"
#include "FrameBuffer.h"

#include <unordered_map>
#include <stdexcept>

#include "../base.h"

namespace Lumina
{
    enum class State
    {
        DEPTH_TEST,
        CULL_FACE,
        BLEND,
    };

    inline uint32_t ConvertToGLEnum(State state) 
    {
        static const std::unordered_map<State, uint32_t> stateMap = 
        {
            { State::DEPTH_TEST, GL_DEPTH_TEST },
            { State::CULL_FACE, GL_CULL_FACE },
            { State::BLEND, GL_BLEND }
        };

        auto it = stateMap.find(state);
        if (it != stateMap.end()) 
        {
            return it->second;
        }

        throw std::invalid_argument("Invalid RenderState");
    }

    class Renderer
    {
    public:
        static void Init();
        static void Shutdown();

        static void OnWindowResize(uint32_t width, uint32_t height);

        static void Clear();
        static void ClearColor(float r, float g, float b, float a = 1.0f);
        static void Enable(State param);

        static void Begin();
        static void End();

        static void SaveFrameBufferToImage(std::string& path);

        static void Draw(const Ref<VertexArray>& vertexArray);
        static void DrawInstanced(const Ref<VertexArray>& vertexArray, uint32_t instances);

        static uint32_t GetID() { return s_FrameBuffer->GetColorAttachment(); }; 
    private:
        static Ref<FrameBuffer> s_FrameBuffer;
    };
}