
const char* cc3D_PositionNormalTex_vert = R"(

#ifdef USE_NORMAL_MAPPING
#if (MAX_DIRECTIONAL_LIGHT_NUM > 0)
uniform vec3 u_DirLightSourceDirection[MAX_DIRECTIONAL_LIGHT_NUM];
#endif
#endif
#if (MAX_POINT_LIGHT_NUM > 0)
uniform vec3 u_PointLightSourcePosition[MAX_POINT_LIGHT_NUM];
#endif
#if (MAX_SPOT_LIGHT_NUM > 0)
uniform vec3 u_SpotLightSourcePosition[MAX_SPOT_LIGHT_NUM];
#ifdef USE_NORMAL_MAPPING
uniform vec3 u_SpotLightSourceDirection[MAX_SPOT_LIGHT_NUM];
#endif
#endif

attribute vec4 a_position;
attribute vec2 a_texCoord;
attribute vec3 a_normal;
#ifdef USE_NORMAL_MAPPING
attribute vec3 a_tangent;
attribute vec3 a_binormal;
#endif
varying vec2 TextureCoordOut;

#ifdef USE_NORMAL_MAPPING
#if MAX_DIRECTIONAL_LIGHT_NUM
varying vec3 v_dirLightDirection[MAX_DIRECTIONAL_LIGHT_NUM];
#endif
#endif
#if MAX_POINT_LIGHT_NUM
varying vec3 v_vertexToPointLightDirection[MAX_POINT_LIGHT_NUM];
#endif
#if MAX_SPOT_LIGHT_NUM
varying vec3 v_vertexToSpotLightDirection[MAX_SPOT_LIGHT_NUM];
#ifdef USE_NORMAL_MAPPING
varying vec3 v_spotLightDirection[MAX_SPOT_LIGHT_NUM];
#endif
#endif

#ifndef USE_NORMAL_MAPPING
#if ((MAX_DIRECTIONAL_LIGHT_NUM > 0) || (MAX_POINT_LIGHT_NUM > 0) || (MAX_SPOT_LIGHT_NUM > 0))
varying vec3 v_normal;
#endif
#endif

#include <ShadowRecieverVert>

void main(void)
{
    vec4 ePosition = CC_MVMatrix * a_position;
#ifdef USE_NORMAL_MAPPING
    #if ((MAX_DIRECTIONAL_LIGHT_NUM > 0) || (MAX_POINT_LIGHT_NUM > 0) || (MAX_SPOT_LIGHT_NUM > 0))
        vec3 eTangent = normalize(CC_NormalMatrix * a_tangent);
        vec3 eBinormal = normalize(CC_NormalMatrix * a_binormal);
        vec3 eNormal = normalize(CC_NormalMatrix * a_normal);
    #endif
    #if (MAX_DIRECTIONAL_LIGHT_NUM > 0)
        for (int i = 0; i < MAX_DIRECTIONAL_LIGHT_NUM; ++i)
        {
            v_dirLightDirection[i].x = dot(eTangent, u_DirLightSourceDirection[i]);
            v_dirLightDirection[i].y = dot(eBinormal, u_DirLightSourceDirection[i]);
            v_dirLightDirection[i].z = dot(eNormal, u_DirLightSourceDirection[i]);
        }
    #endif

    #if (MAX_POINT_LIGHT_NUM > 0)
        for (int i = 0; i < MAX_POINT_LIGHT_NUM; ++i)
        {
            vec3 pointLightDir = u_PointLightSourcePosition[i].xyz - ePosition.xyz;
            v_vertexToPointLightDirection[i].x = dot(eTangent, pointLightDir);
            v_vertexToPointLightDirection[i].y = dot(eBinormal, pointLightDir);
            v_vertexToPointLightDirection[i].z = dot(eNormal, pointLightDir);
        }
    #endif

    #if (MAX_SPOT_LIGHT_NUM > 0)
        for (int i = 0; i < MAX_SPOT_LIGHT_NUM; ++i)
        {
            vec3 spotLightDir = u_SpotLightSourcePosition[i] - ePosition.xyz;
            v_vertexToSpotLightDirection[i].x = dot(eTangent, spotLightDir);
            v_vertexToSpotLightDirection[i].y = dot(eBinormal, spotLightDir);
            v_vertexToSpotLightDirection[i].z = dot(eNormal, spotLightDir);

            v_spotLightDirection[i].x = dot(eTangent, u_SpotLightSourceDirection[i]);
            v_spotLightDirection[i].y = dot(eBinormal, u_SpotLightSourceDirection[i]);
            v_spotLightDirection[i].z = dot(eNormal, u_SpotLightSourceDirection[i]);
        }
    #endif
#else
    #if (MAX_POINT_LIGHT_NUM > 0)
        for (int i = 0; i < MAX_POINT_LIGHT_NUM; ++i)
        {
            v_vertexToPointLightDirection[i] = u_PointLightSourcePosition[i].xyz - ePosition.xyz;
        }
    #endif

    #if (MAX_SPOT_LIGHT_NUM > 0)
        for (int i = 0; i < MAX_SPOT_LIGHT_NUM; ++i)
        {
            v_vertexToSpotLightDirection[i] = u_SpotLightSourcePosition[i] - ePosition.xyz;
        }
    #endif

    #if ((MAX_DIRECTIONAL_LIGHT_NUM > 0) || (MAX_POINT_LIGHT_NUM > 0) || (MAX_SPOT_LIGHT_NUM > 0))
        v_normal = CC_NormalMatrix * a_normal;
    #endif
#endif

    CCTransferShadow(ePosition);

    TextureCoordOut = a_texCoord;
    TextureCoordOut.y = 1.0 - TextureCoordOut.y;
    gl_Position = CC_PMatrix * ePosition;
}
)";

