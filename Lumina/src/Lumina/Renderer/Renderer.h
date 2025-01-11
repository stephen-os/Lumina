#pragma once

#include "VertexArray.h"
#include "FrameBuffer.h"

#include <unordered_map>
#include <stdexcept>

#include "../base.h"

namespace Lumina
{
    enum class RenderState {
        DepthTest,
        CullFace,
        Blend,
    };

    inline uint32_t ConvertToGLEnum(RenderState state) {
        static const std::unordered_map<RenderState, uint32_t> stateMap = {
            { RenderState::DepthTest, GL_DEPTH_TEST },
            { RenderState::CullFace, GL_CULL_FACE },
            { RenderState::Blend, GL_BLEND }
        };

        auto it = stateMap.find(state);
        if (it != stateMap.end()) {
            return it->second;
        }

        throw std::invalid_argument("Invalid RenderState");
    }

	class Renderer
	{
	public:
        void Init(); 

		void OnWindowResize(uint32_t width, uint32_t height);

		void Clear();
		void ClearColor(float r, float g, float b, float a = 1.0f);
		void Enable(RenderState param);

		void Begin(); 
		void End(); 

        void SaveFrameBufferToImage(std::string& path); 

		void Draw(const Ref<VertexArray>& vertexArray);
		uint32_t GetID() { return m_FrameBuffer->GetColorAttachment(); }
	private:
		Ref<FrameBuffer> m_FrameBuffer; 
	};
}