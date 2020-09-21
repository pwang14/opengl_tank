#version 330 core

out vec4 FragColor;

in vec3 fragPos;
in vec3 dataNormal;

uniform vec4 objColor;
uniform vec3 lightColor;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform float specularStrength;
uniform float flatness;

void main() {
    float lightStrength = 1.5;

    float ambientStrength = 0.25;
    vec3 ambient = ambientStrength * lightColor;

    float distance = length(lightPos - fragPos);
    float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * distance * distance);

    vec3 norm = normalize(dataNormal);
    vec3 lightDir = normalize(lightPos - fragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * attenuation;

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor * attenuation;

    vec4 result = lightStrength * mix(vec4(ambient + diffuse + specular, 1.0f) * objColor, objColor, flatness);
    FragColor = result; 
}
