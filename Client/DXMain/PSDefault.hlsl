#include "PackGbuffer.hlsli"

struct Material {
	float3 normal;
	float4 diffuseColor;
	float specExp;
	float specIntensity;
};

cbuffer gMaterialInfo : register(b3) {
	float4 gMaterialColor : packoffset(c0);
	float gSpecExp : packoffset(c1.x);
	float gSpecIntensity : packoffset(c1.y);
}


//texture
Texture2D    gtxtDefault : register(t0);
Texture2D    gtxtSpec : register(t1);
Texture2D    gtxtCP : register(t2);

SamplerState gssWRAP_LINEAR : register(s0);
SamplerState gssWRAP_POINT : register(s1);
SamplerState gssCLAMP_LINEAR : register(s2);
SamplerState gssCLAMP_POINT : register(s3);


//texture

struct PixelShaderInput
{
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
};

// (보간된) 색 데이터에 대한 통과 함수입니다.
PS_GBUFFER_OUT main(PixelShaderInput input)
{
	PS_GBUFFER_OUT output = (PS_GBUFFER_OUT)0;
	float4 cCPColor = gtxtCP.Sample(gssWRAP_LINEAR, input.uv);
	clip(cCPColor.g < 0.05f ? -1 : 1);

	float4 cColor = gtxtDefault.Sample(gssWRAP_LINEAR, input.uv) * gMaterialColor;
	float4 cSpecColor = gtxtSpec.Sample(gssWRAP_LINEAR, input.uv);
	//cColor *= cSpecColor;
	//cColor = gMaterialColor;

	float Depth = input.position.z / input.position.w;
	
	return (PackGBuffer(cColor.xyz, normalize(input.normal), cSpecColor.xyz, gSpecExp));

}
//float4 main(PixelShaderInput input) : SV_TARGET
//{
//
//return gMaterialColor;
//
//}
