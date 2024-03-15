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

uniform float constant;
uniform float linear;
uniform float quadratic;

in vec2 texCoord;
in vec3 normCoord;
in vec3 fragPos;

void main() {

    vec3 normal = normalize(normCoord);

    vec3 lightDir = normalize(lightPos - fragPos);

    float diff =  max(dot(normal, lightDir), 0.0);

    vec3 diffuse = diff * lightColor;
    
    vec3 ambientCol = ambientColor * ambientStr;

    vec3 viewDir = normalize(cameraPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    float spec = pow(max(dot(reflectDir, viewDir), 0.1), specPhong);
    
    vec3 specColor = spec * specStr * lightColor;

    float distance = length(lightPos - fragPos);

    float intensity = constant + (linear * distance) + (quadratic * (distance * distance));
    float attentuation = 1.0f / intensity;

    specColor *= attentuation;
    diffuse *= attentuation;
    ambientCol *= attentuation;
    

    FragColor = vec4(specColor + diffuse + ambientCol, 1.0);
}