const char* cc3D_SkinPositionNormalTex_vert = R"(

#ifdef USE_NORMAL_MAPPING
#if (MAX_DIRECTIONAL_LIGHT_NUM > 0)
uniform vec3 u_DirLightSourceDirection[MAX_DIRECTIONAL_LIGHT_NUM];
#endif
#endif
#if (MAX_POINT_LIGHT_NUM > 0)
uniform vec3 u_PointLightSourcePosition[MAX_POINT_LIGHT_NUM];
#endif
#if (MAX_SPOT_LIGHT_NUM > 0)
uniform vec3 u_SpotLightSourcePosition[MAX_SPOT_LIGHT_NUM];
#ifdef USE_NORMAL_MAPPING
uniform vec3 u_SpotLightSourceDirection[MAX_SPOT_LIGHT_NUM];
#endif
#endif

attribute vec3 a_position;

attribute vec4 a_blendWeight;
attribute vec4 a_blendIndex;

attribute vec2 a_texCoord;

attribute vec3 a_normal;
#ifdef USE_NORMAL_MAPPING
attribute vec3 a_tangent;
attribute vec3 a_binormal;
#endif

const int SKINNING_JOINT_COUNT = 60;
// Uniforms
uniform vec4 u_matrixPalette[SKINNING_JOINT_COUNT * 3];

// Varyings
varying vec2 TextureCoordOut;

#ifdef USE_NORMAL_MAPPING
#if MAX_DIRECTIONAL_LIGHT_NUM
varying vec3 v_dirLightDirection[MAX_DIRECTIONAL_LIGHT_NUM];
#endif
#endif
#if MAX_POINT_LIGHT_NUM
varying vec3 v_vertexToPointLightDirection[MAX_POINT_LIGHT_NUM];
#endif
#if MAX_SPOT_LIGHT_NUM
varying vec3 v_vertexToSpotLightDirection[MAX_SPOT_LIGHT_NUM];
#ifdef USE_NORMAL_MAPPING
varying vec3 v_spotLightDirection[MAX_SPOT_LIGHT_NUM];
#endif
#endif

#ifndef USE_NORMAL_MAPPING
#if ((MAX_DIRECTIONAL_LIGHT_NUM > 0) || (MAX_POINT_LIGHT_NUM > 0) || (MAX_SPOT_LIGHT_NUM > 0))
varying vec3 v_normal;
#endif
#endif

#include <ShadowRecieverVert>

void getPositionAndNormal(out vec4 position, out vec3 normal, out vec3 tangent, out vec3 binormal)
{
    float blendWeight = a_blendWeight[0];

    int matrixIndex = int (a_blendIndex[0]) * 3;
    vec4 matrixPalette1 = u_matrixPalette[matrixIndex] * blendWeight;
    vec4 matrixPalette2 = u_matrixPalette[matrixIndex + 1] * blendWeight;
    vec4 matrixPalette3 = u_matrixPalette[matrixIndex + 2] * blendWeight;


    blendWeight = a_blendWeight[1];
    if (blendWeight > 0.0)
    {
        matrixIndex = int(a_blendIndex[1]) * 3;
        matrixPalette1 += u_matrixPalette[matrixIndex] * blendWeight;
        matrixPalette2 += u_matrixPalette[matrixIndex + 1] * blendWeight;
        matrixPalette3 += u_matrixPalette[matrixIndex + 2] * blendWeight;

        blendWeight = a_blendWeight[2];
        if (blendWeight > 0.0)
        {
            matrixIndex = int(a_blendIndex[2]) * 3;
            matrixPalette1 += u_matrixPalette[matrixIndex] * blendWeight;
            matrixPalette2 += u_matrixPalette[matrixIndex + 1] * blendWeight;
            matrixPalette3 += u_matrixPalette[matrixIndex + 2] * blendWeight;

            blendWeight = a_blendWeight[3];
            if (blendWeight > 0.0)
            {
                matrixIndex = int(a_blendIndex[3]) * 3;
                matrixPalette1 += u_matrixPalette[matrixIndex] * blendWeight;
                matrixPalette2 += u_matrixPalette[matrixIndex + 1] * blendWeight;
                matrixPalette3 += u_matrixPalette[matrixIndex + 2] * blendWeight;
            }
        }
    }

    vec4 p = vec4(a_position, 1.0);
    position.x = dot(p, matrixPalette1);
    position.y = dot(p, matrixPalette2);
    position.z = dot(p, matrixPalette3);
    position.w = p.w;

#if ((MAX_DIRECTIONAL_LIGHT_NUM > 0) || (MAX_POINT_LIGHT_NUM > 0) || (MAX_SPOT_LIGHT_NUM > 0))
    vec4 n = vec4(a_normal, 0.0);
    normal.x = dot(n, matrixPalette1);
    normal.y = dot(n, matrixPalette2);
    normal.z = dot(n, matrixPalette3);
#ifdef USE_NORMAL_MAPPING
    vec4 t = vec4(a_tangent, 0.0);
    tangent.x = dot(t, matrixPalette1);
    tangent.y = dot(t, matrixPalette2);
    tangent.z = dot(t, matrixPalette3);
    vec4 b = vec4(a_binormal, 0.0);
    binormal.x = dot(b, matrixPalette1);
    binormal.y = dot(b, matrixPalette2);
    binormal.z = dot(b, matrixPalette3);
#endif
#endif
}

