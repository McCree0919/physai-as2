#version 330 core

// vertex shader: transform position to screen, pass world-space data to fragment shader
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 uModel;         // 模型空间 → 世界空间
uniform mat4 uViewProj;      // 世界空间 → 裁剪空间
uniform mat4 uNormalMatrix;  // 法线矩阵（只用左上角 3x3）

out vec3 vWorldPos;          // 世界空间位置（第 5 关算光照时用）
out vec3 vNormal;            // 世界空间法线

void main() {
    vec4 worldPos = uModel * vec4(aPos, 1.0);
    vWorldPos = worldPos.xyz;
    vNormal = mat3(uNormalMatrix) * aNormal;
    gl_Position = uViewProj * worldPos;
}