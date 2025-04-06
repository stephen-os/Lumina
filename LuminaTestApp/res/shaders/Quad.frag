#version 330 core

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;

out vec4 FragColor;

void main()
{
    FragColor = v_Color;
}