void main()
{
    vec4 position;
    vec3 normal;
    vec3 tangent;
    vec3 binormal;
    getPositionAndNormal(position, normal, tangent, binormal);
    vec4 ePosition = CC_MVMatrix * position;

#ifdef USE_NORMAL_MAPPING
    #if ((MAX_DIRECTIONAL_LIGHT_NUM > 0) || (MAX_POINT_LIGHT_NUM > 0) || (MAX_SPOT_LIGHT_NUM > 0))
        vec3 eTangent = normalize(CC_NormalMatrix * tangent);
        vec3 eBinormal = normalize(CC_NormalMatrix * binormal);
        vec3 eNormal = normalize(CC_NormalMatrix * normal);
    #endif

    #if (MAX_DIRECTIONAL_LIGHT_NUM > 0)
        for (int i = 0; i < MAX_DIRECTIONAL_LIGHT_NUM; ++i)
        {
            v_dirLightDirection[i].x = dot(eTangent, u_DirLightSourceDirection[i]);
            v_dirLightDirection[i].y = dot(eBinormal, u_DirLightSourceDirection[i]);
            v_dirLightDirection[i].z = dot(eNormal, u_DirLightSourceDirection[i]);
        }
    #endif

    #if (MAX_POINT_LIGHT_NUM > 0)
        for (int i = 0; i < MAX_POINT_LIGHT_NUM; ++i)
        {
            vec3 pointLightDir = u_PointLightSourcePosition[i].xyz - ePosition.xyz;
            v_vertexToPointLightDirection[i].x = dot(eTangent, pointLightDir);
            v_vertexToPointLightDirection[i].y = dot(eBinormal, pointLightDir);
            v_vertexToPointLightDirection[i].z = dot(eNormal, pointLightDir);
        }
    #endif

    #if (MAX_SPOT_LIGHT_NUM > 0)
        for (int i = 0; i < MAX_SPOT_LIGHT_NUM; ++i)
        {
            vec3 spotLightDir = u_SpotLightSourcePosition[i] - ePosition.xyz;
            v_vertexToSpotLightDirection[i].x = dot(eTangent, spotLightDir);
            v_vertexToSpotLightDirection[i].y = dot(eBinormal, spotLightDir);
            v_vertexToSpotLightDirection[i].z = dot(eNormal, spotLightDir);

            v_spotLightDirection[i].x = dot(eTangent, u_SpotLightSourceDirection[i]);
            v_spotLightDirection[i].y = dot(eBinormal, u_SpotLightSourceDirection[i]);
            v_spotLightDirection[i].z = dot(eNormal, u_SpotLightSourceDirection[i]);
        }
    #endif
#else
    #if (MAX_POINT_LIGHT_NUM > 0)
        for (int i = 0; i < MAX_POINT_LIGHT_NUM; ++i)
        {
            v_vertexToPointLightDirection[i] = u_PointLightSourcePosition[i].xyz- ePosition.xyz;
        }
    #endif

    #if (MAX_SPOT_LIGHT_NUM > 0)
        for (int i = 0; i < MAX_SPOT_LIGHT_NUM; ++i)
        {
            v_vertexToSpotLightDirection[i] = u_SpotLightSourcePosition[i] - ePosition.xyz;
        }
    #endif

    #if ((MAX_DIRECTIONAL_LIGHT_NUM > 0) || (MAX_POINT_LIGHT_NUM > 0) || (MAX_SPOT_LIGHT_NUM > 0))
        v_normal = CC_NormalMatrix * normal;
    #endif
#endif

    CCTransferShadow(ePosition);

    TextureCoordOut = a_texCoord;
    TextureCoordOut.y = 1.0 - TextureCoordOut.y;
    gl_Position = CC_PMatrix * ePosition;
}

)";