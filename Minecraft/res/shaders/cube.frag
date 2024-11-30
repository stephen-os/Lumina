#version 330 core

in vec3 v_Normal;
in vec3 v_Position;
in vec2 v_TexCoord;

out vec4 FragColor;

uniform vec3 lightColor = vec3(1.0, 1.0, 1.0);
uniform sampler2D u_Texture;

void main()
{
    // Uniform lighting (simulate ambient lighting)
    vec3 uniformLighting = lightColor; // Full brightness from light color

    // Texture color
    vec3 texColor = texture(u_Texture, v_TexCoord).rgb;

    FragColor = vec4(texColor, 1.0);
}
