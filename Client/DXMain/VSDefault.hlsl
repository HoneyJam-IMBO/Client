cbuffer ViewProjectionConstantBuffer : register(b0)
{
	matrix gmtxView;
	matrix gmtxProj;
};

struct VS_INPUT
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
	float4x4 mtxWorld : INSWORLDMTX;
};

// 픽셀 셰이더를 통과한 픽셀당 색 데이터입니다.
struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float3 positionV : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
};


VS_OUTPUT main(VS_INPUT input, uint instanceID : SV_InstanceID)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.positionV = mul(float4(input.position, 1.0f), input.mtxWorld).xyz;
	output.position = mul(float4(output.positionV, 1.0f), gmtxView);
	output.positionV = output.position.xyz;
	output.position = mul(output.position, gmtxProj);
	output.normal = mul(input.normal, (float3x3)input.mtxWorld);

	output.uv = input.uv;
	//output.color = float4(1.0f,1.0f,0.0f, 1.0f);

	return output;
}
