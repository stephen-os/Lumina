#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in mat4 a_Transforms;

uniform mat4 u_View;
uniform mat4 u_Projection;
uniform vec3 u_ChunkOffset; // Chunk offset in world space

out vec3 v_Normal;
out vec2 v_TexCoord;
out vec3 v_Position;

void main()
{
    vec3 worldPosition = a_Position + u_ChunkOffset; // Apply chunk offset
    gl_Position = u_Projection * u_View * a_Transforms * vec4(worldPosition, 1.0);
    v_Normal = mat3(transpose(inverse(a_Transforms))) * a_Normal;
    v_Position = vec3(a_Transforms * vec4(worldPosition, 1.0));
    v_TexCoord = a_TexCoord;
}
