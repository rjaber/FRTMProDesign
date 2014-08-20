#version 330 core

layout(location = 0) in vec3 vVertex;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vUV;

uniform mat4 M;
uniform mat4 MV;
uniform mat4 MVP;

uniform mat4 projectionMat[8];  //projector matrices of cameras
uniform mat4 potentialProjMat;  //projector matrix of 180 FOV camera
uniform int numOfCams;
uniform bool is180FOVOn;

smooth out vec2 vUVout;
smooth out vec3 vEyeSpaceNormal;
smooth out vec3 vEyeSpacePosition;

smooth out vec4 projTexCoord[8];    //texture coords for camera's  view coverage
smooth out vec4 projPotentialCoord; //texture coords for camera's 180 FOV coverage

uniform vec4 yClipPlane;
float gl_ClipDistance[1];

void main() {

        for(int i=0; i<numOfCams; ++i)
            projTexCoord[i] = projectionMat[i] * ( M * vec4(vVertex, 1.0f) );

        if(is180FOVOn)
            projPotentialCoord = potentialProjMat * ( M * vec4(vVertex, 1.0f) );

        vUVout = vUV;

        vEyeSpacePosition = (MV * vec4(vVertex, 1)).xyz;
        vEyeSpaceNormal = normalize(mat3(MV) * vNormal);

        gl_ClipDistance[0] = dot(M * vec4(vVertex, 1.0f), yClipPlane);
        gl_Position = MVP * vec4(vVertex,1);
}
