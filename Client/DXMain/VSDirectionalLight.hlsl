
#include "UnpackGBuffer.hlsli"
#include "CalcDirectionalLight.hlsli"

static const float2 arrBasePos[4] = {
	float2(-1.0, -1.0),
	float2(-1.0, 1.0),
	float2(1.0, -1.0),
	float2(1.0, 1.0)
};
static const float2 arrUV[4] = {
	float2(0, 1),
	float2(0, 0),
	float2(1, 1),
	float2(1, 0)
};
//
struct VS_DIRECTIONAL_OUTPUT {
	float4 position : SV_Position;
	float2 cpPos	: TEXCOORD0;
	float2 uv		: TEXCOORD1;
	float3 rayDir	: TEXCOORD2;
};

//¥Ÿ¡ﬂ ∑ª¥ı≈∏∞Ÿ
VS_DIRECTIONAL_OUTPUT  main(uint vertexID : SV_VertexID)
{
	VS_DIRECTIONAL_OUTPUT  output = (VS_DIRECTIONAL_OUTPUT)0;
	output.position = float4(arrBasePos[vertexID], 0.0f, 1.0f);
	output.cpPos = output.position.xy;
	//output.cpPos = output.position.xy; // float4(arrUV[vertexID], 0.0f, 1.0f);
	output.uv = float4(arrUV[vertexID], 0.0f, 1.0f); 

	float4 rayDirview = mul(float4(output.uv.xy, 1.f, 1.f) * gFar, PerspectiveInv);
	output.rayDir = mul(rayDirview, ViewInv).xyz;
	output.rayDir -= EyePosition.xyz;

	return(output);
}
