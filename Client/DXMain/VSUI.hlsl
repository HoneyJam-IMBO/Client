



cbuffer ViewProjectionConstantBuffer : register(b4)
{
	matrix		g_mtxViewProjection;
	//float		g_fAlpha;
};

struct VS_OUTPUT {
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
};

VS_OUTPUT main(float3 position : POSITION,
			float2 texCoord : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.position = mul(float4(position, 1.0f), g_mtxViewProjection);
	output.texCoord = texCoord;

	return output;
}