#version 330 core

layout(location=0) out vec4 vFragColor;

smooth in vec2 vUVout;

uniform sampler2D textureMap;  //texture for the current mesh
uniform float useTextureMap;
uniform vec3 materialKd;
uniform vec3 materialKs;
uniform vec3 materialKa;
uniform float materialNs;

void main() {
        vFragColor = vec4(vec3(mix(materialKd, vec3(texture(textureMap, vUVout)), useTextureMap)), 1.0f);
}
