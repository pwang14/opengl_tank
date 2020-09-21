#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out float shadowExists;
out vec3 fragPos;

uniform float offset;

uniform vec3 lightPos;

uniform bool rightHand;

uniform vec3 inP1;
uniform vec3 inP2;
uniform vec3 inP3;
uniform mat4 planeTransform;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    vec3 p1 = vec3(planeTransform * vec4(inP1, 1.0f));
    vec3 p2 = vec3(planeTransform * vec4(inP2, 1.0f));
    vec3 p3 = vec3(planeTransform * vec4(inP3, 1.0f));

    vec3 tplaneNorm;
    if (rightHand) tplaneNorm = cross((p3-p2), (p1-p2));
    else tplaneNorm = cross((p1-p2), (p3-p2));

    vec3 taPos = vec3(model * vec4(aPos, 1.0f));
    vec3 pointDir = normalize(taPos - p1);

    if (dot(pointDir, normalize(tplaneNorm)) < 0) {
        float D = dot(p1, tplaneNorm);
        float t = (D - dot(tplaneNorm, taPos))/dot(tplaneNorm, tplaneNorm);
        taPos = taPos + t * tplaneNorm;
    }

    vec3 rayDir = normalize(taPos - lightPos);        
    shadowExists = 0.0f;
    vec3 tempPos = vec3(0.0f, 0.0f, 0.0f); 

    if (dot(rayDir, normalize(tplaneNorm)) <= -0.01) {
        float D = dot(p1, tplaneNorm);
        float t = (D - dot(tplaneNorm, lightPos))/dot(tplaneNorm, rayDir);
        vec3 intersect = lightPos + t * rayDir - 0.001 * rayDir + offset * normalize(tplaneNorm); 
        shadowExists = 1.0f;
        tempPos = intersect; 
    }

    fragPos = tempPos;
    gl_Position = projection * view * vec4(tempPos, 1.0f); 
}
