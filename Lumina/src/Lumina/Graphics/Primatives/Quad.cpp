#include "Quad.h"

namespace Lumina
{
	std::string Quad::GetVertexShader()
	{
		return R"(
			#version 430 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;
			layout(location = 2) in vec2 a_TexCoord;
			layout(location = 3) in float a_TexIndex;

			out vec4 v_Color;
			out vec2 v_TexCoord;
			flat out float v_TexIndex;

			uniform mat4 u_ViewProjection;

			void main()
			{
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
				v_Color = a_Color;
				v_TexCoord = a_TexCoord;
				v_TexIndex = a_TexIndex;
			}
		)";
	}

	std::string Quad::GetFragmentShader()
	{
		return R"(
			#version 430 core

			layout(location = 0) out vec4 o_Color;

			in vec4 v_Color;
			in vec2 v_TexCoord;
			flat in float v_TexIndex;

			layout (binding = 0) uniform sampler2D u_Textures[32];

			void main()
			{
				vec4 texColor = vec4(1.0);
    
				// Determine which texture to sample based on v_TexIndex
				int index = int(v_TexIndex);
    
				texColor = texture(u_Textures[index], v_TexCoord);

				// Discard fragment if alpha is 0
				if (texColor.a < 0.001)
					discard; 

				// Apply tint color
				o_Color = texColor * v_Color;
			}
		)";
	}
}