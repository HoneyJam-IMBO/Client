


Texture2D	gtxtUI		: register(t0);
sampler		gSamplerD	: register(s0);
sampler		gSamplerP	: register(s1);
//sampler		gSampler	: register(s2);

sampler		gSampler	: register(s2);

sampler textureSampler = sampler_state
{
	addressU = Clamp;
	addressV = Clamp;
	mipfilter = NONE;
	minfilter = LINEAR;
	magfilter = LINEAR;
};

struct VS_OUTPUT {
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
};

float4 main(VS_OUTPUT input) : SV_TARGET
{
	float4 cBaseTexColor = gtxtUI.Sample(textureSampler, input.texCoord);
	cBaseTexColor.a = 1.f;
	
	return cBaseTexColor;
}