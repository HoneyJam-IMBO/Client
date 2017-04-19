Texture2D           gtxtDepth				: register(t0);
Texture2D           gtxtNormal				: register(t2);

//Texture2D<float> DepthTex : register(t0);
//Texture2D<float3> NormalsTex : register(t1);
RWStructuredBuffer<float4> MiniDepthRW : register(u0);
cbuffer DownScaleConstants : register(b0){
	uint2 Res : packoffset(c0);   // Resulotion of the down scaled image: x - width, y - height
	float2 ResRcp  : packoffset(c0.z);
	float4 ProjParams : packoffset(c1);
	float4x4 ViewMatrix : packoffset(c2);
	float OffsetRadius : packoffset(c6);
	float Radius : packoffset(c6.y);
	float MaxDepth : packoffset(c6.z);
}


float ConvertDepthToLinear(float depth){
	float linearDepth = ProjParams.z / (depth + ProjParams.w);
	return linearDepth;
}
//cbuffer DownScaleConstants : register(b0) {
//	uint2 Res : packoffset(c0);
//	float2 ResRcp : packoffset(c0.z);
//	float4 PerspectiveValues : packoffset(c1);
//	float4x4 ViewMtx : packoffset(c2);
//	float Offsetradius : packoffset(c6.x);
//	float Radius : packoffset(c6.y);
//	float MaxDepth : packoffset(c6.z);
//};
//
//float ConvertDepthToLinear(float depth) {
//	float linearDepth = (PerspectiveValues.z / (depth + PerspectiveValues.w));
//	return linearDepth;
//}

//총 픽셀 수를 1024로 나눈 만큼의 x스레드 그룹을 다운 스케일 한 컴퓨트 세이더에 적용
[numthreads(1024, 1, 1)]
void main( uint3 dispatchThreadId : SV_DispatchThreadID ){
	uint3 CurPixel = uint3(dispatchThreadId.x % Res.x, dispatchThreadId.x / Res.x, 0);
	if (CurPixel.y < Res.y)
	{
		float minDepth = 1.0;
		float3 avgNormalWorld = float3(0.0, 0.0, 0.0);
		uint3 FullResPixel = CurPixel * 2;

		[unroll]
		for (int i = 0; i < 2; i++)
		{
			[unroll]
			for (int j = 0; j < 2; j++)
			{
				//가장 작은 depth를 저장한다.
				float curDepth = gtxtDepth.Load(FullResPixel, int2(j, i));
				minDepth = min(curDepth, minDepth);

				//평균 normal을 저장한다.
				float3 normalWorld = gtxtNormal.Load(FullResPixel, int2(j, i));
				avgNormalWorld += normalize(normalWorld * 2.0 - 1.0);
			}
		}

		float3 avgNormalView = mul(avgNormalWorld * 0.25, (float3x3)ViewMatrix);
		MiniDepthRW[dispatchThreadId.x].x = ConvertDepthToLinear(minDepth);
		MiniDepthRW[dispatchThreadId.x].yzw = avgNormalView;
		//MiniDepthRW[CurPixel.xy] = float4(avgNormalView, ConvertDepthToLinear(minDepth));
	}
}