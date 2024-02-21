#version 330 core

out vec4 FragColor;

uniform sampler2D tex0;

uniform vec3 lightPos;
uniform vec3 lightColor;

in vec2 texCoord;
in vec3 normCoord;
in vec3 fragPos;

void main()
{
    
    vec3 normal = normalize(normCoord);
    vec3 lightDir = normalize(lightPos - fragPos);

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    FragColor = vec4(diffuse, 1.0) * texture(tex0, texCoord);

}