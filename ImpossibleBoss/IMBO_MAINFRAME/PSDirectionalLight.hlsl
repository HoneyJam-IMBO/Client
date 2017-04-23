
//#include "Directional_AmbientLight.hlsli"
#include "UnpackGBuffer.hlsli"
#include "CalcDirectionalLight.hlsli"
#include "CalcDirectionalShadow.hlsli"

Texture2D<float> AOTex : register(t4);

SamplerState gssWRAP_LINEAR : register(s0);
SamplerState gssWRAP_POINT : register(s1);
SamplerState gssCLAMP_LINEAR : register(s2);
SamplerState gssCLAMP_POINT : register(s3);

struct VS_TEXTURED_OUTPUT {
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD0;
};

float4 main(VS_TEXTURED_OUTPUT input) : SV_Target{

	
	SURFACE_DATA gbd = UnpackGBuffer_Tex(input.uv);

if (gbd.depth > 0.99999) {
	return float4(gbd.Color.xyz, 1.0);
}

	//데이터를 재질 구조체로 변환
	Material mat;
	mat.normal = gbd.Normal.xyz;
	mat.diffuseColor.xyz = gbd.Color;
	mat.diffuseColor.w = 1.0f;//완전 불투명

	mat.specExp = g_SpecPowerRange.x + g_SpecPowerRange.y * gbd.SpecPow;
	mat.specIntensity = gbd.SpecInt;

	//월드 위치 복원
	float2 cpPos = input.uv * float2(2.0, -2.0) - float2(1.0, -1.0);
	float3 positionW = CalcWorldPos(cpPos, gbd.LinearDepth);
	float ao = AOTex.Sample(gssCLAMP_LINEAR, input.uv);

	//엠비언트 및 디렉셔널 라이트 비중 계산
	float4 finalColor;
	finalColor.xyz = CalcAmbient(mat.normal, mat.diffuseColor.xyz) * ao;
	finalColor.xyz += CalcDirectional(positionW, mat);
	//finalColor.xyz = mat.normal;

	float3 dir = float3(0, 0, 0) - DirToLight;
	finalColor.rgb *= ShaderPCF(positionW, dot(mat.normal, dir));
		
	finalColor.a = 1.0f;
	return finalColor;
}