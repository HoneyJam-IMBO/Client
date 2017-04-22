Texture2D<float> DepthTex : register(t0);
RWTexture2D<float> OcclussionRW : register(u0);

cbuffer OcclussionConstants : register(b0) {
	uint2 Res : packoffset(c0);
}

[numthreads(1024, 1, 1)]
void main( uint3 dispatchThreadId : SV_DispatchThreadID ){
	uint3 CurPixel = uint3(dispatchThreadId.x % Res.x, dispatchThreadId.x / Res.x, 0);

	if (CurPixel.y < Res.y){
		// Get the depth
		float curDepth = DepthTex.Load(CurPixel);
	
		// sky box¸é 1 return
		OcclussionRW[CurPixel.xy] = (curDepth > 0.99);
	}
}