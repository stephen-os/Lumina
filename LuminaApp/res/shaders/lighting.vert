#version 330 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;

out vec3 v_Normal;
out vec3 v_Position; 

uniform mat4 u_MVP;

void main()
{
    gl_Position = u_MVP * vec4(a_Position, 1.0);
    v_Normal = (u_MVP * vec4(a_Normal, 1)).xyz;
    v_Position = (u_MVP * vec4(a_Position, 1)).xyz; 
}
