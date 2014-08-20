#version 330 core

layout(location = 0) in vec3 vVertex;	 //vertex position
layout(location = 1) in vec3 vNormal;	 //vertex normal
layout(location = 2) in vec2 vUV;        //vertex uv coordinates

uniform mat4 MVP;
uniform mat4 projectionMat;

out vec4 projTexCoord;

uniform vec4 yClipPlane;
float gl_ClipDistance[1];

void main() {
    projTexCoord = projectionMat * ( vec4(vVertex, 1.0f) );

    gl_ClipDistance[0] = dot(vec4(vVertex, 1.0f), yClipPlane);
    gl_Position = MVP * vec4(vVertex, 1.0f);
}


