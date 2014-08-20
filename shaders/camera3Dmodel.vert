#version 330 core

layout(location = 0) in vec3 vVertex;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vUV;

uniform mat4 MVP;

smooth out vec2 vUVout;

void main() {

        vUVout = vUV;

        gl_Position = MVP * vec4(vVertex, 1);
}
