#version 330 core

in vec3 v_Normal;  // The normal vector passed from the vertex shader
out vec4 FragColor;

void main()
{
    vec3 normalizedNormal = normalize(v_Normal);
    vec3 color = (normalizedNormal + 1.0) * 0.5;
    FragColor = vec4(color, 1.0);
}