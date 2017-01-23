#include "Uniforms.glsl"
#include "Samplers.glsl"
#include "Transform.glsl"
#include "ScreenPos.glsl"

#ifdef COMPILEPS
uniform vec3 cMapMinPoint;
uniform vec3 cMapMaxPoint;
uniform vec4 cDefaultColor;
#endif

varying vec3 vFarRay;
varying vec4 vScreenPos;
varying vec2 vScreenPosPreDiv;

void VS()
{
    mat4 modelMatrix = iModelMatrix;
    vec3 worldPos = GetWorldPos (modelMatrix);
    gl_Position = GetClipPos (worldPos);
    vScreenPos = GetScreenPos (gl_Position);
    vScreenPosPreDiv = GetScreenPosPreDiv (gl_Position);
    vFarRay = GetFarRay (gl_Position) * gl_Position.w;
}

void PS()
{
    vec3 diffRGB = texture2D (sDiffMap, vScreenPosPreDiv).rgb;
    float depth = ReconstructDepth (texture2DProj (sDepthBuffer, vScreenPos).r);
    vec3 worldPos = vFarRay * depth / vScreenPos.w;
    worldPos += cCameraPosPS;

    if (worldPos.x > cMapMaxPoint.x ||
        worldPos.x < cMapMinPoint.x ||
        worldPos.z > cMapMaxPoint.z ||
        worldPos.z < cMapMinPoint.z)
    {
        gl_FragColor = vec4 (cDefaultColor.x, cDefaultColor.y, cDefaultColor.z, 1.0);
    }
    else
    {
        vec3 delta = cMapMaxPoint - cMapMinPoint;
        vec3 localCoord = worldPos - cMapMinPoint;

        vec2 maskCoord;
        maskCoord.x = localCoord.x / delta.x;
        maskCoord.y = 1.0 - localCoord.z / delta.z;

        vec3 maskRGB = texture2D (sNormalMap, maskCoord).rgb;
        gl_FragColor = vec4 (diffRGB.x * maskRGB.x, diffRGB.y * maskRGB.y, diffRGB.z * maskRGB.z, 1.0);
    }
}
