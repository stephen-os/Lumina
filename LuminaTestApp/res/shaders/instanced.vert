#version 330 core

// Standard vertex attributes
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;

// Instance transformation matrix (passed per instance)
layout(location = 3) in mat4 a_InstanceTransform;

// Uniforms
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform vec3 u_CameraPosition;

// Outputs to the fragment shader
out vec3 v_Normal;
out vec2 v_TexCoord;
out vec3 v_CameraPosition;
out vec3 v_Position;

void main()
{
    // Apply instance transformation and camera view/projection
    gl_Position = u_Projection * u_View * a_InstanceTransform * vec4(a_Position, 1.0);

    // Transform normal (optional for shading)
    v_Normal = mat3(transpose(inverse(a_InstanceTransform))) * a_Normal;

    v_CameraPosition = u_CameraPosition;

    v_Position = vec3(a_InstanceTransform * vec4(a_Position, 1.0));

    // Pass texture coordinates to the fragment shader
    v_TexCoord = a_TexCoord;
}
