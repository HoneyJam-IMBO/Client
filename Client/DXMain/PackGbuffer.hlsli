

//specpowr 정규화 
static const float2 g_SpecPowerRange = { 0.1, 250.0 };
static const float gFar = 10000.f;

struct PS_GBUFFER_OUT {
	float4 Normal : SV_TARGET0;
	float4 ColorSpecInt : SV_TARGET1;
	float4 SpecPow : SV_TARGET2;	
};

PS_GBUFFER_OUT PackGBuffer(float3 BaseColor, float4 Normal, float3 SpecIntensity, float SpecPower) {
	PS_GBUFFER_OUT Out = (PS_GBUFFER_OUT)0;

	//스펙큘러 파워 정규화
	float SpecPowerNorm = (SpecPower - g_SpecPowerRange.x) / g_SpecPowerRange.y;

	//GBuffer 구조체에 데이터 패킹
	//Out.ColorSpecInt = float4(BaseColor.rgb, SpecIntensity);
	Out.ColorSpecInt = float4(BaseColor.rgb, 0.f);
	Out.Normal = float4(Normal.w, Normal.xyz * 0.5 + 0.5);
	//Out.Normal_Depth = float4(Normal.xyz, Depth);

	//specPow값 패킹
	Out.SpecPow = float4(SpecPowerNorm, SpecIntensity);
	//Out.PositionW_SpecPow.xyz = PositionW;

	
	return Out;
}