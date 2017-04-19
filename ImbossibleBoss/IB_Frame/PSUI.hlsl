


Texture2D	gtxtUI		: register(t0);
sampler		gSampler	: register(s0);



struct VS_OUTPUT {
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
};

float4 main(VS_OUTPUT input) : SV_TARGET
{
	float4 cBaseTexColor = gtxtUI.Sample(gSampler, input.texCoord);
	cBaseTexColor.a = 1.f;
	return cBaseTexColor;
}