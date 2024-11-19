#version 330 core

in vec3 v_Normal;      // Interpolated normal from vertex shader
in vec3 v_FragPos;     // Interpolated fragment position
in vec2 v_TexCoord;    // Interpolated texture coordinate

uniform vec3 u_LightPos;    // Position of the point light
uniform vec3 u_ViewPos;     // Camera/view position
uniform sampler2D u_Texture; // Texture sampler

out vec4 FragColor;

void main()
{
    // Ambient lighting
    vec3 ambient = 0.1 * texture(u_Texture, v_TexCoord).rgb;

    // Diffuse lighting
    vec3 norm = normalize(v_Normal);
    vec3 lightDir = normalize(u_LightPos - v_FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * texture(u_Texture, v_TexCoord).rgb;

    // Specular lighting
    vec3 viewDir = normalize(u_ViewPos - v_FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = spec * vec3(0.5); // White specular highlight

    // Combine results
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}
