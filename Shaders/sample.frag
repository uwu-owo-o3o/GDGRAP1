#version 330 core

out vec4 FragColor;

uniform sampler2D tex0;

uniform vec3 lightPos;
uniform vec3 lightColor;

uniform float ambientStr; 
uniform vec3 ambientColor;

in vec2 texCoord;
in vec3 normCoord;
in vec3 fragPos;

void main()
{
    
    vec3 normal = normalize(normCoord);
    vec3 lightDir = normalize(lightPos - fragPos);

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    vec3 ambientCol = ambientColor * ambientStr;
    FragColor = vec4(diffuse + ambientCol, 1.0) * texture(tex0, texCoord);
}