#version 330 core

in vec3 v_Normal;
in vec3 v_Position;
in vec3 v_CameraPosition;
in vec2 v_TexCoord;

out vec4 FragColor;

uniform vec3 lightPos = vec3(4.0, -5.0, 3.0);
uniform vec3 lightColor = vec3(1.0, 1.0, 1.0);

uniform sampler2D u_Texture;

void main()
{
    // Ambient lighting
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse lighting
    vec3 norm = normalize(v_Normal);
    vec3 lightDir = normalize(lightPos - v_Position);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular lighting
    float specularStrength = 0.5;
    vec3 viewDir = normalize(v_CameraPosition - v_Position);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32); // 32 is the shininess factor
    vec3 specular = specularStrength * spec * lightColor;

    // Texture color
    vec3 texColor = texture(u_Texture, v_TexCoord).rgb;

    // Combine results
    vec3 result = (ambient + diffuse + specular) * texColor;
    FragColor = vec4(result, 1.0);
}
