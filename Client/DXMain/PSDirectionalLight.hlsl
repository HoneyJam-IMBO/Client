
//#include "Directional_AmbientLight.hlsli"
#include "UnpackGBuffer.hlsli"
#include "CalcDirectionalLight.hlsli"

Texture2D<float> AOTex : register(t4);
Texture2D<float4> ShadowMap : register(t5);

SamplerState gssWRAP_LINEAR : register(s0);
SamplerState gssWRAP_POINT : register(s1);
SamplerState gssCLAMP_LINEAR : register(s2);
SamplerState gssCLAMP_POINT : register(s3);
SamplerState gssSHADOW : register(s4);

SamplerComparisonState ShadowSampler
{
	// sampler state
	//Filter = MIN_MAG_LINEAR_MIP_POINT;
	AddressU = MIRROR;
	AddressV = MIRROR;

	// sampler comparison state
	ComparisonFunc = LESS;
	Filter = COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
};

cbuffer LightVPCBuffer : register(b4)
{
	matrix gmtxLightV;
	matrix gmtxLightP;

};

struct VS_TEXTURED_OUTPUT {
	float4 position : SV_Position;
	float2 cpPos	: TEXCOORD0;
	float2 uv		: TEXCOORD1;
	float3 rayDir	: TEXCOORD2;
};

float ShaderPCF(float3 position, float bias){

	float4 LightPosV = mul(float4(position, 1.f), gmtxLightV);
	float4 LightPosP = mul(LightPosV, gmtxLightP);
	float3 UVD = LightPosP.xyz / LightPosP.w;

	UVD.y *= -1.f;
	UVD.xy = 0.5 * UVD.xy + 0.5;

	//uv.y *= -1;
	//uv *= 0.5;
	//uv += 0.5;

	
	//float4 LightDepth = ShadowMap.Sample(gssSHADOW, UVD.xy);
	//float fLightDepth = LightDepth.x;
	//if (fLightDepth < fCamposDepth  * 0.98f)
	//{
	//	return 0.5f;
	//}
	//return 1.f;

	float fCamposDepth = ConvertDepthToLinear(UVD.z) /gFar;
	//return ShadowMap.SampleCmpLevelZero(ShadowSampler, UVD.xy, fCamposDepth);

	float sum = 0.f;
	float fxlsize = 1 / 4096.f;
	float x, y;
	//bias = 0.04f;
	for (y = -1.f; y <= 1.f; y += 1.f)
	{
		for (x = -1.f; x <= 1.f; x += 1.f)
		{
			float2 pcfUV = UVD + float2(x * fxlsize, y * fxlsize);
			float LightDepth = ShadowMap.Sample(gssSHADOW, pcfUV).x;

			if (LightDepth < fCamposDepth - bias)
			{
				sum += 0.07f;
			}
		}
	}
	return 1 - sum;
	//return ShadowMap.SampleCmpLevelZero(ShadowSampler, UVD.xy, ConvertDepthToLinear(UVD.z) / gFar * 0.99f);
}

float4 main(VS_TEXTURED_OUTPUT input) : SV_Target{

	float2 uv = input.uv.xy;
	float3 location3 = float3(uv, 0.f);
	SURFACE_DATA gbd = UnpackGBuffer(uv);


	//데이터를 재질 구조체로 변환
	Material mat;
	mat.normal = gbd.Normal.xyz;
	mat.diffuseColor.xyz = gbd.Color;
	mat.diffuseColor.w = 1.0f;//완전 불투명

	mat.specExp = g_SpecPowerRange.x + g_SpecPowerRange.y * gbd.SpecPow;
	mat.specIntensity = gbd.SpecInt;
	
	//float4 LightDepth = ShadowMap.Sample(gssCLAMP_POINT, uv);
	//return LightDepth.xxxx;

	//월드 위치 복원
	float3 positionW = CalcWorldPos(input.cpPos, gbd.LinearDepth);
	float ao = AOTex.Sample(gssCLAMP_POINT, input.uv);
//	return float4(positionW, 1.f);
	//엠비언트 및 디렉셔널 라이트 비중 계산
	float4 finalColor;
	finalColor.xyz = CalcAmbient(mat.normal, mat.diffuseColor.xyz) * ao;
	finalColor.xyz += CalcDirectional(positionW, mat);
	
	float3 LDir = normalize(float3(0.f, 0.f, 0.f) - DirToLight.xyz);
	float bias = 0.05 * tan(acos(dot(normalize(gbd.Normal.xyz), LDir)));
	bias = clamp(bias, 0.0, 0.01);
	/*bias = 0.005f;*/
//	float bias = 0.006f;
	float lighting = ShaderPCF(positionW.xyz, bias);
	/*if (lighting == 1.f)
	{
		finalColor.rgb = 0.3f;
	}*/
	finalColor.rgb *= lighting;
	finalColor.a = 1.0f;
		
	

	return finalColor;
}

/*
sampler랑. rs를 넣었는데도 안되심
*/