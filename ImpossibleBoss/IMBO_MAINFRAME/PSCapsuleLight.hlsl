#include "UnpackGBuffer.hlsli"
#include "CalcCapsuleLight.hlsli"


struct DS_OUTPUT
{
	float4 Position	: SV_POSITION;
	float2 cpPos	: TEXCOORD0;
	uint instanceID : TEXCOORD1;
	// TODO: 다른 내용을 변경/추가합니다.
};

float4 main(DS_OUTPUT input) : SV_TARGET
{
	//return float4(1.0,1.0,1.0,1.0);

	//GBuffer 언패킹
	//float2 uv = input.UV;
	float2 uv = input.Position.xy;
	SURFACE_DATA gbd = UnpackGBuffer(uv);
	//return float4(gbd.Normal,1.0);
	//재질 구조체로 데이터 변환
	Material mat;
	mat.normal = gbd.Normal;
	mat.diffuseColor.xyz = gbd.Color;
	mat.diffuseColor.w = 1.0f;
	mat.specExp = g_SpecPowerRange.x + g_SpecPowerRange.y + gbd.SpecPow;
	mat.specIntensity = gbd.SpecInt;

	//월드 위치 복원
	float3 positionW = CalcWorldPos(input.cpPos, gbd.LinearDepth);

	//return mat.diffuseColor;
	//조명 분포 계산
	float4 finalColor;
	//finalColor.xyz = CalcCapsule(gbd.PositionW, mat, input.instanceID);
	finalColor.xyz = CalcCapsule(positionW, mat, input.instanceID);

	//finalColor.xyz = mat.diffuseColor.xyz;
	finalColor.w = 1.0f;

	return finalColor;
}