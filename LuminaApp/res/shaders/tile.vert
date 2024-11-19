#version 330 core

layout(location = 0) in vec3 a_Position;  // Vertex position
layout(location = 1) in vec3 a_Normal;    // Vertex normal
layout(location = 2) in vec2 a_TexCord;   // Texture coordinates

uniform mat4 u_Transform;  // Model transform matrix
uniform mat4 u_View;       // View matrix
uniform mat4 u_Projection; // Projection matrix

out vec3 v_Normal;         // Pass normal to fragment shader
out vec3 v_FragPos;        // Fragment position in world space
out vec2 v_TexCoord;       // Pass texture coordinate to fragment shader

void main()
{
    v_FragPos = vec3(u_Transform * vec4(a_Position, 1.0));
    v_Normal = mat3(transpose(inverse(u_Transform))) * a_Normal; // Correct normal for non-uniform scaling
    v_TexCoord = a_TexCord;

    gl_Position = u_Projection * u_View * vec4(v_FragPos, 1.0);
}
