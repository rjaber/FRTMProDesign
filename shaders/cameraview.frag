#version 330 core

layout(location=0) out vec4 vFragColor;


uniform mat4 MV;
uniform sampler2D textureMap;  //texture for the current mesh
uniform float useTextureMap;
uniform vec3 materialKd;
uniform vec3 materialKs;
uniform vec3 materialKa;
uniform float materialNs;
uniform vec3 lightPosition;

smooth in vec3 vEyeSpaceNormal;
smooth in vec3 vEyeSpacePosition;
smooth in vec2 vUVout;

void calculateBlinnPhongLight(out vec3 blinnPhongColor) {

    vec4 vEyeSpaceLightPosition = normalize(MV * vec4(lightPosition,1));
    float diffuse = max(0.0f, dot(vEyeSpaceNormal, vec3(vEyeSpaceLightPosition)));

    vec3 L = normalize(vEyeSpaceLightPosition.xyz - vEyeSpacePosition);     //light vector
    vec3 eye = -normalize(vEyeSpacePosition);

    vec3 halfVector = normalize(L + eye);
    float specular = max(0.0, dot(vEyeSpaceNormal, halfVector));

    float materialShininess = materialNs;

    if (diffuse == 0.0)
       specular = 0.0;
    else
       specular = pow(specular, materialShininess);

    const float SHININESS_STRENGTH = 0.1f;

    blinnPhongColor = materialKa +
            (materialKd * diffuse) +
            (materialKs * specular * SHININESS_STRENGTH);
}

void main() {
     vec3 blinnPhongLightRGB;
     if(useTextureMap == 0.0f)
         calculateBlinnPhongLight(blinnPhongLightRGB);

     vec3 fragRGB = mix(blinnPhongLightRGB , vec3(texture(textureMap, vUVout)), useTextureMap);
     vFragColor = vec4(fragRGB, 1.0f);
}
