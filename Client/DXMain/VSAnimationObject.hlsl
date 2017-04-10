
cbuffer cbWorldMtx : register(b1) {
	float4x4 mtxWorld : packoffset(c0);
};

cbuffer cbSkinned : register(b10){
	// �� ĳ���ʹ� �ִ� ���� ������ 96
	float4x4 gmtxBoneTransforms[256] : packoffset(c0);
};

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
	float3 boneWeight : BONE_WEIGHT;
	float4 boneIndex : BONE_INDEX;
};

// �ȼ� ���̴��� ����� �ȼ��� �� �������Դϴ�.
struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float3 positionV : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
};


VS_OUTPUT main(VS_INPUT input, uint instanceID : SV_InstanceID)
{
	// �迭�� �ʱ�ȭ�Ѵ�. �׷��� ������ SV_POSITION�� ������ ��� �޽����� ���´�.
	float weights[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	weights[0] = input.boneWeight.x;
	weights[1] = input.boneWeight.y;
	weights[2] = input.boneWeight.z;
	weights[3] = 1.0f - (weights[0] + weights[1] + weights[2]);//input.weights.w;

	float3 posL = float3(0.0f, 0.0f, 0.0f);
	float3 normalL = float3(0.0f, 0.0f, 0.0f);

	// ���� ������ ����
	for (int i = 0; i < 4; ++i){
		posL += mul(float4(input.position, 1.0f), gmtxBoneTransforms[input.boneIndex[i]]).xyz * weights[i];
		normalL += mul(input.normal, (float3x3)gmtxBoneTransforms[input.boneIndex[i]]) * weights[i];
	}
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.positionV	= mul(float4(posL, 1.0f), mtxWorld).xyz;
	output.position		= mul(float4(output.positionV, 1.0f), gmtxView);
	output.positionV	= output.position.xyz;
	output.position		= mul(output.position, gmtxProj);
	output.normal		= mul(normalL, (float3x3)mtxWorld);

	output.uv = input.uv;

	return output;
}