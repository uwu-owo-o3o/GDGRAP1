#version 330 core

out vec4 FragColor;

uniform sampler2D tex0;

uniform vec3 lightPos;
uniform vec3 lightColor;

uniform float ambientStr; 
uniform vec3 ambientColor;

uniform vec3 cameraPos;
uniform float specStr;
uniform float specPhong;

in vec2 texCoord;
in vec3 normCoord;
in vec3 fragPos;

void main()
{
    float brightness = 0.75f;

    vec3 normal = normalize(normCoord);

    // removed light position as it's not needed for directional light, negated fragPos to "flip" the light //
    vec3 lightDir = normalize(-fragPos);

    float diff =  max(dot(normal, lightDir), 0.0);

    // multiply brightness to diffuse as this is the light that is reflected that viewers see //
    vec3 diffuse = diff * lightColor * brightness;
    
    vec3 ambientCol = ambientColor * ambientStr;

    vec3 viewDir = normalize(cameraPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    float spec = pow(max(dot(reflectDir, viewDir), 0.1), specPhong);
    
    vec3 specColor = spec * specStr * lightColor;

    FragColor = vec4(specColor + diffuse + ambientCol, 1.0) * texture(tex0, texCoord);
}