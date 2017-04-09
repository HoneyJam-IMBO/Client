//specpowr ����ȭ 
static const float2 g_SpecPowerRange = { 0.1, 250.0 };
static const float gFar = 10000.f;

cbuffer cbgbufferUnpack : register(b0) {
	float4 PerspectiveValues	: packoffset(c0);
	float4x4 ViewInv			: packoffset(c1);
	float4x4 PerspectiveInv		: packoffset(c5);
}

SamplerState        gssSamplerState				: register(s0);

Texture2D           gtxtDepthTexture		 	: register(t0);
Texture2D           gtxtColorSpecInt			: register(t1);
Texture2D           gtxtNormal_Depth			: register(t2);
Texture2D           gtxtSpecPow	 	            : register(t3);

struct SURFACE_DATA {
	float LinearDepth;
	float3 Color;
	float3 Normal;
	float3 SpecInt;
	float SpecPow;
	float depth;
};

//------------------hellper func---------------------


float ConvertDepthToLinear(float depth){
	//(-0.01f / (10000.f - 0.01f)) / (depth - (10000.f / (10000.f - 0.01f)));

	float linearDepth = PerspectiveValues.z / (depth + PerspectiveValues.w);
	return linearDepth;
}

float3 CalcWorldPos(float2 csPos, float depth){
	float4 position;
	
	//position.xy = csPos.xy;
	//position.z = depth;
	//position.w = 1;
	////
	//return mul(mul(position, PerspectiveInv), ViewInv).xyz;

	position.xy = csPos.xy * PerspectiveValues.xy * depth;
	position.z = depth;
	position.w = 1.0;

	return mul(position, ViewInv).xyz;
}
//------------------hellper func---------------------

//------------------unpack gbuffer-------------------
SURFACE_DATA UnpackGBuffer(float2 location) {
	SURFACE_DATA Out = (SURFACE_DATA)0;

	float3 location3 = float3(location, 0);

	//���̽� ����� ����ŧ�� ���� �� ����
	float4 baseColorSpecInt = gtxtColorSpecInt.Sample(gssSamplerState, location);
	//float4 baseColorSpecInt = gtxtColorSpecInt.Load(location3);
	Out.Color = baseColorSpecInt.xyz;

	//Out.SpecInt = baseColorSpecInt.w;
	
	//��� ���ø� �� ��ü ���� ��ȯ �� ����ȭ
	float4 normal_depth = gtxtNormal_Depth.Sample(gssSamplerState, location);
	//float4 normal_depth = gtxtNormal_Depth.Load(location3);
	Out.Normal = normal_depth.yzw;
	Out.Normal = normalize(Out.Normal * 2.0 - 1.0);

	//���� �� ���� �� ���� ���� ������ ��ȯ
	Out.depth = normal_depth.x;
	//Out.depth = gtxtDepthTexture.Load(location3).x;
	Out.LinearDepth = normal_depth.x * gFar;
	//Out.LinearDepth = ConvertDepthToLinear(Out.depth);

	//���� ���� ���� ���� ����ŧ�� �Ŀ� ������ ����
	float4 SpecPowerNorm = gtxtSpecPow.Sample(gssSamplerState, location);
	//float4 SpecPowerNorm = gtxtSpecPow.Load(location3);
	//Out.PositionW = PositionW_SpecPowerNorm.xyz;
	Out.SpecPow = g_SpecPowerRange.x + SpecPowerNorm * g_SpecPowerRange.y;
	Out.SpecInt = SpecPowerNorm.yzw;

	return Out;
}
SURFACE_DATA UnpackGBuffer_Tex(float2 texCoord) {
	SURFACE_DATA Out = (SURFACE_DATA)0;

	//���̽� ����� ����ŧ�� ���� �� ����
	float4 baseColorSpecInt = gtxtColorSpecInt.Sample(gssSamplerState, texCoord);
	//float4 baseColorSpecInt = gtxtColorSpecInt.Load(location3);
	Out.Color = baseColorSpecInt.xyz;
	//Out.SpecInt = baseColorSpecInt.w;
	

	//��� ���ø� �� ��ü ���� ��ȯ �� ����ȭ
	float4 normal_depth = gtxtNormal_Depth.Sample(gssSamplerState, texCoord);
	//float4 normal_depth = gtxtNormal_Depth.Load(location3);
	Out.Normal = normal_depth.xyz;
	Out.Normal = normalize(Out.Normal * 2.0 - 1.0);

	//���� �� ���� �� ���� ���� ������ ��ȯ
	Out.depth = gtxtDepthTexture.Sample(gssSamplerState, texCoord);
	//float depth = gtxtDepthTexture.Load(location3).x;
	//Out.LinearDepth = ConvertLinearDepth(Out.depth);
	
	//Out.LinearDepth = ConvertDepthToLinear(Out.depth);
	Out.LinearDepth = normal_depth.x * gFar;
	//Out.LinearDepth = depth;

	//���� ���� ���� ���� ����ŧ�� �Ŀ� ������ ����
	float4 SpecPowerNorm = gtxtSpecPow.Sample(gssSamplerState, texCoord);
	//float4 PositionW_SpecPowerNorm = gtxtPositionW_SpecPow.Load(location3);
	Out.SpecPow = g_SpecPowerRange.x + SpecPowerNorm.w * g_SpecPowerRange.y;
	Out.SpecInt = SpecPowerNorm.yzw;

	return Out;
}
/*SURFACE_DATA UnpackGBuffer(float2 texCoord) {
	SURFACE_DATA Out = (SURFACE_DATA)0;

	//���̽� ����� ����ŧ�� ���� �� ����
	float4 baseColorSpecInt = gtxtColorSpecInt.Sample(gssSamplerState, texCoord);
	Out.Color = baseColorSpecInt.xyz;
	Out.SpecInt = baseColorSpecInt.w;

	//��� ���ø� �� ��ü ���� ��ȯ �� ����ȭ
	float4 normal_depth = gtxtNormal_Depth.Sample(gssSamplerState, texCoord);
	Out.Normal = normal_depth.xyz;
	Out.Normal = normalize(Out.Normal * 2.0 - 1.0);

	//���� �� ���� �� ���� ���� ������ ��ȯ
	float depth = normal_depth.w;
	Out.LinearDepth = ConvertDepthToLinear(depth);

	//���� ���� ���� ���� ����ŧ�� �Ŀ� ������ ����
	float SpecPowerNorm = gtxtSpecPow.Sample(gssSamplerState, texCoord);
	Out.SpecPow = SpecPowerNorm.x + SpecPowerNorm * g_SpecPowerRange.y;

	return Out;
}*/
//------------------unpack gbuffer-------------------
