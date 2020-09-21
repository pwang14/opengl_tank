#version 330 core

out vec4 FragColor;

in float shadowExists;
in vec3 fragPos;

uniform bool rightHand;

uniform vec3 inP1;
uniform vec3 inP2;
uniform vec3 inP3;
uniform mat4 planeTransform;

uniform float shadowStrength;
uniform vec3 baseColor;

void main() {
    vec3 p1 = vec3(planeTransform * vec4(inP1, 1.0f));
    vec3 p2 = vec3(planeTransform * vec4(inP2, 1.0f));
    vec3 p3 = vec3(planeTransform * vec4(inP3, 1.0f));

    vec3 tplaneNorm;
    if (rightHand) tplaneNorm = cross((p3 - p2), (p1 - p2));
    else tplaneNorm = cross((p1 - p2), (p3 - p2));

    float dots[3];
    dots[0] = dot(normalize(fragPos - p1), normalize(cross(tplaneNorm, p2 - p1)));
    dots[1] = dot(normalize(fragPos - p2), normalize(cross(tplaneNorm, p3 - p2)));
    dots[2] = dot(normalize(fragPos - p3), normalize(cross(tplaneNorm, p1 - p3)));

    float maskShadow = 1.0f;
    for (int i=0; i<3; ++i) {
        if (rightHand && dots[i] < 0.0f) maskShadow = 0.0f;
        else if (!rightHand && dots[i] > 0.0f) maskShadow = 0.0f;
    }

    FragColor = vec4(baseColor, maskShadow * shadowExists * 1.0f);
}
