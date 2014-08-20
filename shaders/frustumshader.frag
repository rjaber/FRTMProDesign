#version 330 core

layout(location=0) out vec4 vFragColor;

uniform vec4 frustumColor;
uniform sampler2DShadow shadowMap;

in vec4 projTexCoord;

void main() {
    float visable = textureProj(shadowMap, projTexCoord);

    vFragColor =  visable * frustumColor * 0.6;
}
