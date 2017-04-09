#include "UnpackGBuffer.hlsli"

texture2D gtxtViewer : register(t0);

SamplerState gssWRAP_LINEAR : register(s0);
SamplerState gssWRAP_POINT : register(s1);
SamplerState gssCLAMP_LINEAR : register(s2);
SamplerState gssCLAMP_POINT : register(s3);

struct VS_TEXTURED_OUTPUT {
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
};

float4 main(VS_TEXTURED_OUTPUT input) : SV_Target{
	float4 cColor = gtxtViewer.Sample(gssWRAP_LINEAR, input.texCoord);
	if (cColor.x < 0.1)
		return float4(1.f, 0.f, 1.f, 1.f);
	return float4(cColor.x, cColor.x, cColor.x, 1.f);

	float depth = cColor.x;
	float2 cpPos = (input.texCoord * 2.f) - 1.f;//0-1 -> -1~1
	float3 location3 = float3(cpPos, 0);
	//float depth = gtxtViewer.Load(location3).x;
	float Zn = 0.01f;
	float Zf = 10000.f;
	float Q = (Zf / (Zn - Zf));
	//float linearDepth = -(Q*Zn) / (depth - Q);
	//float linearDepth = ConvertDepthToLinear(depth);
	float linearDepth = (-0.01f/ (10000.f - 0.01f))/ (depth - (10000.f / (10000.f - 0.01f)));
	//float3 positionW = CalcWorldPos(location3.xy, linearDepth);
	float4 finalColor;
	finalColor = float4(1.0 - saturate(linearDepth / 10), 1.0 - saturate(linearDepth / 25), 1.0 - saturate(linearDepth / 1000.0), 0.0);
	//오호 이게 xyz에 있는 10 25 1000이 제한 거리네 오호 신기해랑 맞네 ㅂㄷㅂㄷ ㅋㅋㅋㅋㅋㅋ
	//제한거리! 이거 깊이 텍스쳐 그릴때 써먹어도 될듯
	//finalColor = float4(positionW, 0.0);
	//finalColor = float4(0.f, 0.f, 1.0 - saturate(linearDepth / 200.0), 0.0);
	//return finalColor;

	return float4(linearDepth, linearDepth, linearDepth, 1.f);

	
	//cColor.r /= 1000;
	//cColor.r += 0.05;
}