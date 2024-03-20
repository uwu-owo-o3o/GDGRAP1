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

uniform vec3 lightDirection;
uniform vec3 lightColor2;
uniform float ambientStr2; 
uniform vec3 ambientColor2;
uniform float specStr2;
uniform float specPhong2;

in vec2 texCoord;
in vec3 normCoord;
in vec3 fragPos;

void main() {

    vec3 normal = normalize(normCoord);
    vec3 viewDir = normalize(cameraPos - fragPos);

    vec3 lightDir = normalize(lightPos - fragPos);

    float diff =  max(dot(normal, lightDir), 0.0);

    vec3 diffuse = diff * lightColor;
    
    vec3 ambientCol = ambientColor * ambientStr;

    vec3 reflectDir = reflect(-lightDir, normal);

    float spec = pow(max(dot(reflectDir, viewDir), 0.1), specPhong);
    
    vec3 specColor = spec * specStr * lightColor;

    float distance = length(lightPos - fragPos);

    float intensity = constant + (linear * distance) + (quadratic * (distance * distance));
    float attentuation = 1.0f / intensity;

    specColor *= attentuation;
    diffuse *= attentuation;
    ambientCol *= attentuation;
    
    vec3 pointLightResult = (specColor + diffuse + ambientCol);

    vec3 lightDir2 = normalize(-lightDirection);
    float diff2 = max(dot(normal, lightDir2), 0.0);
    vec3 diffuse2 = diff2 * lightColor2;
    vec3 ambientCol2 = ambientColor2 * ambientStr2;
    vec3 reflectDir2 = reflect(-lightDir2, normal);
    float spec2 = pow(max(dot(reflectDir2, viewDir), 0.1), specPhong2);
    vec3 specColor2 = spec2 * specStr2 * lightColor2;
    
    vec3 dirLightResult = (specColor2 + diffuse2 + ambientCol2);

    vec3 result = dirLightResult + pointLightResult;
    
   
    FragColor = vec4(result, 1.0) * texture(tex0, texCoord);
}