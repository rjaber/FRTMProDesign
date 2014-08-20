#version 330 core

layout(location=0) out vec4 vFragColor;

uniform mat4 MV;

uniform sampler2D textureMap;  //texture for the current mesh
uniform sampler2DShadow shadowMap[8];
uniform sampler2DShadow shadowMapPotential;

uniform float useTextureMap;
uniform vec3 materialKd;
uniform vec3 materialKs;
uniform vec3 materialKa;
uniform float materialNs;

uniform vec3 lightPosition;
uniform int numOfCams;
uniform bool is180FOVOn;

smooth in vec3 vEyeSpaceNormal;
smooth in vec3 vEyeSpacePosition;
smooth in vec2 vUVout;

smooth in vec4 projTexCoord[8];         //texture coords for camera's  view coverage
smooth in vec4 projPotentialCoord;      //texture coords for camera's 180 FOV coverage

const vec4 cameraCoverageColor = vec4(0.4f , 0.4f, 0.0f, 1.0f);
const float CAMERA_PROJECTION_STRENGTH = 0.8f;
const float CAMERA_180_PROJECTION_STRENGTH = 0.6f;

void calculateBlinnPhongLight(out vec3 blinnPhongColor) {

    vec4 vEyeSpaceLightPosition = normalize(MV * vec4(lightPosition, 1));
    float diffuse = max(0.0f, dot(vEyeSpaceNormal, vec3(vEyeSpaceLightPosition)));

    vec3 L = normalize(vEyeSpaceLightPosition.xyz - vEyeSpacePosition);  //light vector
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

bool isValidTextCoord(const in vec4 texCoord){
    return (texCoord.s >= 0.0 && texCoord.s <= 1.0) &&
            (texCoord.t >= 0.0 && texCoord.t <= 1.0);
}

void cameraProjection(const in int camNum, inout vec4 addColor) {
    vec4 projTexColor = vec4(0.0f);
    if(projTexCoord[camNum].z > 0.0f){
       vec4 pTexCoord = projTexCoord[camNum]/projTexCoord[camNum].w;
       if(isValidTextCoord(pTexCoord))
           projTexColor = cameraCoverageColor;
    }

    float visable = textureProj(shadowMap[camNum], projTexCoord[camNum]);
    addColor += visable * projTexColor * CAMERA_PROJECTION_STRENGTH;
}

void camera180Projection(inout vec4 addColor) {
    vec4 projPotColor = vec4(0.0f);
    if(projPotentialCoord.z > 0.0f){
       vec4 pTexCoord = projPotentialCoord/projPotentialCoord.w;
       if(isValidTextCoord(pTexCoord))
           projPotColor = cameraCoverageColor;
    }

    float visablePotential = textureProj(shadowMapPotential, projPotentialCoord);
        addColor += visablePotential * projPotColor * CAMERA_180_PROJECTION_STRENGTH;
}


void main() {
        vec4 totalColor = vec4(0.0f);        

        /*In opengl 3.3 sampler arrays can only be indexed with constants.
          Thus, the for loop is unrolled. The unrolled loop is equivalent to:

          for (int i = 0; i < 8 ; ++i) {
            cameraProjection(i, totalColor);
          }
        */
        if(numOfCams >= 1)
            cameraProjection(0, totalColor);
        if(numOfCams >= 2)
            cameraProjection(1, totalColor);
        if(numOfCams >= 3)
            cameraProjection(2, totalColor);
        if(numOfCams >= 4)
            cameraProjection(3, totalColor);
        if(numOfCams >= 5)
            cameraProjection(4, totalColor);
        if(numOfCams >= 6)
            cameraProjection(5, totalColor);
        if(numOfCams >= 7)
            cameraProjection(6, totalColor);
        if(numOfCams >= 8)
            cameraProjection(7, totalColor);

        //Full Potential Coverage
        if(is180FOVOn)
            camera180Projection(totalColor);

        vec3 blinnPhongLightRGB;
        if(useTextureMap == 0.0f)
            calculateBlinnPhongLight(blinnPhongLightRGB);

        vec3 fragRGB = totalColor.xyz + mix(blinnPhongLightRGB,
                                          vec3(texture(textureMap, vUVout)), useTextureMap);

        vFragColor = vec4(fragRGB, 1.0f);
}
