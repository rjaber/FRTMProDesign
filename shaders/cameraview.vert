#version 330 core

layout(location = 0) in vec3 vVertex;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vUV;

uniform mat4 M;
uniform mat4 MV;
uniform mat4 MVP;

smooth out vec2 vUVout;
smooth out vec3 vEyeSpaceNormal;
smooth out vec3 vEyeSpacePosition;

void main() {

        vUVout = vUV;

        vEyeSpacePosition = (MV * vec4(vVertex, 1)).xyz;
        vEyeSpaceNormal = normalize(mat3(MV) * vNormal);

        gl_Position = MVP * vec4(vVertex,1);

}
