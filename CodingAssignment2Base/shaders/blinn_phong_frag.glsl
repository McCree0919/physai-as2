#version 330 core

in vec3 vWorldPos;
in vec3 vNormal;
out vec4 fragColor;

uniform vec3 uLightPos;
uniform vec3 uViewPos;
uniform vec3 uIl;
uniform vec3 uIa;
uniform vec3 uKa;
uniform vec3 uKd;
uniform vec3 uKs;
uniform float uShininess;

void main() {
    vec3 N = normalize(vNormal);
    vec3 L = normalize(uLightPos - vWorldPos);   // 从表面指向光源
    vec3 V = normalize(uViewPos - vWorldPos);    // 从表面指向相机
    vec3 H = normalize(L + V);                   // 半程向量

    float diff = max(dot(N, L), 0.0);
    float spec = pow(max(dot(N, H), 0.0), uShininess);
    if (diff <= 0.0) {
        spec = 0.0;                              // 背光面不产生高光
    }

    // C = (ka * Ia + kd * max(N·L, 0)) * Il + ks * Il * max(N·H, 0)^alpha
    vec3 color = (uKa * uIa + uKd * diff) * uIl + uKs * uIl * spec;
    fragColor = vec4(color, 1.0);
}